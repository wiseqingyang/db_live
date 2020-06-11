#include "CutAndRecgnize.h"
#include "QPoint"
#include "QDebug"
#include "QSize"
#include "QPixmap"
#include "QJsonObject"
#include "FunctionDef.h"
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "UserConfig.h"
#include "QTime"

using namespace cv;

CutAndRecgnize::CutAndRecgnize()
{
	connect(&mRecognizeThread, &RecognizeThread::onRecognized, this, &CutAndRecgnize::onRecognizedCode);
	connect(&mRecognizeThread, &RecognizeThread::updatePic, this, &CutAndRecgnize::updatePicture);
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

}


CutAndRecgnize::~CutAndRecgnize()
{
}

void CutAndRecgnize::setImagePath(QString imagePath)
{
	mImagePath = imagePath;
	mRecognizeThread.setImagePath(imagePath);
}

void CutAndRecgnize::setTicketSizePos(QJsonArray sizeAndPos)
{
	mSizeAndPos = sizeAndPos;
	mRecognizeThread.setTicketSizePos(sizeAndPos);
}

void CutAndRecgnize::recognizeCode()
{
	mRecognizeThread.start();
}

void CutAndRecgnize::onRecognizedCode(QString result, QString imagePath)
{
	emit onRecognizedTicketCode(result, imagePath);
}

void CutAndRecgnize::updatePicture(QString picType, QString imageName)
{
	emit updatePic(picType, imageName);
}

void CutAndRecgnize::onlyCutPic()
{
	QString fileName = mImagePath;
	QString newFileName = "";

	qDebug() << "mSizeAndPos:::   " << mSizeAndPos;
	//QSize sizeTab[] = { QSize(875, 440), QSize(875, 550), QSize(875, 875), QSize(875, 1300), QSize(875, 1740), QSize(875, 2185) };
	QPoint originPoint = QPoint(1934, 2592);
	QSize descSize = QSize(mSizeAndPos.at(0).toInt(), mSizeAndPos.at(1).toInt());

	QPixmap pic = QPixmap(fileName);
	QPixmap descPic = pic.copy(QRect(QPoint(originPoint.x() - descSize.width(), originPoint.y() - descSize.height()), descSize));

	newFileName = fileName.replace(".jpg", "_upload.jpg");
	descPic.save(newFileName, "JPG");
	emit updatePic("savePic", newFileName);
}

QString CutAndRecgnize::decodeImageBottom(QImage* originImg, int posIndex)
{
	qDebug() << "recongnize tic " << originImg << " index " << posIndex;
	enum DecodeResult
	{
		ONE_29_31,		//识别出29位的最终结果
		DOUBlE_18_11,	//识别出18位和11位两个结果
		HALF_18,		//识别出一个18位结果
		HALF_11,		//识别出一个11位结果
		NONE			//没有结果

	};

	DecodeResult resultState = NONE;
	float margins = 15;
	QImage img = originImg->copy(QRect(margins, margins, originImg->width() - margins * 2, originImg->height() - margins * 2));
	int w = img.width();
	int h = img.height();
	img.save("11.jpg");

	float discernW = 160;
	float discernH = discernW;

	//从左上角顺时针
	QRect rect[] = {
		QRect(0, 0, discernW, discernH),
		QRect(w - discernW, 0, discernW, discernH),
		QRect(w - discernW, h - discernH, discernW, discernH),
		QRect(0, h - discernH, discernW, discernH),
	};

	QImage m = img.copy(rect[posIndex]);
	QImage *m1 = new QImage(m);
	m1 = lightness(100, m1);
	//m_Zxing.setDecoder(DecoderFormat_DATA_MATRIX);
	QString info = m_Zxing.decodeImage(*m1);
	sRectInfo rinfo;
	rinfo.rect = rect[posIndex];
	rinfo.strCode = info;
	rinfo.is_exist = (info.length() > 1) ? true : false;
	rinfo.index = posIndex;

	if (rinfo.is_exist)
	{
		m_RectInfo.push_back(rinfo);
	}
	m.save("qrcode.jpg");
	emit updatePic("qrcodePic", "qrcode.jpg");
	delete m1;
	//m.save(name);

	for (int i = 0; i < m_RectInfo.size(); i++)
	{
		QString code = m_RectInfo.at(i).strCode;
		qDebug() << i << "index strCode is " << code << code.length();
		if (code.length() == 29 || code.length() == 31)
		{
			resultState = ONE_29_31;
			m_RectInfo.clear();
			return code;
		}
		else if (code.length() == 18)
		{
			resultState = HALF_18;
		}
	}

	if (resultState == HALF_18)
	{
		QRect rr[2];
		sRectInfo rinfo = m_RectInfo[0];

		QRect tmpRect = rinfo.rect;

		if (tmpRect.x() == 0)
		{
			rr[0] = QRect(tmpRect.x() + tmpRect.width()*0.8, tmpRect.y(), tmpRect.width(), tmpRect.height());
		}
		else
		{
			rr[0] = QRect(tmpRect.x() - tmpRect.width()*0.8, tmpRect.y(), tmpRect.width(), tmpRect.height());
		}

		for (size_t i = 0; i < 1; i++)
		{
			QImage m = img.copy(rr[i]);
			QImage *m1 = new QImage(m);
			m1 = lightness(100, m1);
			QString info = m_Zxing.decodeImage(*m1);
			sRectInfo rinfo;
			rinfo.rect = rect[i];
			rinfo.strCode = info;
			rinfo.is_exist = (info.length() > 1) ? true : false;
			rinfo.index = i;

			if (rinfo.is_exist)
			{
				m_RectInfo.push_back(rinfo);
			}
			m.save("qrcode1.jpg");
			emit updatePic("qrcodePic1", "qrcode1.jpg");
			delete m1;
			break;

			qDebug() << info; //<< " index : " << QString::number(i);
			delete m1;
		}
	}
	QString returnStr = "";
	if (m_RectInfo.size() == 2)
	{
		//前面的一位肯定是18位的
		returnStr = QString("%1%2").arg(m_RectInfo.at(0).strCode).arg(m_RectInfo.at(1).strCode);
	}
	m_RectInfo.clear();
	return returnStr;
}

