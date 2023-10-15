#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor
#include <functional> // std::function
#include <algorithm> // std::min
#include <cmath> // std::fmod

// recall opencv image pixels are stored in BGR format by default
// TODO: Make BGR -> HLS conversion happen only once per image Mat (regardless of number of sorts)
namespace comparator {
  inline float get_hue(const cv::Vec3b& v1) {
    auto minc_v1 = std::min(v1[0], v1[1], v1[2]);
    auto maxc_v1 = std::max(v1[0], v1[1], v1[2]);

    float v1_hue = 0.0;

    if (minc_v1 < maxc_v1) { // minc_v1 == maxc_v1 => v1_hue == 0
      auto diff_v1 = maxc_v1 - minc_v1;
      auto red_v1 = (maxc_v1 - v1[2]) / diff_v1;
      auto green_v1 = (maxc_v1 - v1[1]) / diff_v1;
      auto blue_v1 = (maxc_v1 - v1[0]) / diff_v1;
      if (red_v1 == maxc_v1) v1_hue = blue_v1 - green_v1;
      else if (green_v1 == maxc_v1) v1_hue = 2.0 + red_v1 - blue_v1;
      else v1_hue = 4.0 + green_v1 - red_v1;

      v1_hue = std::fmod(v1_hue, 6.0);
      return v1_hue;
    }
  }

  std::function<bool(const cv::Vec3b&, const cv::Vec3b&)> lightness = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    //auto minc_v1 = std::min(v1[0], v1[1], v1[2]);
    //auto minc_v2 = std::min(v2[0], v2[1], v2[2]);
    //auto maxc_v1 = std::max(v1[0], v1[1], v1[2]);
    //auto maxc_v2 = std::max(v2[0], v2[1], v2[2]);
    cv::Vec3b hls_v1(v1);
    cv::Vec3b hls_v2(v2);
    cv::cvtColor(v1, hls_v1, cv::COLOR_BGR2HLS_FULL);
    cv::cvtColor(v2, hls_v2, cv::COLOR_BGR2HLS_FULL);
    return hls_v1[1] < hls_v2[1];
    //return ((minc_v1 + maxc_v1) / 2.0) < ((minc_v2 + maxc_v2) / 2.0);
  };

  std::function<bool(const cv::Vec3b&, const cv::Vec3b&)> hue = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    cv::Vec3b hls_v1(v1);
    cv::Vec3b hls_v2(v2);
    cv::cvtColor(v1, hls_v1, cv::COLOR_BGR2HLS_FULL);
    cv::cvtColor(v2, hls_v2, cv::COLOR_BGR2HLS_FULL);
    return v1[0] < v2[0];
    //return get_hue(v1) < get_hue(v2);
  };

  std::function<bool(const cv::Vec3b&, const cv::Vec3b&)> saturation = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    cv::Vec3b hls_v1(v1);
    cv::Vec3b hls_v2(v2);
    cv::cvtColor(v1, hls_v1, cv::COLOR_BGR2HLS_FULL);
    cv::cvtColor(v2, hls_v2, cv::COLOR_BGR2HLS_FULL);
    return hls_v1[2] < hls_v2[2];
  };

  std::function<bool(const cv::Vec3b&, const cv::Vec3b&)> intensity = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    return (v1[0] + v1[1] + v1[2]) < (v2[0] + v2[1] + v2[2]);
  };

  std::function<bool(const cv::Vec3b&, const cv::Vec3b&)> minimum = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    return std::min(v1[0], v1[1], v1[2]) < std::min(v2[0], v2[1], v2[2]);
  };

  std::function<bool(const cv::Vec3b&, const cv::Vec3b&)> maximum = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    return std::max(v1[0], v1[1], v1[2]) < std::max(v2[0], v2[1], v2[2]);
  };

}