#include <string>
#include <functional>
#include <algorithm>
#ifdef _DEBUG
#include <iostream>
#include <string_view>
#include <source_location>
#define TRACE(...) trace(std::source_location::current(), __VA_ARGS__)
#endif
#include <opencv2/videoio.hpp>

namespace util {
#ifdef _DEBUG
	template <typename ...Args>
	void trace(const std::source_location& loc, Args&& ...args) {
		std::ostringstream stream;
		stream << loc.file_name() << ":" << loc.function() << ":" << loc.line() << ": ";
		(stream << ... << std::forward<Args>(args)) << '\n';
		std::cout << stream.str();
	}
#define TRACE(...) trace(__VA_ARGS__)
#else
#define TRACE(...)
#endif
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
