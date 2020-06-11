#pragma once

#include <QObject.h>
#include <QImage.h>
#include "qzxing.h"
#include <vector>
#include "QVector"



struct sRectInfo
{
	QRect rect;
	QString strCode;
	bool is_exist;
	int index;
};


class Discern : public QObject
{
public:
	Discern();
	virtual ~Discern();

	QString decodeImage(QImage* img);
	QString decodeImage(QImage* img, int i);

private:

//	QImage m_Image;
	QZXing m_Zxing;

	QVector<sRectInfo> m_RectInfo;     //±£¥Ê–≈œ¢
};

