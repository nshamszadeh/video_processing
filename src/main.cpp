#include <iostream>
#include <string>
//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "comparators.h"
#include "sorter.h"

//using namespace cv;

int main() {
	std::string video_path = "/mnt/c/Users/xxtou/Downloads/input.mp4";
	cv::VideoCapture src(video_path);
	if (!src.isOpened()) {
		std::cout << "Could not open " << video_path << std::endl;
		return 1;
	}
	cv::VideoWriter pixelSortedVid = fx::pixelSort(src);
	pixelSortedVid.release();
	return 0;
}
