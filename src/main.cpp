#include <string>
#include "fx.h"
#include "maskgen.h"

int main(int argc, char *argv[]) {
	std::string video_path = static_cast<std::string>(argv[1]);
	cv::VideoWriter mask = maskgen::bgSubtract(video_path);
	fx::pixelSort(video_path, comparator::lightness());
	return 0;
}
