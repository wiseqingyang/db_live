#include "AnimationCoins.h"
#include "QPainter"
#include "QDebug"
#include <QtGlobal>
#include "QTime"

int xSpeedRange = 30;
int ySpeedRange = 30;
int ySpeedAdd = 3;

AnimationCoins::AnimationCoins()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	mCoinIcon = QPixmap(":/coin");
	mCurrentTime = 1;
	mTimer = NULL;
	mPixVector.clear();
	for (int i = 1; i < 7; i++)
	{
		mCoinIcons[i - 1] = QPixmap(QString(":/coinIcon%1").arg(i));
	}
}


AnimationCoins::~AnimationCoins()
{
	if (mTimer)
	{
		delete mTimer;
	}
	if (mPixVector.size()>0)
	{		
		for (QVector<MyPixmap*>::iterator ite = mPixVector.begin(); ite != mPixVector.end();)
		{
			delete *ite;
			ite = mPixVector.erase(ite);
		}
	}
}

void AnimationCoins::startAnimation()
{
	if (!mPixVector.isEmpty())
	{
		mPixVector.clear();
	}
	mTimer = new QTimer(this);
	mTimer->setInterval(1000/20);
	connect(mTimer, &QTimer::timeout, this, &AnimationCoins::updateList);
	mTimer->start();
}

void AnimationCoins::updateList()
{
	for (QVector<MyPixmap*>::iterator iter = mPixVector.begin(); iter != mPixVector.end(); )
	{
		MyPixmap *tmpPix = *iter;
		tmpPix->frameIndex++;
		if (updatePixDate(tmpPix))
		{			
			delete tmpPix;
			iter = mPixVector.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	int newNum = 5 / (mCurrentTime*0.3);
	mCurrentTime ++;

	for (int i = 0; mCurrentTime%5==1,i < newNum; i++)
	{
		MyPixmap *newPix = createNewPix();;
		mPixVector.append(newPix);
	}
	if (newNum <= 0 && mPixVector.size() == 0)
	{
		endAnimation();
	}
	update();
}

void AnimationCoins::endAnimation()
{
	mTimer->stop();
	mPixVector.clear();
	mCurrentTime = 1;
	emit onAnimationEnd();
}

MyPixmap *AnimationCoins::createNewPix()
{
	int signal = (qrand() % 2) * 2 - 1;//随机正负
	QPoint originPoint(width() / 2.0 + signal*(qrand()%(width()/4)), height());
	MyPixmap *newPix = new MyPixmap;
	newPix->pixmap = mCoinIcons[0];
	newPix->frameIndex = 0;
	newPix->xSpeed = signal*(qrand() % xSpeedRange);
	newPix->ySpeed = -1 * (qrand() % ySpeedRange + 40);
	newPix->position = originPoint;
	return newPix;
}

bool AnimationCoins::updatePixDate(MyPixmap *pix)
{
	QPoint oldPoint = pix->position;
	pix->pixmap = mCoinIcons[pix->frameIndex/4%6];//每4帧换一张图片
	pix->position = QPoint(oldPoint.x() + pix->xSpeed, oldPoint.y() + pix->ySpeed);
	pix->ySpeed += ySpeedAdd;
	if (pix->position.x()<-pix->pixmap.width() || pix->position.x()>width() || pix->position.y() > height())
	{
		return true;
	}
	return false;
}

void AnimationCoins::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	for (QVector<MyPixmap*>::iterator iter = mPixVector.begin(); iter != mPixVector.end(); iter++)
	{
		MyPixmap *tmpPix = *iter;
		painter.drawPixmap(tmpPix->position, tmpPix->pixmap);
	}
}
