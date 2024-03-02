#include <format>
#include "maskgen.h"
#include "util.h"

namespace maskgen {
    std::string bgSubtract(std::string src_path) {
        cv::Ptr<cv::bgsegm::BackgroundSubtractorLSBP> p_bgsub = cv::bgsegm::createBackgroundSubtractorLSBP();
        util::VideoParameters vp(src_path);
        cv::VideoCapture src_vid(src_path);
        std::string mask_path = vp.name + "_mask.avi";
        cv::VideoWriter mask(mask_path, cv::VideoWriter::fourcc('M','J','P','G'), vp.fps, vp.frameSize, false);
        cv::Mat frame, mask_frame, mask_frame_8UC1;
        if (!src_vid.isOpened()) {
            TRACE("Something went wrong here.");
            return "";
        }
        const size_t frame_count = src_vid.get(cv::CAP_PROP_FRAME_COUNT);
        for (size_t i = 0; i < frame_count; i++) {
            src_vid >> frame;
            p_bgsub->apply(frame, mask_frame);
            cv::extractChannel(mask_frame, mask_frame_8UC1, 0);
            IMSHOW("mask", mask_frame_8UC1);
            mask.write(mask_frame_8UC1);
            std::string mat_type;
            GET_MAT_TYPE(mask_frame_8UC1.type(), mat_type);
        }
        mask.release();
        src_vid.release();
        return mask_path;
    }

    std::string imageSegmentation(std::string src_path) {
        // https://learnopencv.com/deep-learning-based-object-detection-and-instance-segmentation-using-mask-rcnn-in-opencv-python-c/
        std::vector<std::string> classes;
        std::vector<cv::Scalar> colors;
        const std::string mscoco_path = std::getenv("MSCOCO_PATH");
        if (mscoco_path.data() == nullptr) {
            TRACE("Need to export MSCOCO_PATH env var");
            return "";
        }
        // initialize parameters
        float confThreshold = 0.5;
        float maskThreshold = 0.3;
        // load the model and classes
        std::string classesFile = mscoco_path + "/mscoco_labels.names";
        std::ifstream ifs(classesFile);
        std::string line;
        while (std::getline(ifs, line)) classes.push_back(line);
        // load the colors
        std::string colorsFile = mscoco_path + "/colors.txt";
        std::ifstream colorFptr(colorsFile);
        while (std::getline(colorFptr, line)) {
            char* pEnd;
            double r, g, b;
            r = std::strtod(line.c_str(), &pEnd);
            g = std::strtod(pEnd, NULL);
            b = std::strtod(pEnd, NULL);
            colors.push_back(cv::Scalar(r, g, b, 255.0));
        }
        // Give the configuration and weight files for the model
        std::string textGraph = mscoco_path + "/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";
        std::string modelWeights = mscoco_path + "/frozen_inference_graph.pb";
        // Load the network
        cv::dnn::Net net = cv::dnn::readNetFromTensorflow(modelWeights, textGraph);
#ifdef _CUDA
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
#else
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
#endif
        // Generate mask file name
        util::VideoParameters vp(src_path);
        cv::VideoCapture src_vid(src_path);
        std::string mask_path = vp.name + "_mask.avi";
        // process each frame
        cv::Mat frame, mask, blob;
        while (cv::waitKey(1)) {
            src_vid >> frame;
            if (frame.empty()) {
                TRACE("EMPTY FRAME");
                break;
            }
            // create 4d blob from image
            cv::dnn::blobFromImage(frame, blob, 1.0, cv::Size(frame.cols, frame.rows), cv::Scalar(), true, false);
            // sets the input to the network
            net.setInput(blob);
            // runs the forward pass to get output from the output layers
            std::vector<std::string> outNames(2);
            outNames[0] = "detection_out_final";
            outNames[1] = "detection_masks";
            std::vector<cv::Mat> outs;
            net.forward(outs, outNames);
            // extract the bounding box and mask for each of the detected objects
            postProcess(frame, outs);
            // Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
            std::vector<double> layersTimes;
            double freq = cv::getTickFrequency();
            double t = net.getPerfProfile(layersTimes) / freq;
            std::string label = std::format("Mask-RCNN on some ryzen cpu or possibly an rtx 3070, Inference time for a frame : %0.0f ms", t);
            cv::putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0,0,0));

