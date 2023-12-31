#include <string>
#include "fx.h"
#include "maskgen.h"

int main(int argc, char *argv[]) {
	std::string video_path = static_cast<std::string>(argv[1]);
	//cv::VideoWriter mask;
	//mask.open("mask.avi")
	//maskgen::bgSubtract(src, &mask);
	TRACE("Calling pixelSort()");
	fx::pixelSort(video_path, comparator::lightness());
	//src.release();
	return 0;
}
