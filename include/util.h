#include <iostream>
#include <string>

#include <opencv2/videoio.hpp>

#define INT_TO_CHAR(fourcc)                         \
		{(char)(fourcc & 0XFF) ,                    \
		 (char)((fourcc & 0XFF00) >> 8),            \
		 (char)((fourcc & 0XFF0000) >> 16),         \
		 (char)((fourcc & 0XFF000000) >> 24), 0};

namespace util {
	struct VideoParameters {
		std::string name;
		int fourcc;
		double fps;
		cv::Size frameSize;
		bool isColor = true;

		VideoParameters(std::string _name, int _fourcc, double _fps, cv::size _frameSize, bool _isColor = true)
										: name{name}, fourcc{_fourcc}, fps(_fps), frameSize{_frameSize}, isColor{_isColor} {}
		VideoParameters(std::string filename);
		VideoParameters(cv::VideoCapture src);
  };

	


}
