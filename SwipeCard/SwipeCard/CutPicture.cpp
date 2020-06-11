#include "CutPicture.h"

using namespace std;
using namespace cv;

CutPicture::CutPicture()
{
}

CutPicture::~CutPicture()
{
}

bool CutPicture::CutPictureName(const char* fileName)
{
	Mat img = imread(fileName);

	Mat blurred = img.clone();
	blur(img, blurred, cv::Size(9, 9));
	threshold(blurred, blurred, 90, 255, cv::THRESH_BINARY);

	Mat gray = img.clone();
	cvtColor(blurred, gray, CV_RGB2GRAY);   //二值化

	Mat grayx = img.clone();
	Mat grayy = img.clone();
	Sobel(gray, grayx, CV_32F, 1, 0);
	Sobel(gray, grayy, CV_32F, 0, 1);
	subtract(grayx, grayy, gray);
	convertScaleAbs(gray, gray);

	Mat element = getStructuringElement(MORPH_RECT, cv::Size(25, 25));
	morphologyEx(gray, gray, MORPH_CLOSE, element);

	vector<vector<cv::Point> > contours;
	vector<Mat> f_contours;
	std::vector<cv::Point> approx2;
	//注意第5个参数为CV_RETR_EXTERNAL，只检索外框  
	findContours(gray.clone(), f_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //找轮廓

	sort(f_contours.begin(), f_contours.end(), [](const Mat &mat1, const Mat &mat2)
	{
		return contourArea(mat1) > contourArea(mat2);
	});

	RotatedRect rect = minAreaRect(f_contours[0]);
	contours.push_back(f_contours[0]);
	//drawContours(img, contours, -1, Scalar(255), 3);

	float width = rect.size.width;
	float height = rect.size.height;
	Mat srcPointsMat;
	boxPoints(rect, srcPointsMat);
	vector<Point2f> srcPoints = (vector<Point2f>)srcPointsMat.reshape(2, 4);

	//加入顺序 左上（原点）右上 右下 左下
	vector<Point2f> CanvasPoints;
	CanvasPoints.push_back(Point2f(0, 0));
	CanvasPoints.push_back(Point2f(width, 0));
	CanvasPoints.push_back(Point2f(width, height));
	CanvasPoints.push_back(Point2f(0, height));

	sort(srcPoints.begin(), srcPoints.end(), [](const Point2f &point1, const Point2f &point2)
	{
		return point1.x < point2.x;
	});

	if (srcPoints[1].y < srcPoints[0].y)
	{
		srcPoints = { srcPoints[0], srcPoints[1], srcPoints[3], srcPoints[2] };
	}
	else
	{
		srcPoints = { srcPoints[0], srcPoints[2], srcPoints[3], srcPoints[1] };
	}	

	/*for (int i = 0; i < 4; i++)
	{
		qDebug() << "src x : " << srcPoints[i].x << ", y: " << srcPoints[i].y;
	}
	for (int i = 0; i < 4; i++)
	{
		qDebug() << "dest x : " << CanvasPoints[i].x << ", y: " << CanvasPoints[i].y;
	}
	qDebug() << "width: " << width << " height" << height;*/
	
	Mat finalM = getPerspectiveTransform(srcPoints, CanvasPoints);
	warpPerspective(img, img, finalM, Size((int)width, (int)height));
	imwrite(fileName, img);
	return true;
}

float CutPicture::distance2Points(Point2f point1, Point2f point2)
{
	return sqrt((point1.x - point2.x)*(point1.x - point2.x) - (point1.y - point2.y)*(point1.y - point2.y));
}
