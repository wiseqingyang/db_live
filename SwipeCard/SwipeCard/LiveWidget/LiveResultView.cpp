#include "LiveResultView.h"
#include "QPainter"
#include "QDebug"

LiveResultView::LiveResultView()
{
	mAwardNum = 0;
	mTotalWidth = 0;
	mPixmapList.clear();
	mJumpDatas.clear();
	mSpeeds.clear();
	mJumpTime = 0;
	mTimer = NULL;
}

LiveResultView::~LiveResultView()
{
}

void LiveResultView::setAwardNum(int awardNum)
{
	mAwardNum = awardNum;
	if (mAwardNum == -1)//中大奖  临时性的
	{
		getBigPrizePixmapsTmp();
		mSpeeds.clear();
		mJumpDatas.clear();
		startAnimation();
	}
	else if (mAwardNum >= 10000)
	{
		getBigPrizePixmaps();
		mSpeeds.clear();
		mJumpDatas.clear();
		startAnimation();
	}
	else
	{
		getFinalPixmaps();
	}
}

void LiveResultView::startAnimation()
{	
	if (!mTimer)
	{
		mTimer = new QTimer(this);
		mTimer->setInterval(1000 / 20);//一秒20次
		connect(mTimer, &QTimer::timeout, this, &LiveResultView::updateJumpDatas);
	}	
	mTimer->start();
}

void LiveResultView::updateJumpDatas()
{
	mJumpTime++;
	int maxSpeed = 50;

	if (mSpeeds.isEmpty())
	{
		for (int i = 0; i < mPixmapList.size(); i++)
		{
			mSpeeds.append(0);
		}
	}

	if (mJumpDatas.isEmpty())
	{
		for (int i = 0; i < mPixmapList.size(); i++)
		{
			mJumpDatas.append(0);
		}
	}
	for (int i = 0; i < mPixmapList.size(); i++)
	{
		if (mJumpTime > i* 3)//前一个跳动和后一个跳动有0.3秒间隔
		{
			if (mSpeeds[i] == 0 && mJumpTime < (i+1) * 3)
			{
				mSpeeds[i] = maxSpeed*-1;
			}
			else
			{
				if (mJumpDatas[i] != 0)
				{
					mSpeeds[i] += 10;
				}				
				if (mSpeeds[i] >= maxSpeed)
				{
					mSpeeds[i] = maxSpeed;
				}
				mJumpDatas[i] += mSpeeds[i];
				if (mJumpDatas[i] > 0)
				{
					mJumpDatas[i] = 0;
				}
				if (i == (mPixmapList.size() - 1) && mSpeeds[i] == maxSpeed) //最后一个且速度为最大值，说明最后一个回到了原点
				{
					stopAnimate();
				}
			}
		}
	}
}

void LiveResultView::stopAnimate()
{
	qDebug() << "stopped!!!!";
	mTimer->stop();
	mJumpTime = 0;
	mSpeeds.clear();
	mJumpDatas.clear();
}

void LiveResultView::getFinalPixmaps()
{
	if (!mPixmapList.empty())
	{
		mPixmapList.clear();
		mTotalWidth = 0;
	}
	if (mAwardNum <= 0)
	{
		mPixmapList.append(QPixmap(":/resultNon"));
	}
	else
	{
		mPixmapList.append(QPixmap(":/resultCongra"));
		QString awardNum = QString("%1").arg(mAwardNum);
		QStringList strList = awardNum.split("");
		for (int i = 0; i < strList.size(); i++ )
		{
			mPixmapList.append(QPixmap(QString(":/result%1").arg(strList.at(i))));
		}
		mPixmapList.append(QPixmap(":/resultYuan"));
		this->repaint();
	}

	for (int i = 0; i < mPixmapList.size(); i++)
	{
		mTotalWidth += mPixmapList.at(i).width();
	}
	mJumpDatas.clear();
}

void LiveResultView::getBigPrizePixmapsTmp()
{
	if (!mPixmapList.empty())
	{
		mPixmapList.clear();
		mTotalWidth = 0;
	}
	mPixmapList.append(QPixmap(":/resultBigXi"));
	mPixmapList.append(QPixmap(":/resultBigZhong"));
	mPixmapList.append(QPixmap(":/resultBigDa"));
	mPixmapList.append(QPixmap(":/resultBigJiang"));

	for (int i = 0; i < mPixmapList.size(); i++)
	{
		mTotalWidth += mPixmapList.at(i).width();
	}
}

void LiveResultView::getBigPrizePixmaps()
{
	if (!mPixmapList.empty())
	{
		mPixmapList.clear();
		mTotalWidth = 0;
	}
	mPixmapList.append(QPixmap(":/resultBigXi"));
	mPixmapList.append(QPixmap(":/resultBigZhong"));
	mPixmapList.append(QPixmap(":/resultBigDa"));
	mPixmapList.append(QPixmap(":/resultBigJiang"));
	QString awardNum = QString::number(mAwardNum);
	QStringList strList = awardNum.split("");
	for (int i = 0; i < strList.size(); i++)
	{
		mPixmapList.append(QPixmap(QString(":/resultBig%1").arg(strList.at(i))));
	}

	mPixmapList.append(QPixmap(":/resultBigYuan"));

	for (int i = 0; i < mPixmapList.size(); i++)
	{
		mTotalWidth += mPixmapList.at(i).width();
	}
}

void LiveResultView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int tmpWidth = mJumpDatas.isEmpty() ? 950 : 500;
	float scale = width() / 950;
	int currX = (width() - mTotalWidth*scale) / 2;
	for (int i = 0; i < mPixmapList.size(); i++)
	{
		QPixmap pixmap = mPixmapList.at(i);
		int yadd = mJumpDatas.isEmpty() ? 0 : mJumpDatas.at(i);
		QRect rect(currX, (height() - pixmap.height()*scale) / 2 + yadd, pixmap.width()*scale, pixmap.height()*scale);
		painter.drawPixmap(rect, pixmap);
		currX += rect.width();
	}		
}
