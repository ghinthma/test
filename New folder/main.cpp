#include "headers.h"
#include "Utilities.h"


using namespace std;
using namespace cv;

void allData(Mat in);
void backProjection(Mat sample, Mat image);
void colourReduce(Mat input, int div = 2);

int main() {

	

	///create image of single colour
	//Mat image02(200, 200, 16, Scalar(0,0,255));
	//imshow("RED", image02); cvWaitKey();
	////cout << image.data << endl;

	Mat sample_green = imread("sample_green.png", 1);
	imshow("sample_green", sample_green); waitKey();
	if (sample_green.empty()) cout << "ERROR LOADING sample_green" << endl;

	Mat cam01 = imread("CamVidLights01.png", 1);
	if (cam01.empty()) cout << "ERROR LOADING CAMVID01" << endl;

	///colour reduction before sending to backProjection()
	colourReduce(cam01);
	imshow("colour_reduced", cam01); waitKey();

	///implement back projection on cam01 using sample_green
	backProjection(sample_green, cam01);

	return 0;
}

void allData(Mat input_image) {
	cout << "		... image data ..." << endl;
	cout << "image.size() -> " << input_image.size << endl;
	cout << "rows         -> " << input_image.rows << endl;
	cout << "cols         -> " << input_image.cols << endl;
	cout << "channels     -> " << input_image.channels() << endl;
	cout << "data type    -> " << input_image.type() << endl;
	
}


void backProjection(Mat sample_colour, Mat src_image) {
	int channels[] = { 0, 1 }; // Hue = channels[0], Sat = channels[1]
	Mat hist;
	int hue_bins = 10, sat_bins = 10;
	int hist_size[] = { hue_bins, sat_bins };
	float h_range[] = { 0 , 180 };
	float s_range[] = { 0, 255 };
	const float* ranges[] = { h_range, s_range };

	Mat target_image_hsv;
	cvtColor(src_image, target_image_hsv, CV_BGR2HSV);
	imshow("target_hsv", target_image_hsv); waitKey();

	Mat sample_hsv;
	cvtColor(sample_colour, sample_hsv, CV_BGR2HSV);
	calcHist(&sample_hsv, 1, channels, Mat(), hist, 2, hist_size, ranges);
	normalize(hist, hist, 0, 255, NORM_MINMAX);
	
	Mat back_projection_image;
	calcBackProject(&target_image_hsv, 1, channels, hist, back_projection_image, ranges, 1, true);
	imshow("Back Projection", back_projection_image); waitKey();
	threshold(back_projection_image,back_projection_image, 230, 255, CV_THRESH_BINARY);
	imshow("Back Projection Thrresholded", back_projection_image); waitKey();
}

void colourReduce(Mat in, int div) {
	int num_lines = in.rows;
	int elements_per_line = in.cols * in.channels();

	for (int j = 0; j < num_lines; j++) {
		//get address of row j
		uchar* data = in.ptr<uchar>(j);
		
		for (int i = 0; i < elements_per_line; i++) {
			data[i] = data[i] / div * div + div / 2;
		}// end of particualr line
	}
}


