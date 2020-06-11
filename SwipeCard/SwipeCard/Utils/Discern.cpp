#include "Discern.h"
#include <QDebug>
#include "QJsonObject"
#include "FunctionDef.h"

#define DISCERN_W  150
#define DISCERN_H  150

Discern::Discern()
{
}


Discern::~Discern()
{
}

QString Discern::decodeImage(QImage* img,int i)
{
	int w = img->width();
	int h = img->height();
	QRect rect(0, h - 120, w, 120);
	QImage m = img->copy(rect);
	QString info = m_Zxing.decodeImage(m);
	m.save("newimage.jpg");
	qDebug() << "newImageCode is  " << info;
	return info;
}

QString Discern::decodeImage(QImage* originImg)
{
	//decodeImage(originImg, 1);
	int lightValue = 0;
	int threshold = 50;
	int shorter = originImg->width() < originImg->height() ? originImg->width() : originImg->height();
	float margins = 25 / 850.0 * shorter;
	QImage img = originImg->copy(QRect(margins, margins, originImg->width() - margins * 2, originImg->height() - margins * 2));
	int w = img.width();
	int h = img.height();
	//img.save("11.jpg");

	int minner = w < h ? w : h;
	float xRate = 145 / 850.0;
	float discernW = xRate*minner;
	float discernH = discernW;

	QRect rect[] = {
		QRect(0, h - discernH, discernW, discernH),
		QRect(w - discernW, h - discernH, discernW, discernH),
		QRect(w - discernW, 0, discernW, discernH),
		QRect(0, 0, discernW, discernH),
	};

	QJsonObject	json;

	for (size_t i = 0; i < 4; i++)
	{
		QImage m = img.copy(rect[i]);
		//m = m.scaled(QSize(m.width() * 10, m.height() * 10));
		QImage* m1 = lightness(lightValue, &m);
		//m1 = greyScale(m1);
		//m1 = saturation(150, m1);
		//m = convertToBinary(500, m);
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
		QString name = QString::number(i)+ ".jpg";

		m1->save(name);

		delete m1;
	}

	if (m_RectInfo.isEmpty())
	{
		int bigger = w > h ? w : h;
		float xRate = 145 / 850.0;
		float discernW = xRate*bigger;
		float discernH = discernW;

		QRect rect[] = {
			QRect(0, h - discernH, discernW, discernH),
			QRect(w - discernW, h - discernH, discernW, discernH),
			QRect(w - discernW, 0, discernW, discernH),
			QRect(0, 0, discernW, discernH),
		};

		QJsonObject	json;

		for (size_t i = 0; i < 4; i++)
		{
			QImage m = img.copy(rect[i]);
			//m = m.scaled(QSize(m.width() * 10, m.height() * 10));
			QImage* m1 = lightness(lightValue, &m);
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
			QString name = QString::number(i) + ".jpg";

			m1->save(name);

			delete m1;
		}

	}
	
	if (m_RectInfo.size() == 1)
	{
		QRect rr[2];
		sRectInfo rinfo = m_RectInfo[0];

		QRect tmpRect = rinfo.rect;

		if (tmpRect.x() == 0)
		{
			rr[0] = QRect(tmpRect.x() + tmpRect.width(), tmpRect.y(), tmpRect.width(), tmpRect.height());
		}
		else
		{
			rr[0] = QRect(tmpRect.x() - tmpRect.width(), tmpRect.y(), tmpRect.width(), tmpRect.height());
		}

		if (tmpRect.y() == 0)
		{
			rr[1] = QRect(tmpRect.x(), tmpRect.y() + tmpRect.height(), tmpRect.width(), tmpRect.height());
		}
		else
		{
			rr[1] = QRect(tmpRect.x(), tmpRect.y() - tmpRect.height(), tmpRect.width(), tmpRect.height());
		}

		for (size_t i = 0; i < 2; i++)
		{
			QImage m = img.copy(rr[i]);

			QImage* m1 = lightness(lightValue, &m);

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

			qDebug() << info; //<< " index : " << QString::number(i);

			QString name = QString::number(i+4) + ".jpg";

			m1->save(name);

			delete m1;
		}
	}

	for (int i = 0; i < m_RectInfo.size(); i++)
	{
		qDebug() << i << "index strCode is " << m_RectInfo.at(i).strCode;
	}

	QString returnStr = "";
	if (m_RectInfo.size() == 2)
	{
		returnStr = QString("%1 %2").arg(m_RectInfo.at(0).strCode).arg(m_RectInfo.at(1).strCode);
	}
	else if(m_RectInfo.size() == 1)
	{
		returnStr = m_RectInfo.at(0).strCode;
	}
	m_RectInfo.clear();
	return returnStr;
}