            // Write the frame with the detection boxes
            cv::Mat detectedFrame;
            frame.convertTo(detectedFrame, cv::CV_8U);
            
        }
        
    }

        // For each frame, extract the bounding box and mask for each detected object
    void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs) {
        cv::Mat outDetections = outs[0];
        cv::Mat outMasks = outs[1];
        
        // Output size of masks is NxCxHxW where
        // N - number of detected boxes
        // C - number of classes (excluding background)
        // HxW - segmentation shape
        const int numDetections = outDetections.size[2];
        const int numClasses = outMasks.size[1];
        
        outDetections = outDetections.reshape(1, outDetections.total() / 7);
        for (int i = 0; i < numDetections; ++i) {
            float score = outDetections.at<float>(i, 2);
            if (score > confThreshold) {
                // Extract the bounding box
                int classId = static_cast<int>(outDetections.at<float>(i, 1));
                int left = static_cast<int>(frame.cols * outDetections.at<float>(i, 3));
                int top = static_cast<int>(frame.rows * outDetections.at<float>(i, 4));
                int right = static_cast<int>(frame.cols * outDetections.at<float>(i, 5));
                int bottom = static_cast<int>(frame.rows * outDetections.at<float>(i, 6));
                
                left = max(0, min(left, frame.cols - 1));
                top = max(0, min(top, frame.rows - 1));
                right = max(0, min(right, frame.cols - 1));
                bottom = max(0, min(bottom, frame.rows - 1));
                Rect box = Rect(left, top, right - left + 1, bottom - top + 1);
                
                // Extract the mask for the object
                cv::Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i,classId));
                
                // Draw bounding box, colorize and show the mask on the image
                drawBox(frame, classId, score, box, objectMask);
                
            }
        }
    }

    // Draw the predicted bounding box, colorize and show the mask on the image
    void drawBox(cv::Mat& frame, int classId, float conf, Rect box, cv::Mat& objectMask) {
        //Draw a rectangle displaying the bounding box
        cv::rectangle(frame, cv::Point(box.x, box.y), cv::Point(box.x+box.width, box.y+box.height), cv::Scalar(255, 178, 50), 3);
        
        //Get the label for the class name and its confidence
        std::string label = format("%.2f", conf);
        if (!classes.empty()) {
            CV_Assert(classId < (int)classes.size());
            label = classes[classId] + ":" + label;
        }
        
        //Display the label at the top of the bounding box
        int baseLine;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        box.y = max(box.y, labelSize.height);
        cv::rectangle(frame, cv::Point(box.x, box.y - round(1.5*labelSize.height)), cv::Point(box.x + round(1.5*labelSize.width), box.y + baseLine), cv::Scalar(255, 255, 255), FILLED);
        cv::putText(frame, label, cv::Point(box.x, box.y), FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,0),1);

        cv::Scalar color = colors[classId%colors.size()];
        
        // Resize the mask, threshold, color and apply it on the image
        cv::resize(objectMask, objectMask, cv::Size(box.width, box.height));
        cv::Mat mask = (objectMask > maskThreshold);
        cv::Mat coloredRoi = (0.3 * color + 0.7 * frame(box));
        coloredRoi.convertTo(coloredRoi, CV_8UC3);

        // Draw the contours on the image
        std::vector<cv::Mat> contours;
        cv::Mat hierarchy;
        mask.convertTo(mask, CV_8U);
        cv::findContours(mask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        cv::drawContours(coloredRoi, contours, -1, color, 5, LINE_8, hierarchy, 100);
        coloredRoi.copyTo(frame(box), mask);

    }
}
