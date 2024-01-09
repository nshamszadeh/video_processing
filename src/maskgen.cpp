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
      if (i == 24){TRACE("mask_frame_8UC1.type(): ", mat_type, "\n mask_frame_8UC1.rows: ", mask_frame_8UC1.rows, " mask_frame_8UC1.cols: ", mask_frame_8UC1.cols);}
    }
    mask.release();
    src_vid.release();
    return mask_path;
  }
}