#include <utility>
#include "fx.h"

namespace fx {
	cv::Mat pixelSortFrame(cv::Mat frame, cv::Mat mask, comparator::comparator comp, const bool rowWise) {
		// frame and mask must be same dimensions
    std::string mat_type;
    GET_MAT_TYPE(mask.type(), mat_type);
		TRACE("mask.type(): ", mat_type);
		CV_Assert((frame.rows == mask.rows) && (frame.cols == mask.cols));
		CV_Assert((frame.type() == CV_8UC3) && (mask.type() == CV_8UC1 || mask.type() == CV_8U));
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
	// TODO: make overloaded function where mask_path is not an argument, mask is just cv::Mat mask = cv::Mat::ones(frame.size(), CV_8UC1);
	void pixelSort(std::string src_path, std::string mask_path, comparator::comparator comp, const bool rowWise) {
		cv::VideoCapture src_vid(src_path);
		cv::VideoCapture mask_vid(mask_path);
    util::VideoParameters vp(src_path);
		cv::VideoWriter output_vid;
		output_vid.open(vp.name + "_modified.avi", cv::VideoWriter::fourcc('M','J','P','G'), vp.fps, vp.frameSize);
		const size_t frame_count = src_vid.get(cv::CAP_PROP_FRAME_COUNT);
		cv::Mat frame, mask;
		if (!output_vid.isOpened() || !src_vid.isOpened() || !mask_vid.isOpened()) {
			TRACE("One of the videos did not open.");
			return;
		}
		for (size_t i = 0; i < frame_count; i++) {
			mask_vid >> mask;
			if (i == 1) {TRACE("mask.type(): ", mask.type());}
      src_vid >> frame;
			cv::Mat sorted_frame = pixelSortFrame(frame, mask, comp, rowWise);
			IMSHOW("sorted frame: ", sorted_frame);
			output_vid.write(sorted_frame);
		}
		output_vid.release();
    src_vid.release();
	}
}