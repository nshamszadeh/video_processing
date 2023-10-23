#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor
#include <functional> // std::function
#include <algorithm> // std::min
#include <cmath> // std::fmod

// recall opencv image pixels are stored in BGR format by default
// TODO: Make BGR -> HLS conversion happen only once per image Mat (regardless of number of sorts)
namespace comparator {

  template <typename T>
  inline T min3(T& a, T&b, T&c) {
    T temp = (a < b) ? a : b;
    return (temp < c) ? temp : c;
  }

template <typename T>
  inline T max3(T& a, T&b, T&c) {
    T temp = (a > b) ? a : b;
    return (temp > c) ? temp : c;
  }

  auto lightness = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
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

  auto hue = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    cv::Vec3b hls_v1(v1);
    cv::Vec3b hls_v2(v2);
    cv::cvtColor(v1, hls_v1, cv::COLOR_BGR2HLS_FULL);
    cv::cvtColor(v2, hls_v2, cv::COLOR_BGR2HLS_FULL);
    return v1[0] < v2[0];
    //return get_hue(v1) < get_hue(v2);
  };

  auto saturation = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    cv::Vec3b hls_v1(v1);
    cv::Vec3b hls_v2(v2);
    cv::cvtColor(v1, hls_v1, cv::COLOR_BGR2HLS_FULL);
    cv::cvtColor(v2, hls_v2, cv::COLOR_BGR2HLS_FULL);
    return hls_v1[2] < hls_v2[2];
  };

  auto intensity = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    return (v1[0] + v1[1] + v1[2]) < (v2[0] + v2[1] + v2[2]);
  };

  auto minimum = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    return min3(v1[0], v1[1], v1[2]) < min3(v2[0], v2[1], v2[2]);
  };

  auto maximum = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    return max3(v1[0], v1[1], v1[2]) < max3(v2[0], v2[1], v2[2]);
  };

}