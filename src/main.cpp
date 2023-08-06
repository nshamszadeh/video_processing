#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "util.h"

#include <iostream>
#include <string>
#include <sstream>


//using namespace cv;

/*
Goal right now is to open a video file, play it, and write it to an avi file.
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
		cv::imshow("AHAHAHA", frame);
		output_vid.write(frame);
		cv::waitKey(30);
	} while (!frame.empty() );

	return 0;
}



