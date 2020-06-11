#include "CameraWidget.h"
#include "QCameraInfo"
#include "QLayout"
#include "QLabel"
#include "QPushButton"
#include "QLineEdit"
#include "QGridLayout"
#include "AnimationWidget/CustomDynamicWidget.h"
#include "QSound"
#include "UserConfig.h"
#include "QLayout"


CameraWidget::CameraWidget(QWidget *parent) :QWidget(parent), 
											mFreeWidget(NULL),
											mGrayBack(NULL),
											mTitle(NULL),
											mHead(NULL),
											mPort(NULL),
											mResult(NULL),
											mAnimation(NULL),
											camera(NULL),
											viewFinder(NULL),
											viewFinderLayout(NULL),
											mCatchedName(""),
											mIsGroup(false)
{	
	initCamera();
	initWidgets();
}


CameraWidget::~CameraWidget()
{
}

void CameraWidget::initCamera()
{
	QString cameraName = g_userConfig()->GetCameraName();
	QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	foreach(const QCameraInfo &cameraInfo, cameras) {
		if (cameraInfo.description().compare(cameraName) == 0)
		{
			camera = new QCamera(cameraInfo,this);
			break;
		}
	}
	if (!camera)
	{
		camera = new QCamera(this);
	}
	
	viewFinder = new QCameraViewfinder(this);	
	viewFinder->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);

	camera->setViewfinder(viewFinder);
	camera->start();
	QList<QSize> listSize = camera->supportedViewfinderResolutions();
	int size = 10 < listSize.size() - 1 ? 10 : listSize.size() - 1;
	QCameraViewfinderSettings viewfinderSettings;
	viewfinderSettings.setResolution(listSize[size]);
	camera->setViewfinderSettings(viewfinderSettings);
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(viewFinder);

	//layout->addSpacing(418);
	this->setLayout(layout);

}

void CameraWidget::initWidgets()
{
	viewFinderLayout = new QGridLayout;
	viewFinderLayout->setContentsMargins(QMargins(0, 0, 0, 0));

	mTitle = new LiveTitleWidget;
	mHead = new LiveHeadWidget;
	mPort = new LivePortWidget;
	mResult = new LiveResultView;
	mFreeWidget = new LiveFreeWidget;
	mGrayBack = new LiveGraybk;
	mAnimation = new AnimationCoins;
	mBigAward = new LiveBigAward;
	mLeftNum = new LiveLeftNumWidget;

	connect(mAnimation, &AnimationCoins::onAnimationEnd, this, &CameraWidget::hideResult);
	
	viewFinderLayout->addWidget(mLeftNum, 0, 0, 3, 1);
	viewFinderLayout->addWidget(mGrayBack, 0, 0, -1, -1);
	viewFinderLayout->addWidget(mAnimation, 0, 0, -1, -1);	
	viewFinderLayout->addWidget(mBigAward, 0, 0, -1, -1);
	viewFinderLayout->addWidget(mResult, 0, 0, -1, -1);
	viewFinderLayout->addWidget(mFreeWidget, 0, 0, -1, -1);	
	viewFinderLayout->addWidget(mPort, 0, 0, 3, 1);
	viewFinderLayout->addWidget(mTitle, 0, 0, 3, 1);
	viewFinderLayout->addWidget(mHead, 0, 0, 3, 1);
	

	viewFinder->setLayout(viewFinderLayout);

	mTitle->setVisible(false);
	mHead->setVisible(false);
	mGrayBack->setVisible(true);
	mAnimation->setVisible(false);
	mResult->setVisible(false);
	mFreeWidget->setVisible(true);
	mLeftNum->setVisible(false);
	mBigAward->setVisible(false);
	setWaiting();
	//startSwipe();
}

void CameraWidget::setIsGroup(QString issue)
{
	mIsGroup = (issue.length() != 0);
	mTitle->setIssue(issue);
}

void CameraWidget::setWaiting()
{
	mTitle->setVisible(false);
	mHead->setVisible(false);
	mGrayBack->setVisible(true);
	mAnimation->setVisible(false);
	mResult->setVisible(true);
	mFreeWidget->setVisible(true);
	mLeftNum->setVisible(false);
	mBigAward->setVisible(false);
	/*mResult->setVisible(false);
	mFreeWidget->setVisible(true);*/
}

void CameraWidget::startSwipe()
{
	QSound::play(":/neworder");
	mFreeWidget->setVisible(false);
	mGrayBack->setVisible(false);
	mResult->setVisible(false);
	mAnimation->setVisible(false);
	mHead->setVisible(true);
	mTitle->setVisible(mIsGroup);
	mLeftNum->setVisible(true);
	//mPort->setVisible(true);
}

void CameraWidget::setSeatIndex(QString seatIndex)
{
	mPort->setIndex(seatIndex);
}

void CameraWidget::setUserInfo(QString userName, QPixmap headPix)
{
	mHead->setUserInfo(userName, headPix);
}

void CameraWidget::setTotalNumAndLeftNum(int totalNum, int leftNum)
{
	mLeftNum->setTotalNumAndLeftNum(totalNum, leftNum);
}

void CameraWidget::setUserName(QString userName)
{
	mHead->setUserName(userName);
}

void CameraWidget::showAward(int awradNum)
{
	if (awradNum > 0 || awradNum == -1)
	{
		mAnimation->setVisible(true);
		showCoinsAnimation(awradNum);
		/*mBigAward->showBigAward(1000);
		mBigAward->setVisible(true);*/
	}
	mResult->setAwardNum(awradNum);
	mResult->setVisible(true);
	mGrayBack->setVisible(true);

	if (awradNum == 0)
	{
		QTimer::singleShot(3000, this, &CameraWidget::hideResult);
	}
}

void CameraWidget::showCoinsAnimation(int awardNum)
{
	if (mAnimation)
	{
		//mAnimation->setAnimation(QPixmap(":/levelNum"), 10, 100);
		mAnimation->startAnimation();
	}
}

void CameraWidget::hideResult()
{
	mTitle->setVisible(false);
	mHead->setVisible(false);
	mGrayBack->setVisible(true);
	mAnimation->setVisible(false);
	mResult->setVisible(false);
	mFreeWidget->setVisible(true);
	mLeftNum->setVisible(false);
	mBigAward->setVisible(false);

	emit onResult();
}

void CameraWidget::paintEvent(QPaintEvent *event)
{
}

void CameraWidget::setSpace(int length)
{
	
}
