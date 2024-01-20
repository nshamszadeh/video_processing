#include <string>
#include "fx.h"
#include "maskgen.h"

int main(int argc, char *argv[]) {
	std::string video_path = static_cast<std::string>(argv[1]);
	std::string mask_path;
	if (argc > 2) mask_path = argv[2];
	else mask_path = maskgen::bgSubtract(video_path);
	//std::string mask_path = "/home/navid/Documents/video_processing/input3_mask.avi";
	fx::pixelSort(video_path, mask_path, comparator::maximum());
	return 0;
}
