#pragma once
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/videoio.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

namespace maskgen {
    std::string bgSubtract(std::string src_path);
    std::string imageSegmentation(std::string src_path);
}
