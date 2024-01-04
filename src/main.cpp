#include <string>
#include "fx.h"

int main(int argc, char *argv[]) {
	std::string video_path = static_cast<std::string>(argv[1]);
	cv::VideoCapture src(video_path);
	if (!src.isOpened()) {
		TRACE("Could not open ", video_path);
		return 1;
	}
	TRACE("Calling pixelSort()");
	fx::pixelSort(src, comparator::lightness());
	src.release();
	return 0;
}
