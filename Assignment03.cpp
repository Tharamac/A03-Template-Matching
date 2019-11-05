#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//! [declare]
/// Global Variables
bool use_mask;


Mat img_list[10];
Mat templ_list[3];
Mat img;
Mat templ;
Mat mask; 
Mat result;
const char* img_dir[] = {
	"samples\\img1.jpg",
	"samples\\img2.jpg",
	"samples\\img3.jpg",
	"samples\\img4.jpg",	
	"samples\\img5.jpg",
	"samples\\img6.jpg",
	"samples\\img7.jpg",
	"samples\\img8.jpg",
	"samples\\img9.jpg",
	"samples\\img10.jpg",
};
const char* templ_dir[] = {
	"samples\\pen1.png",
	"samples\\pen2.png",
	"samples\\pen3.png",
	"samples\\pen4.png",
};
const char* image_window = "Source Image";
const char* templ_window = "Template";
const char* result_window = "Result window";

int match_method;
int image_no;
int templ_no;
int max_method = 5;
const int img_amount = 10;
const int templ_amount = 4;

//! [declare]

/// Function Headers
void MatchingMethod(int, void*);

/**
 * @function main
 */
int main(int argc, char** argv)
{


	//! [load_image]
	/// Load image and template
	for (int i = 0; i < img_amount; i++) {
		img_list[i] = imread(img_dir[i], IMREAD_COLOR);
		if(i == 8)
			resize(img_list[i], img_list[i], Size(0, 0), 0.75, 0.75);
		resize(img_list[i], img_list[i],Size(0,0),0.75,0.75);
	
	
	}
	
	for (int i = 0; i < templ_amount; i++) {
		templ_list[i] = imread(templ_dir[i], IMREAD_COLOR);
		resize(templ_list[i], templ_list[i], Size(0, 0), 0.7, 0.7);
	}

	//use_mask = true;
	//mask = Mat::zeros(img.rows,img.cols,CV_8U);
	//rectangle(img, Point(500, 64), Point(750,105), Scalar(255), CV_FILLED);
	//rectangle(mask, Point(100,28), Point(1000,800), Scalar(255), CV_FILLED);


//	imshow("1. Source img", img);
//	imshow("2. Template", templ);
	//imshow("3. mask", mask);
	
	//waitKey(0);

	

	
	//! [load_image]

	//! [create_windows]
	/// Create windows
	namedWindow(image_window, WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);
	namedWindow(templ_window, WINDOW_AUTOSIZE);
	//! [create_windows]

	//! [create_trackbar]
	/// Create Trackbar
	const char* img_label = "Image";
	const char* method_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	const char* templ_label = "Template";

	createTrackbar(img_label, image_window, &image_no, img_amount-1, MatchingMethod);
	createTrackbar(templ_label, image_window, &templ_no, templ_amount-1, MatchingMethod);
	createTrackbar(method_label, image_window, &match_method, max_method, MatchingMethod);
	//! [create_trackbar]

	MatchingMethod(0, 0);

	//! [wait_key]
	waitKey(0);
	return 0;
	//! [wait_key]
}



/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(int, void*)

{
	img = img_list[image_no];
	templ = templ_list[templ_no];

	//! [copy_source]
	/// Source image to display
	Mat img_display;
	img.copyTo(img_display);
	//! [copy_source]

	//! [create_result_matrix]
	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);
	//! [create_result_matrix]

	//! [match_template]
	/// Do the Matching and Normalize
	bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
	if (use_mask && method_accepts_mask)
	{
		cout << "1" << endl;
		matchTemplate(img, templ, result, match_method, mask);
	}
	else
	{
		cout << "2" << endl;
		matchTemplate(img, templ, result, match_method);
	}
	//! [match_template]

	//! [normalize]
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	//! [normalize]

	//! [best_match]
	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	//! [best_match]

	//! [match_loc]
	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	//! [match_loc]

	//! [imshow]
	/// Show me what you got
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	imshow(templ_window, templ);
	imshow(image_window, img_display);
	imshow(result_window, result);
	//! [imshow]

	return;
}