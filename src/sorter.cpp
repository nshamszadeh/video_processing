#include "sorter.h"
// poop

namespace fx {
	template <typename F>
	cv::Mat pixelSortFrame(cv::Mat frame, cv::Mat mask, const F& comp, bool rowWise = false) {
		// frame and mask must be same dimensions
		CV_Assert((frame.rows == mask.rows) && (frame.cols == mask.cols));
		CV_Assert((frame.type() == CV_8UC3) && (mask.type() == CV_8UC1));
		//cv::Mat frame_hls(frame.rows, frame.cols, frame.type);
		//cv::cvtColor(frame, frame_hls, cv::COLOR_BGR2HLS_FULL);
		cv::Mat sorted_frame = cv::Mat(frame.size(), frame.type());
		if (!rowWise) {
			cv::rotate(frame, sorted_frame, cv::ROTATE_90_COUNTERCLOCKWISE);
			cv::rotate(mask, mask, cv::ROTATE_90_COUNTERCLOCKWISE);
		}
		else {
			frame.copyTo(sorted_frame);
		}
		// sort column-wise
		// find an interval, sort based on the interval, go to next interval
		constexpr int UNSET = -1;
		int start = UNSET;
		for (int i = 0; i < sorted_frame.rows; i++) {
			auto tmp_row = sorted_frame.row(i);
			auto mask_row = mask.row(i);
			for (int j = 0; j < tmp_row.cols; j++) {
				if (mask_row.at<uchar>(j) == 1 && start == UNSET) { // new interval has been reached
					start = j; // set interval starting point
				} else if ((mask_row.at<uchar>(j) == 0 || j == tmp_row.cols - 1) && start != UNSET) { // current interval has ended
					int end = j - 1; // set current interval ending point. j - 1 is safe because j == 0 implies start == UNSET
					std::sort(tmp_row.begin<cv::Vec3b>() + start, tmp_row.begin<cv::Vec3b>() + end, comp); // sort pixels
					// reset starting and ending points
					start = UNSET;
				}
			}
		}
		if (!rowWise) cv::rotate(sorted_frame, sorted_frame, cv::ROTATE_90_CLOCKWISE);
		return sorted_frame;
	}

	template <typename F>
	cv::VideoWriter pixelSort(cv::VideoCapture src_vid, const F& comp) {
		util::VideoParameters vp(src_vid);
		cv::VideoWriter output_vid;
		output_vid.open(vp.name, vp.fourcc, vp.fps, vp.frameSize, vp.isColor);

		cv::Mat frame;
		do {
			src_vid >> frame;
			cv::Mat mask = cv::Mat::ones(frame.size(), CV_8UC1);
			cv::Mat sorted_frame = pixelSortFrame(frame, mask, comp, true);
			output_vid.write(sorted_frame);
		} while (!frame.empty());

		return output_vid;
	}
}