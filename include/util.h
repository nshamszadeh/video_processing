#include <string>
#include <functional>
#include <algorithm>
#include <opencv2/videoio.hpp>

// debug helpers (may put these in another header later)
#ifdef _DEBUG
#include <iostream>
#include <string_view>
#include <source_location>
#include<opencv2/highgui.hpp>
template <typename ...Args>
void trace(const std::source_location& loc, Args&& ...args) {
	std::ostringstream stream;
	stream << loc.file_name() << "\t" << loc.function_name() << ":" << loc.line() << "\t";
	(stream << ... << std::forward<Args>(args)) << '\n';
	std::cout << stream.str();
}
#define IMSHOW(name, frame) do { \
		cv::imshow(name, frame); \
		cv::waitKey(1); \
	} while(0)
#define TRACE(...) do { trace(std::source_location::current(), __VA_ARGS__); } while(0)
#else
#define TRACE(...)
#define IMSHOW(name, frame)
#endif
#pragma once

namespace util {
	struct VideoParameters {
		std::string name;
		int fourcc;
		double fps;
		cv::Size frameSize;
		bool isColor = true;

		VideoParameters(std::string _name, int _fourcc, double _fps, cv::Size _frameSize, bool _isColor = true)
										: name{name}, fourcc{_fourcc}, fps{_fps}, frameSize{_frameSize}, isColor{_isColor} {}
		VideoParameters(std::string filename);
		VideoParameters(cv::VideoCapture src);
  };
}
