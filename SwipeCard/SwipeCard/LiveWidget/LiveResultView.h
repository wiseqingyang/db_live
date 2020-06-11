#pragma once
#include "qwidget.h"
#include "QTimer"

class LiveResultView :
	public QWidget
{
	Q_OBJECT
public:
	LiveResultView();
	~LiveResultView();

	void setAwardNum(int awardNum);

protected:
	virtual void paintEvent(QPaintEvent *event);
	void stopAnimate();
	void getBigPrizePixmaps();
	void getBigPrizePixmapsTmp();

private slots:
	void updateJumpDatas();

private:
	int mAwardNum;
	int mTotalWidth;
	QTimer *mTimer;
	QList<QPixmap> mPixmapList;
	QVector <int> mJumpDatas;
	QVector <int> mSpeeds;
	int mJumpTime;

	void getFinalPixmaps();
	void startAnimation();
};

