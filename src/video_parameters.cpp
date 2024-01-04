#include "util.h"

namespace util {
  std::string getOutputFilename(std::string video_path) {
    std::string output_name = video_path.substr(video_path.find_last_of("/") + 1);
    std::string::size_type const p(output_name.find_last_of("."));
    output_name = output_name.substr(0, p);
    //output_name += "_modified.avi";
    return output_name;
  }

  VideoParameters::VideoParameters(std::string filename) {
    cv::VideoCapture src(filename);
    if (!src.isOpened()) TRACE("Could not open video ", filename);
    fourcc = static_cast<int>(src.get(cv::CAP_PROP_FOURCC));
    fps = src.get(cv::CAP_PROP_FPS);
    name = getOutputFilename(filename);
    frameSize = cv::Size(static_cast<int>(src.get(cv::CAP_PROP_FRAME_WIDTH)), 
                                  static_cast<int>(src.get(cv::CAP_PROP_FRAME_HEIGHT)));
    //isColor = (src.get(cv::CAP_PROP_FORMAT) == CV_8UC3); // doesn't catch all cases
  }

  VideoParameters::VideoParameters(cv::VideoCapture src) {
    fourcc = static_cast<int>(src.get(cv::CAP_PROP_FOURCC));
    fps = src.get(cv::CAP_PROP_FPS);
    name = "output_vid.avi";
    int height = static_cast<int>(src.get(cv::CAP_PROP_FRAME_HEIGHT));
    int width = static_cast<int>(src.get(cv::CAP_PROP_FRAME_WIDTH));
    frameSize = cv::Size(width, height);
    //isColor = (src.get(cv::CAP_PROP_FORMAT) == CV_8UC3); // doesn't catch all cases
  }
}