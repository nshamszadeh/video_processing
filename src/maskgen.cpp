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
        while (cv::waitKey(1)) {
            
        }
        
    }
}
