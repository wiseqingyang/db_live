#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <algorithm>
#include <QDebug>


class CutPicture
{
public:
	CutPicture();
	~CutPicture();

	bool CutPictureName(const char* fileName);
	float distance2Points(cv::Point2f point1, cv::Point2f point2);
};

