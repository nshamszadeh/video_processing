#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "util.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>


//using namespace cv;

/*
TODO: output a pixel-sorted video file
*/

/**
 * Sorts pixels of frame using intervals from mask.
 * Sorting is based on the sorting method passed by sorter.
 * For now this is going to sort row-wise only.
 * 
 * Inputs:
 * 	cv::Mat frame - image to apply pixel sorting to
 * 	cv::Mat mask  - Binary image detailing the intervals to sort ()
 * 									Intervals are set to 1 and separated by 0
 */
cv::Mat pixelSort(cv::Mat frame, cv::Mat mask, std::function<bool(const cv::Vec3b&, const cv::Vec3b&)> comp, bool rowWise = false) {
	// frame and mask must be same dimensions
	CV_Assert((frame.rows == mask.rows) && (frame.cols == mask.cols));
	CV_Assert((frame.type() == CV_8UC3) && (mask.type() == CV_8UC1));

	cv::Mat tmp = cv::Mat(frame.size(), frame.type());
	if (!rowWise)
		cv::rotate(frame, tmp, cv::ROTATE_90_COUNTERCLOCKWISE);
	else
		frame.copyTo(tmp);
	// sort column-wise
	// find an interval, sort based on the interval, go to next interval
	size_t start = -1;
	for (int i = 0; i < tmp.rows; i++) {
		auto row = tmp.row(i);
		for (cv::MatIterator_<cv::Vec3b> it = row.begin<cv::Vec3b>(); it != row.end<cv::Vec3b>(); it++) {

		}

	}
	return tmp;
}

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
		//cv::imshow("AHAHAHA", frame);
		//output_vid.write(frame);
		//cv::waitKey(30);
	} while (/*!frame.empty()*/ 0 );

	return 0;
}