void CutAndRecgnize::addWaterMark(QString imagePath, bool isPortrait)
{
	Mat logo = imread("waterMark.png", CV_LOAD_IMAGE_UNCHANGED);//第二个参数带透明读取
	int w = 390;
	int h = 170;

	if (isPortrait)
	{
		int tmp = w;
		w = h;
		h = tmp;
		//旋转90度.
		flip(logo, logo, 1);
		transpose(logo, logo);
	}

	int margins = 0;
	cv::String path = imagePath.toStdString().c_str();
	Mat image1 = imread(path);
	qDebug() << "=================== add watermark";
	Rect rects[] = {
		Rect(margins, margins, w, h),
		Rect(image1.cols - w - margins, margins, w, h),
		Rect(image1.cols - w - margins, image1.rows - h, w, h),
		Rect(margins, image1.rows - h - margins, w, h),
	};
	Mat imageROI = image1(rects[mSizeAndPos.at(2).toInt()]);
	qDebug() << "----------getROI";
	std::vector<Mat> src_channels;
	std::vector<Mat> desc_channels;
	split(logo, src_channels);
	split(imageROI, desc_channels);
	qDebug() << "----------getROI->split";
	CV_Assert(src_channels.size() == 4 && desc_channels.size() == 3);

	qDebug() << "----------getROI->split->assert";
	for (int i = 0; i < 3; i++)
	{
		desc_channels[i] = desc_channels[i].mul(255.0 - src_channels[3], 1 / 255.0);
		desc_channels[i] += src_channels[i].mul(src_channels[3], 1.0 / 255.0);
	}
	qDebug() << "----------getROI->split->assert->mul";
	merge(desc_channels, imageROI);
	/*Mat mask = logo.clone();
	logo.copyTo(imageROI, mask);*/
	qDebug() << "----------getROI->split->assert->mul->merge";
	resize(image1, image1, cv::Size(0, 0), 0.5, 0.5, INTER_NEAREST);
	qDebug() << "----------getROI->split->assert->mul->merge->resize";
	if (!g_userConfig()->GetDomain().contains("tcrel"))
	{
		QString random = QString::number(qrand());
		int r = qrand() % 155 + 100;
		int g = qrand() % 155 + 100;
		int b = qrand() % 155 + 100;
		putText(image1, random.toStdString(), cv::Point(10, 50), cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(b, g, r),3);
	}
	imwrite(path, image1);
}
