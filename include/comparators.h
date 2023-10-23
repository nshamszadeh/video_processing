#include <opencv2/core.hpp>
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

   inline float get_hue(const cv::Vec3b& v1) {
    auto minc_v1 = min3(v1[0], v1[1], v1[2]);
    auto maxc_v1 = max3(v1[0], v1[1], v1[2]);
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

  auto lightness = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    auto minc_v1 = min3(v1[0], v1[1], v1[2]);
    auto minc_v2 = min3(v2[0], v2[1], v2[2]);
    auto maxc_v1 = max3(v1[0], v1[1], v1[2]);
    auto maxc_v2 = max3(v2[0], v2[1], v2[2]);
    return ((minc_v1 + maxc_v1) / 2.0) < ((minc_v2 + maxc_v2) / 2.0);
  };

  auto hue = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    return get_hue(v1) < get_hue(v2);
  };

  auto saturation = [](const cv::Vec3b& v1, const cv::Vec3b& v2) {
    auto minc_v1 = min3(v1[0], v1[1], v1[2]);
    auto minc_v2 = min3(v2[0], v2[1], v2[2]);
    auto maxc_v1 = max3(v1[0], v1[1], v1[2]);
    auto maxc_v2 = max3(v2[0], v2[1], v2[2]);
    auto l_v1 = (minc_v1 + maxc_v1) / 2.0;
    auto v1_sat = 0.0;
    if (minc_v1 != maxc_v1) {
      if (l_v1 <= 0.5) v1_sat = (maxc_v1 - minc_v1) / (maxc_v1 + minc_v1);
      else v1_sat = (maxc_v1 - minc_v1) / (2.0 - maxc_v1 - minc_v1);
    }

    auto l_v2 = (minc_v2 + maxc_v2) / 2.0;
    auto v2_sat = 0.0;
    if (minc_v2 != maxc_v2) {
      if (l_v2 <= 0.5) v2_sat = (maxc_v2 - minc_v2) / (maxc_v2 + minc_v2);
      else v2_sat = (maxc_v2 - minc_v2) / (2.0 - maxc_v2 - minc_v2);
    }

    return v1_sat < v2_sat;
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