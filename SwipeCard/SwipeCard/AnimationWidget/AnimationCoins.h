#pragma once
#include "qwidget.h"
#include "QTimer"

struct MyPixmap
{
	QPixmap pixmap;
	QPoint position;
	float scale;
	float rotate;
	int frameIndex;
	int xSpeed;
	int ySpeed;
};

class AnimationCoins :
	public QWidget
{
	Q_OBJECT
public:
	AnimationCoins();
	~AnimationCoins();

	void startAnimation();
signals:
	void onAnimationEnd();

private slots:
	void updateList();

private:
	int mCurrentTime;
	QPixmap mCoinIcon;	
	QPixmap mCoinIcons[6];
	QTimer *mTimer;
	QVector<MyPixmap*> mPixVector;

	void paintEvent(QPaintEvent*);
	MyPixmap* createNewPix();
	//返回是否越界 该删除了
	bool updatePixDate(MyPixmap *pix);
	void endAnimation();
};

