#include "maskgen.h"
#include "util.h"

namespace maskgen {
  cv::VideoWriter bgSubtract(std::string src_path) {
    cv::Ptr<cv::bgsegm::BackgroundSubtractorLSBP> p_bgsub = cv::bgsegm::createBackgroundSubtractorLSBP();
    util::VideoParameters vp(src_path);
    cv::VideoCapture src_vid(src_path);
    cv::VideoWriter mask(vp.name + "_mask.avi", cv::VideoWriter::fourcc('M','J','P','G'), vp.fps, vp.frameSize, false);
    cv::Mat frame;
    cv::Mat mask_frame = cv::Mat::zeros(src_vid.get(cv::CAP_PROP_FRAME_HEIGHT), 
                         src_vid.get(cv::CAP_PROP_FRAME_WIDTH), CV_8UC1);
    if (!src_vid.isOpened()) {
				TRACE("Something went wrong here.");
				return mask;
		}
    const size_t frame_count = src_vid.get(cv::CAP_PROP_FRAME_COUNT);
    for (size_t i = 0; i < frame_count; i++) {
      src_vid >> frame;
      p_bgsub->apply(frame, mask_frame);
      IMSHOW("mask", mask_frame);
      mask.write(mask_frame);
    }
    src_vid.release();
    return mask;
  }
}