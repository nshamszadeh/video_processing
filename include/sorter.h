#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

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
template <typename F>
cv::Mat pixelSort(cv::Mat frame, cv::Mat mask, const F& comp, bool rowWise = false) {
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
  cv::rotate(sorted_frame, sorted_frame, cv::ROTATE_90_CLOCKWISE);
	return sorted_frame;
}
