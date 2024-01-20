#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/videoio.hpp>

namespace maskgen {
    std::string bgSubtract(std::string src_path);
}
