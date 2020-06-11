#include "LiveBigAward.h"
#include "QPainter"
#include "QDebug"
#include "QtMath"

//20分之一秒
#define INTERVAL (1000/20)
#define MAXWIDTH 1180
#define PAI 3.1415926

LiveBigAward::LiveBigAward()
{
	mWinAmount = 0;
	EffectConf level1 = { ":/youting", true, 1.5, 0.5, 1.0 };
	EffectConf level2 = { ":/caishen", false, 1.8, 1.0, 1.0 };
	effConfigs->push_back(level1);
	effConfigs->push_back(level2);
	mDataConfg = { false, QRect(0, 0, 0, 0), QRect(0, 0, 0, 0), QPixmap(), 0, 0,0 };
}


LiveBigAward::~LiveBigAward()
{
}

void LiveBigAward::showBigAward(int bonus)
{
	mWinAmount = bonus;
	if (mWinAmount >= 100)
	{
		//大奖
		mDataConfg.effectIndex = 0;
		mDataConfg.timeIndex = 0;

		mTimer = new QTimer(this);
		mTimer->setInterval(INTERVAL);
		connect(mTimer, &QTimer::timeout, this, &LiveBigAward::updateAnimationData);
		mTimer->start();
	}	
}

void LiveBigAward::updateAnimationData()
{
	/*
		长度为 length = dest - from
		时间刻度为 start stop
		y = tan(x) * 1 + 0；(x取-45 - 45 y 取-1 - 1)
		y = tan(x) * length/(1- -1) + (length/2); (PAI/2 - -PAI/2)
		y = tan((x/((stop-start)/PAI))-PAI/2 )*length/2+length/2;

	*/
	EffectConf tmpconf = effConfigs->at(mDataConfg.effectIndex);
	QPixmap pix = QPixmap(tmpconf.fileName);
	int speed = MAXWIDTH / (tmpconf.totalTime * 1000 / INTERVAL);
	int isLeft = tmpconf.isLeft ? 1 : -1;
	float scaleSpeed = (tmpconf.toScale - tmpconf.fromScale) / (tmpconf.totalTime * 1000 / INTERVAL);
	//currScale todo

	float xSeg = tmpconf.totalTime * 1000 / INTERVAL;
	float srcxSeg = PAI / 4 - (-PAI / 4);
	float ySeg = tmpconf.toScale - tmpconf.fromScale;
	float srcySeg = 1 - -1;

	float scale = (tan(mDataConfg.timeIndex * (srcxSeg / xSeg) + (-PAI / 4)) - -1)*(ySeg / srcySeg) + tmpconf.fromScale;
	scale = ((float)((int)(scale * 100))) / 100;
	//qDebug() << "------>"<<scale;

	float xSeg1 = tmpconf.totalTime * 1000 / INTERVAL;
	float srcxSeg1 = PAI / 4 - (-PAI / 4);
	float ySeg1 = MAXWIDTH + pix.width();
	float srcySeg1 = 1 - -1;

	float xpos = (tan(mDataConfg.timeIndex * (srcxSeg1 / xSeg1) + (-PAI / 4)) - -1)*(ySeg1 / srcySeg1) + 1;
	xpos = ((float)((int)(xpos * 100))) / 100;

	if (mDataConfg.timeIndex == 0)
	{
		mDataConfg.showing = true;
	}
	mDataConfg.timeIndex++;

	if (mDataConfg.timeIndex >= xSeg)
	{
		mDataConfg.timeIndex = xSeg;
	}

	pix = pix.scaled(QSize(pix.width()*scale, pix.height()*scale));

	QRect realRect = QRect(-pix.width() + xpos, (height() - pix.height()) / 2, pix.width(), pix.height());
	
	mDataConfg.rect = this->rect().intersected(realRect);
	//qDebug() << "-------->" << realRect;
	mDataConfg.drawPix = getPixmapWithRect(mDataConfg.rect, pix);
	if (mDataConfg.rect.width() == 0)
	{
		mDataConfg = { false, QRect(0, 0, 0, 0), QRect(0, 0, 0, 0), QPixmap(), 0, 0, 0 };
		mTimer->stop();
	}
	repaint();
}

QPixmap LiveBigAward::getPixmapWithRect(QRect &pixRect, QPixmap &pixmap)
{
	if (pixRect.width() == pixmap.width())
	{
		return pixmap;
	}
	else
	{
		if (pixRect.x() == 0)
		{
			return pixmap.copy(QRect(pixmap.width() - pixRect.width(), 0, pixRect.width(), pixRect.height()));
		}
		else
		{
			return pixmap.copy(QRect(0, 0, pixRect.width(), pixRect.height()));
		}
	}
}

void LiveBigAward::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	if (mDataConfg.showing)
	{
		painter.drawPixmap(mDataConfg.rect, mDataConfg.drawPix);
	}
}
