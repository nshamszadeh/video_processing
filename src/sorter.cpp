#include <utility>
#include "fx.h"

namespace fx {
	cv::Mat pixelSortFrame(cv::Mat frame, cv::Mat mask, comparator::comparator comp, const bool rowWise) {
		// frame and mask must be same dimensions
		CV_Assert((frame.rows == mask.rows) && (frame.cols == mask.cols));
		CV_Assert((frame.type() == CV_8UC3) && (mask.type() == CV_8UC1));
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

	void pixelSort(cv::VideoCapture src_vid, comparator::comparator comp, const bool rowWise) {
		util::VideoParameters vp(src_vid);
		cv::VideoWriter output_vid;
		output_vid.open(vp.name, cv::VideoWriter::fourcc('M','J','P','G'), vp.fps, vp.frameSize);
		size_t frame_count = src_vid.get(cv::CAP_PROP_FRAME_COUNT);
		cv::Mat frame;
		src_vid >> frame;
		for (size_t i = 0; i < frame_count; i++) {
			if (!output_vid.isOpened()) {
				TRACE("Something went wrong here.");
				return;
			}
			cv::Mat mask = cv::Mat::ones(frame.size(), CV_8UC1);
			cv::Mat sorted_frame = pixelSortFrame(frame, mask, comp, rowWise);
			IMSHOW("sorted frame: ", sorted_frame);
			output_vid.write(sorted_frame);
			src_vid >> frame;
		}
		output_vid.release();
	}
}