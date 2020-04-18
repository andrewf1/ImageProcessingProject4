#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

using namespace cv;

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cvtColor(src, tgt, COLOR_BGR2GRAY);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	blur(src,tgt,Size(WindowSize,WindowSize));
}

/*-----------------------------------------------------------------------**/
void utility::cv_hist_stretch(Mat &src, Mat &tgt, const vector<roi>& regions) {
	Mat temp_img;
	cv_gray(src, temp_img);
	tgt = temp_img.clone();

	for (int r = 0; r < regions.size(); r++) {
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;
		int a = regions.at(r).a;
		int b = regions.at(r).b;
		
		for (int i = 0; i < temp_img.rows; i++) {
			for (int j = 0; j < temp_img.cols; j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					int curr_pixel = temp_img.at<uchar>(i, j);
					if (curr_pixel < a) {
						tgt.at<uchar>(i, j) = MINRGB;
					}
					else if (curr_pixel > b) {
						tgt.at<uchar>(i, j) = MAXRGB;
					}
					else {
						int newVal = (curr_pixel - a) * (255 / (b - a));
						tgt.at<uchar>(i, j) = checkValue(newVal);
					}
				}
				else {
					int newVal = temp_img.at<uchar>(i, j);
					tgt.at<uchar>(i, j) = checkValue(newVal);
				}
			}
		}
		tgt.copyTo(temp_img);
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_hist_eq(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions) {
	Mat temp_img;
	cv_gray(src, temp_img);
	tgt = temp_img.clone();

	Mat eq_tgt;
	eq_tgt = tgt.clone();
	equalizeHist(temp_img, eq_tgt);

	for (int r = 0; r < regions.size(); r++) {
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;
		
		for (int i = 0; i < temp_img.rows; i++) {
			for (int j = 0; j < temp_img.cols; j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					tgt.at<uchar>(i, j) = checkValue(eq_tgt.at<uchar>(i, j));
				}
				else {
					tgt.at<uchar>(i, j) = checkValue(temp_img.at<uchar>(i, j));
				}
			}
		}
		tgt.copyTo(temp_img);
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_canny_edge(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions) {
	Mat temp_img;
	cv_gray(src, temp_img);
	tgt = temp_img.clone();

	for (int r = 0; r < regions.size(); r++) {
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;
		int T1 = regions.at(r).canny_T1;
		int T2 = regions.at(r).canny_T2;

		// creating canny edge detected image using parameters for ROI
		Mat canny_tgt;
		canny_tgt = tgt.clone();
		Canny(temp_img, canny_tgt, T1, T2);

		for (int i = 0; i < temp_img.rows; i++) {
			for (int j = 0; j < temp_img.cols; j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)					
				) {
					tgt.at<uchar>(i, j) = checkValue(canny_tgt.at<uchar>(i, j));
				}
				else {
					tgt.at<uchar>(i, j) = checkValue(temp_img.at<uchar>(i, j));
				}
			}
		}
		tgt.copyTo(temp_img);
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_sobel_edge(cv::Mat &src, cv::Mat &tgt, const vector<roi>& regions) {
	Mat temp_img;
	cv_gray(src, temp_img);
	tgt = temp_img.clone();
	int ddepth = -1; //CV_16S

	Mat sobel_tgt;
	sobel_tgt = tgt.clone();
	Sobel(temp_img, sobel_tgt, ddepth, 1, 1);

	for (int r = 0; r < regions.size(); r++) {
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;

		for (int i = 0; i < temp_img.rows; i++) {
			for (int j = 0; j < temp_img.cols; j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)					
				) {
					tgt.at<uchar>(i, j) = checkValue(sobel_tgt.at<uchar>(i, j));
				}
				else {
					tgt.at<uchar>(i, j) = checkValue(temp_img.at<uchar>(i, j));
				}				
			}
		}
		tgt.copyTo(temp_img);
	}
}