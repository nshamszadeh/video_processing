#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
#include <sstream>

#define INT_TO_CHAR(fourcc)                         \
		{(char)(fourcc & 0XFF) ,                    \
		 (char)((fourcc & 0XFF00) >> 8),            \
		 (char)((fourcc & 0XFF0000) >> 16),         \
		 (char)((fourcc & 0XFF000000) >> 24), 0};


//using namespace cv;

struct VideoParameters {
	std::string name;
	int fourcc;
	double fps;
	cv::Size frameSize;
	bool isColor = true;

	VideoParameters(std::string _name, int _fourcc, double _fps, cv::size _frameSize, bool _isColor = true)
	: name{name}, fourcc{_fourcc}, fps(_fps), frameSize{_frameSize}, isColor{_isColor} {}
};

VideoParameters getVideoParams(std::string video_path);

/*
Goal right now is to open a video file, play it, and write it to an avi file.
*/

int main() {
	std::string video_path = "/home/spacebar/Videos/vids/coyote.mp4";
	

	do {
		src >> frame;
		cv::imshow("AHAHAHA", frame);
		cv::waitKey(30);
	} while (!frame.empty() );

	return 0;
}


VideoParameters getVideoParams(std::string video_path) {
	cv::VideoCapture src;
	VideoParameters vp;
	int codec = src.get(CAP_PROP_FOURCC);
	double fps = 30.0; // framerate of the created video stream
	std::string output_name = "coyote2.avi";
	
	src.open(video_path);
	if (!src.isOpened()) {
		std::cout << "Could not open video " << video_path << std::endl;
		return vp;
	}

	// get one frame to know file size and type
	cv::Mat frame;
	src >> frame;
	// check if we succeeded
	if  (frame.empty()) {
		std::cerr << "AYO BADABING BADABOOM\n";
	}

}
