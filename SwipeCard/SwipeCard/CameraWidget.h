#pragma once
#include "qwidget.h"
#include "QCamera"
#include "QCameraViewfinder"
#include "QLayout"
#include "LiveFreeWidget.h"
#include "AnimationCoins.h"
#include "LiveGraybk.h"
#include "LiveHeadWidget.h"
#include "LiveTitleWidget.h"
#include "LivePortWidget.h"
#include "LiveResultView.h"
#include "LiveLeftNumWidget.h"
#include "LiveBigAward.h"

class QCamera;
class QCameraViewfinder;

class CameraWidget :
	public QWidget
{
	Q_OBJECT
public:
	CameraWidget(QWidget *parent);
	~CameraWidget();

	void setIsGroup(QString issue);
	void setSeatIndex(QString seatIndex);
	void setUserInfo(QString userName, QPixmap headUrl);
	void showAward(int awradNum);
	void startSwipe();
	bool isFree(){ return mFreeWidget && mFreeWidget->isVisible(); }
	void setSpace(int length);
	void setUserName(QString userName);
	void setTotalNumAndLeftNum(int totalNum, int leftNum);

protected:
	void paintEvent(QPaintEvent *event);

signals:
	void onResult();	

private slots:
	void hideResult();

private:
	QCamera *camera;
	QCameraViewfinder *viewFinder;
	QGridLayout *viewFinderLayout;

	QString mCatchedName;
	QPixmap mCatchedPixMap;

	LiveFreeWidget *mFreeWidget;
	LiveGraybk *mGrayBack;
	LiveTitleWidget *mTitle;
	LiveHeadWidget *mHead;
	LivePortWidget *mPort;
	LiveResultView *mResult;
	AnimationCoins *mAnimation;
	LiveLeftNumWidget *mLeftNum;
	LiveBigAward *mBigAward;

	bool mIsGroup;

	void initCamera();
	void initWidgets();
	void setWaiting();
	void showCoinsAnimation(int awardNum);
};

