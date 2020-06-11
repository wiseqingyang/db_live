#ifndef SWIPECARD_H
#define SWIPECARD_H

#include <QtWidgets/QDialog>
#include "ui_swipecard.h"
#include "NetManager/Net.h"
#include "CameraWidget.h"
#include "QKeyEvent"
#include "QJsonArray"
#include "QJsonObject"
#include "Utils/HttpImage.h"
#include "QTimer"
#include "Discern.h"
#include "HttpUint.h"
#include "CutAndRecgnize.h"
#include "LotteryManager.h"

//标题栏坐标范围
#define pos_min_y  0
#define pos_max_y  40

struct orderInfo
{
	QString userName;
	QString headUrl;
	int userAccount;
	int totalNum;
	int leftNum;
	bool isGroup;
	int grouptype;
	QString orderId;
	int ticketId;
	int roomid;
	QString issue;
	int amount;
};

class SwipeCard : public QDialog
{
	Q_OBJECT

public:
	SwipeCard(QWidget *parent = 0);
	~SwipeCard();

	void connectSocket();

private slots:
	//void connectSocket();
	void onSocketConnected();
	void onSocketDisconnected();
	void onClickTitleButton();
	void onNewOrder(QJsonObject &obj);
	void onResultEnd();
	void updateInput();
	void onAwardResult(QJsonObject &obj);
	void receivePics(QString picType, QString imageName);
	void onReceiveManagerMsg(QJsonObject &obj);
	void onManualModeClicked();
	void onSubmitManualCode();
	void onResetButton();
	void onRecgnizeCode(QString result, QString imagePath);

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *e);
	virtual bool eventFilter(QObject *target, QEvent *event);
	void keyPressEvent(QKeyEvent *keyValue);

private:
	Ui::SwipeCardClass ui;
	Net *net;
	CameraWidget *mCameraWidget;
	bool isMax;
	QJsonObject m_obj;
	HttpImage mHttpImage;
	HttpImage  mHttpImage2;
	QString mInputString;
	orderInfo mCurrOrderInfo;
	int mSeatIndex;
	QTimer *mTimer;
	HttpUint m_Http;
	QString mImagePath;
	int mCount;
	QWidget *mWidget;
	CutAndRecgnize mCutAndRecgnize;
	QVector<QString> mPicStrToDel;
	LotteryManager mLotteryManager;

	bool mIsBusy;
	bool mIsAutoMode;
	bool mIsBigPrize;

	//实现自定义标题栏的拖动
	bool isMousePressed;
	QPoint mousePosition;

	void initCameraView();
	void initButtons();
	void setDetails();
	void setDefaultDetails();
	void vertifyCode(QString &str);
	void takePic();
	void retryTakePic();  //票刮错误，重试。

	int callbackFromCollection(QJsonObject& obj);
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
	void handleTicketCode(QString code);
	void appendRemoveFile(QString fileStr);
	void delayTakePic();
	void updateTakePic();
	void uploadImage();
	void resizeCamera(int WH);
};

#endif // SWIPECARD_H
