//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "util.h"
#include "comparators.h"
#include "sorter.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>


//using namespace cv;

/*
TODO: output a pixel-sorted video file
*/

int main() {
	std::string video_path = "/mnt/c/Users/xxtou/Videos/2022-12-28 13-48-08_Trim.mp4";
	cv::VideoCapture src(video_path);
	if (!src.isOpened()) {
		std::cout << "Could not open " << video_path << std::endl;
		return 1;
	}
	util::VideoParameters vp(src);
	cv::VideoWriter output_vid;
	output_vid.open(vp.name, vp.fourcc, vp.fps, vp.frameSize, vp.isColor);
	if (!output_vid.isOpened()) {
		std::cout << "Could not open " << vp.name << std::endl;
		return 1;
	}

	cv::Mat frame;

	do {
		src >> frame;

		cv::Mat test = cv::Mat::eye(4,4, CV_8UC3);
		test.row(0).at<cv::Vec3b>(0) = cv::Vec3b(255,0,0);
		//std::cout << static_cast<int>(test.at<uchar>(0,0)) << std::endl;
		test.at<cv::Vec3b>(0,1) = cv::Vec3b(34,1,2);
		test.at<cv::Vec3b>(0,2) = cv::Vec3b(5,4,3);
		std::cout << test.at<cv::Vec3b>(2) << std::endl;
		std::cout << std::endl << test << std::endl;
		cv::Mat mask = cv::Mat::ones(frame.size(), CV_8U);
		
		cv::Mat sorted_frame = pixelSort(frame, mask, comparator::lightness);
		output_vid.write(sorted_frame);
		cv::imshow("AHAHAHA", sorted_frame);
		cv::waitKey(30);
	} while (!frame.empty());

	return 0;
}
