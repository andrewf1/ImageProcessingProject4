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
void utility::cv_hist_stretch(Mat &src, Mat &tgt, const vector<roi>& regions, char* outfile) {
	// allocating the memory for the target/temp images
	Mat temp_img;
	cv_gray(src, temp_img);
	// cout << "copied src to temp_img" << endl;

	for (int r = 0; r < regions.size(); r++) {
		// cout << "in regions loop" << endl;
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;
		int a = regions.at(r).a;
		int b = regions.at(r).b;
		// cout << "sets regions var info" << endl;
		
		for (int i = 0; i < temp_img.rows; i++) {
			for (int j = 0; j < temp_img.cols; j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					int curr_pixel = temp_img.at<int>(i, j);
					// cout << "curr_pixel = " << curr_pixel << endl;
					if (curr_pixel < a) {
						// cout << "sets a" << endl;
						tgt.at<int>(i, j) = MINRGB;
					}
					else if (curr_pixel > b) {
						// cout << "sets b" << endl;
						tgt.at<int>(i, j) = MAXRGB;
					}
					else {
						// cout << "sets else" << endl;
						int newVal = (curr_pixel - a) * (255 / (b - a));
						tgt.at<int>(i, j) = checkValue(newVal);
					}
				}
				else {
					// cout << "sets not in roi" << endl;
					int newVal = temp_img.at<int>(i, j);
					tgt.at<int>(i, j) = checkValue(newVal);
				}
			}
		}
		// cout << "copies temp to tgt" << endl;
		temp_img = tgt.clone();
	}
}