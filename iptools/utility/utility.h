#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include "../../project/roi.hpp"
#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void cv_gray(cv::Mat &src, cv::Mat &tgt);
		static void cv_avgblur(cv::Mat &src, cv::Mat &tgt, int WindowSize);
		static void cv_hist_stretch(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions);
		static void cv_hist_eq(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions);
		static void cv_canny_edge(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions);
		static void cv_sobel_edge(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions);
		static void cv_comb_ops(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions);
};

#endif