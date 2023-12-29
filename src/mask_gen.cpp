#include "maskgen.h"

namespace maskgen {
  void bgSubtract(cv::VideoWriter& input_vid, cv::VideoWriter* mask) {
    cv::Ptr<cv::bgsegm::BackgroundSubtractorLSBP> p_bgsub = cv::bgsegm::createBackgroundSubtractorLSBP();
    
     
  }
}