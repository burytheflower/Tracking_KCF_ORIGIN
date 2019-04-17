#include <opencv2/opencv.hpp>
#include "TrackerKCF/trackerKCF.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
	cv::VideoCapture capture;
	capture.open("D:/Res/Videos/StaticBG_vtest.avi");
//	capture.open("D:/Res/Videos/DynamicBG_2.mp4");
//	capture.open("D:/Res/Videos/DynamicBG_car_1.avi");
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}
	double width = capture.get(cv::CAP_PROP_FRAME_WIDTH);
	double height = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	cout << "Size: " << width << " * " << height << endl;

	cv::Rect2d object;
	cv::Rect ROI, boundingRect;
	//float peak_value;
	cv::Mat frame, frame_copy;
	shared_ptr<custom_kcf::TrackerKCF> tracker = custom_kcf::TrackerKCF::create(true, false, true, false, 0.30);

	capture.read(frame);
	ROI = cv::selectROI(frame);
	tracker->init(ROI, frame);

	int frameCount = 0;
	int64 t1, t2;
	double timeCost;
	int countForCost = 0;
	double sumCost_KCF = 0.0f;

	while (capture.read(frame)) {
		//cv::GaussianBlur(frame, frame, cv::Size(3, 3), 3, 3);

		t1 = cv::getTickCount();
		bool detected = tracker->update(frame, boundingRect);
		//cout << "peak_value = " << peak_value << endl;
		if (detected)
			cv::rectangle(frame, boundingRect, cv::Scalar(0, 255, 0));
		else
			cv::rectangle(frame, boundingRect, cv::Scalar(0, 0, 255));

		t2 = cv::getTickCount();

		if (frameCount % 10 == 0) {
			countForCost++;
			timeCost = (t2-t1)*1000.0f/cv::getTickFrequency();
			sumCost_KCF += timeCost;
			cout << "KCF cost: " << timeCost << " ms" << endl;
		}

		cv::imshow("KCF Objects Tracking", frame);

		char c = cv::waitKey(30);
		if (c == 27) {
			break;
		}

	}

	cout << "Average Cost:" << endl;
	cout << "\tKCF:" << sumCost_KCF/countForCost << "ms" << endl;

	capture.release();
//	cv::waitKey(0);
	return 0;
}


/*

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TrackerKCF.hpp"

#include <dirent.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){

	if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;

	for(int i = 0; i < argc; i++){
		if ( strcmp (argv[i], "hog") == 0 )
			HOG = true;
		if ( strcmp (argv[i], "fixed_window") == 0 )
			FIXEDWINDOW = true;
		if ( strcmp (argv[i], "singlescale") == 0 )
			MULTISCALE = false;
		if ( strcmp (argv[i], "show") == 0 )
			SILENT = false;
		if ( strcmp (argv[i], "lab") == 0 ){
			LAB = true;
			HOG = true;
		}
		if ( strcmp (argv[i], "gray") == 0 )
			HOG = false;
	}

	// Create TrackerKCF object
	TrackerKCF tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Frame readed
	Mat frame;

	// Tracker results
	Rect result;

	// Path to list.txt
	ifstream listFile;
	string fileName = "images.txt";
	listFile.open(fileName);

	// Read groundtruth for the 1st frame
	ifstream groundtruthFile;
	string groundtruth = "region.txt";
	groundtruthFile.open(groundtruth);
	string firstLine;
	getline(groundtruthFile, firstLine);
	groundtruthFile.close();

	istringstream ss(firstLine);

	// Read groundtruth like a dumb
	float x1, y1, x2, y2, x3, y3, x4, y4;
	char ch;
	ss >> x1;
	ss >> ch;
	ss >> y1;
	ss >> ch;
	ss >> x2;
	ss >> ch;
	ss >> y2;
	ss >> ch;
	ss >> x3;
	ss >> ch;
	ss >> y3;
	ss >> ch;
	ss >> x4;
	ss >> ch;
	ss >> y4;

	// Using min and max of X and Y for groundtruth rectangle
	float xMin =  min(x1, min(x2, min(x3, x4)));
	float yMin =  min(y1, min(y2, min(y3, y4)));
	float width = max(x1, max(x2, max(x3, x4))) - xMin;
	float height = max(y1, max(y2, max(y3, y4))) - yMin;


	// Read Images
	ifstream listFramesFile;
	string listFrames = "images.txt";
	listFramesFile.open(listFrames);
	string frameName;


	// Write Results
	ofstream resultsFile;
	string resultsPath = "output.txt";
	resultsFile.open(resultsPath);

	// Frame counter
	int nFrames = 0;


	while ( getline(listFramesFile, frameName) ){
		frameName = frameName;

		// Read each frame from the list
		frame = imread(frameName, CV_LOAD_IMAGE_COLOR);

		// First frame, give the groundtruth to the tracker
		if (nFrames == 0) {
			tracker.init( Rect(xMin, yMin, width, height), frame );
			rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );
			resultsFile << xMin << "," << yMin << "," << width << "," << height << endl;
		}
		// Update
		else{
			result = tracker.update(frame);
			rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 1, 8 );
			resultsFile << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
		}

		nFrames++;

		if (!SILENT){
			imshow("Image", frame);
			waitKey(1);
		}
	}
	resultsFile.close();

	listFile.close();

}

*/
