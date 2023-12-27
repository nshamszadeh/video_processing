#include <string>
#include "fx.h"

int main() {
	std::string video_path = "/mnt/c/Users/xxtou/Downloads/input2.mp4";
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
