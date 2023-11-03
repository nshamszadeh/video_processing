#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#ifdef _DEBUG
#include <opencv2/highgui.hpp> // cv::waitKey()
#endif
#include "util.h"
#include "comparators.h"

namespace fx {
	/**
	 * @brief Sorts pixels of frame using intervals from mask.
	 * Sorting is based on the sorting method passed by sorter.
	 * For now this is going to sort row-wise only.
	 * 
	 * 
	 * 	@param frame - image to apply pixel sorting to
	 * 	@param mask  - Binary image detailing the intervals to sort ()
	 * 									Intervals are set to 1 and separated by 0
	 *  @param comp - lambda function defining comparison function for sorting
	 *                  defined in include/comparators.h
	 *  @return frame of sorted pixels
	 */
	template <typename F>
	cv::Mat pixelSortFrame(cv::Mat frame, cv::Mat mask, const F& comp, bool rowWise = false);

	/**
	 * @brief Sorts pixels of each frame of src_vid and returns a video of pixel-sorted frames
	 * 
	 * @param src_vid - video to pixel-sort
	 * @param comp - lambda function defining pixel comparison function for sorting
	 * @return cv::VideoWriter video whose frames are pixel-sorted
	 */
	template <typename F>
	cv::VideoWriter pixelSort(cv::VideoCapture src_vid, const F& comp);
}