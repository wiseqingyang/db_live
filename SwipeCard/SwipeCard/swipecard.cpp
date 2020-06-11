#include "swipecard.h"
#include "QLabel"
#include "QCamera"
#include "QCameraViewfinder"
#include "MessageIdDef.h"
#include "eloamDll.h"
#include "UserConfig.h"
#include "Discern.h"
#include "FunctionDef.h"
#include "QSound"
#include "QFontDatabase"

SwipeCard::SwipeCard(QWidget *parent)
: QDialog(parent), isMax(false), mInputString(""), mCameraWidget(NULL), mSeatIndex(0), mCount(0), mIsBusy(false)
{
	ui.setupUi(this);
	//UI的信号槽
	connect(ui.minBtn, &QPushButton::clicked, this, &SwipeCard::onClickTitleButton);
	connect(ui.maxBtn, &QPushButton::clicked, this, &SwipeCard::onClickTitleButton);
	connect(ui.closeBtn, &QPushButton::clicked, this, &SwipeCard::onClickTitleButton);

	connect(&mCutAndRecgnize, &CutAndRecgnize::updatePic, this, &SwipeCard::receivePics);
	connect(&mCutAndRecgnize, &CutAndRecgnize::onRecognizedTicketCode, this, &SwipeCard::onRecgnizeCode);
	connect(ui.manualMode, &QPushButton::clicked, this, &SwipeCard::onManualModeClicked);
	connect(ui.submitBtn, &QPushButton::clicked, this, &SwipeCard::onSubmitManualCode);
	connect(ui.reset, &QPushButton::clicked, this, &SwipeCard::onResetButton);

	ui.manualMode->setEnabled(false);
	ui.lineEdit->setEnabled(true);

	ui.minBtn->setDefault(false);
	ui.minBtn->setAutoDefault(false); 
	this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏 
	initButtons();
	initCameraView();
	ifWindowsExist();
	mIsAutoMode = true;
	mIsBigPrize = false;
}

SwipeCard::~SwipeCard()
{
	appendRemoveFile("");
}

void SwipeCard::initButtons()
{
	ui.minBtn->setButtonIcon("min");
	ui.maxBtn->setButtonIcon("max");
	ui.closeBtn->setButtonIcon("close");
}

void SwipeCard::connectSocket()
{
	net = new Net();
	connect(net, &Net::connected, this, &SwipeCard::onSocketConnected);
	connect(net, &Net::newOrder, this, &SwipeCard::onNewOrder);
	connect(net, &Net::awardResult, this, &SwipeCard::onAwardResult);
	connect(net, &Net::disconnect, this, &SwipeCard::onSocketDisconnected);
	connect(net, &Net::managerSendAward, this, &SwipeCard::onReceiveManagerMsg);
	bool result = net->open(g_userConfig()->GetScoket());
}

void SwipeCard::onSocketConnected()
{
	QJsonArray roomarr = g_userConfig()->GetRoomArray();
	for (int i = 0; i < roomarr.size();i++)
	{
		QJsonObject login;
		login["msgid"] = join_room_req;
		login["roomid"] = roomarr.at(i).toObject()["roomid"].toInt();
		login["account"] = g_userConfig()->GetAccount();
		login["seatindex"] = g_userConfig()->GetSeatIndex();

		net->sendMsg(login);
	}
	ui.swipe->setText(QString::fromLocal8Bit("刮奖客户端 ") + QString::number(g_userConfig()->GetAccount()));
}

void SwipeCard::onSocketDisconnected()
{
	QMessageBox msgBox;
	msgBox.setText(QString::fromLocal8Bit("与服务器断开连接,请重新登录"));
	appendRemoveFile("");
	int ret = msgBox.exec();
	if (ret == QMessageBox::Ok)
	{
		this->close();
	}
}

void SwipeCard::initCameraView()
{
	mCameraWidget = new CameraWidget(ui.CameraView_2);
	ui.CameraView_2->layout()->addWidget(mCameraWidget);
	connect(mCameraWidget, &CameraWidget::onResult, this, &SwipeCard::onResultEnd);

	int customWH = g_userConfig()->getCustomWH();
	if (customWH != 0)
	{
		resizeCamera(customWH);
	}
}

void SwipeCard::resizeCamera(int WH) {
	if (WH > 0)
	{
		mCameraWidget->setMinimumHeight(WH);
		mCameraWidget->setMaximumHeight(WH);
	}
	else
	{
		mCameraWidget->setMinimumWidth(WH*-1);
		mCameraWidget->setMaximumHeight(WH*-1);
	}
}

void SwipeCard::onClickTitleButton()
{
	QPushButton *pButton = qobject_cast<QPushButton *>(sender());

	if (pButton == ui.minBtn)
	{
		this->showMinimized();
	}
	else if (pButton == ui.closeBtn)
	{
		this->close();
	}
	else if (pButton == ui.maxBtn)
	{
		if (isMax)
		{
			this->showNormal();
			isMax = false;
			ui.maxBtn->setButtonIcon("max");
		}
		else
		{
			this->showMaximized();
			isMax = true;
			ui.maxBtn->setButtonIcon("max_re");
		}
	}
}

void SwipeCard::onNewOrder(QJsonObject &obj)
{
	m_obj = obj;

	if (mCameraWidget && mCameraWidget->isFree())
	{
		delayTakePic();
		setDetails();
	}		
}

void SwipeCard::onReceiveManagerMsg(QJsonObject &obj)
{
	qDebug() << "receive Manager award:: -->" << obj;
	qDebug() << "currOderInfo.oderid ->" << mCurrOrderInfo.orderId;
	qDebug() << "obj.orderid ->" << obj["order_id"].toString();
	if (obj["order_id"].toString().compare(mCurrOrderInfo.orderId) == 0)
	{
		mIsAutoMode = false;
/*
		mCameraWidget->showAward(obj["bonus"].toInt());

		takePic();*/

		QJsonObject awardReq;
		awardReq["msgid"] = award_result_req;
		awardReq["roomid"] = mCurrOrderInfo.roomid;
		awardReq["token"] = g_userConfig()->GetToken();
		awardReq["operatorid"] = g_userConfig()->GetAccount();
		awardReq["orderid"] = mCurrOrderInfo.orderId;
		awardReq["ticketcode"] = obj["ticket_code"].toString();
		awardReq["amount"] = obj["bonus"].toInt();
		awardReq["isgroup"] = mCurrOrderInfo.isGroup;
		if (mCurrOrderInfo.isGroup)
		{
			awardReq["grouptype"] = mCurrOrderInfo.grouptype;
		}
		net->sendMsg(awardReq);
	}
}

void SwipeCard::delayTakePic()
{
	QTimer::singleShot(1500, this, &SwipeCard::updateTakePic);
}

void SwipeCard::updateTakePic()
{
	if (mIsAutoMode)
	{
		takePic();
	}	
}

void SwipeCard::onAwardResult(QJsonObject &obj)
{
	int result = obj["ret"].toInt();
	if (result == 0 && this->mCameraWidget && !mCameraWidget->isFree())
	{
		int bonus = obj["bonus"].toInt();
		if (mIsBigPrize && bonus == 0)
		{
			mCameraWidget->showAward(-1);
			mIsBigPrize = false;
		}
		else
		{
			mCameraWidget->showAward(obj["bonus"].toInt());
			mIsBigPrize = false;
		}
		if (mIsAutoMode)
		{
			uploadImage();
		}
		else
		{
			takePic();
		}		
	}
	else if (result == 1001){    //这个刮错票的情况,lizheng,,20180810

		retryTakePic(); //
	}
	else{
		ui.state->setText(QString::fromLocal8Bit("对比数据错误"));
	}
}

void SwipeCard::receivePics(QString picType, QString imageName)
{
	qDebug() << "receive Pic" << picType << imageName;
	if (picType.compare("savePic") == 0)
	{
		ui.previewPix->setPixmap(QPixmap());
	}
	else if (picType.compare("qrcodePic") == 0)
	{
		ui.qrcode->setPixmap(imageName);
	}
	else if (picType.compare("uploadPic") == 0)
	{
		ui.previewPix->setPixmap(QPixmap(imageName));
	}
	else if (picType.compare("qrcodePic1") == 0)
	{
		ui.qrcode1->setPixmap(imageName);
	}
}

void SwipeCard::uploadImage()
{
	QString newImagePath = mImagePath.replace(".jpg", "_upload.jpg");
	bool isPortrait = mLotteryManager.isWaterMarkPortrait(mCurrOrderInfo.ticketId);
	mCutAndRecgnize.addWaterMark(newImagePath, isPortrait);
	QFile file(newImagePath);
	if (!file.open(QIODevice::ReadOnly) || file.size() == 0)
	{
		file.close();
		QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("文件打开错误，请联系管理员！"));
		return;
	}

	int size = file.size();
	QByteArray fdata = file.readAll();
	if (fdata.isEmpty()){
		return;
	}
	file.close();
	receivePics("uploadPic", newImagePath);

	QJsonObject msg;
	msg["account"] = mCurrOrderInfo.userAccount;
	msg["orderid"] = mCurrOrderInfo.orderId + QString::number(mCurrOrderInfo.leftNum);
	msg["suffix"] = QString(".jpg");

	m_Http.uploadImage("/upload/uploadImage", msg, fdata.toBase64(), [&](bool success, QJsonObject json)
	{
		mIsBusy = false;
		if (success)
		{
			int error = json["errcode"].toInt();
			if (error == 0)
			{
				qDebug() << "upload image success;";
				mImagePath = "";
			}
			else
			{
				QMessageBox::warning(NULL, "warning", json["errmsg"].toString(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			}
		}
		appendRemoveFile("");
	});
}

void SwipeCard::setDetails()
{
	ui.ticketName->setText(m_obj["ticketname"].toString());
	ui.ticketID->setText(QString::number(m_obj["ticketid"].toInt()));

	ui.userName->setText(m_obj["username"].toString());
	ui.userID->setText(QString::number(m_obj["account"].toInt()));

	ui.buyTime->setText(m_obj["buytime"].toString());
	ui.ticketNums->setText(QString("%1/%2").arg(m_obj["buynum"].toInt()).arg(m_obj["amount"].toInt()));
	ui.ticketNumber->setText("0");
	ui.ticketIcon->setPixmap(QPixmap(":/loadingHead"));
	ui.ticketNumber->clear();

	mHttpImage.getData(m_obj["ticketicon"].toString(), [&](bool success, QPixmap pix){
		if (success)
		{
			QSize picSize(190, 190);
			QPixmap scaledPixmap = pix.scaled(picSize, Qt::KeepAspectRatio);
			ui.ticketIcon->setPixmap(scaledPixmap);
		}
	});

	mCurrOrderInfo.headUrl = m_obj["userhead"].toString();
	mCurrOrderInfo.userName = m_obj["username"].toString();
	mCurrOrderInfo.userAccount = m_obj["account"].toInt();
	mCurrOrderInfo.isGroup = m_obj["isgroup"].toInt();
	if (mCurrOrderInfo.isGroup)
	{
		mCurrOrderInfo.issue = m_obj["issue"].toString();
		mCurrOrderInfo.userName = m_obj["ticketname"].toString();
		mCurrOrderInfo.grouptype = m_obj["grouptype"].toInt();
		//qDebug() << "==============>" << mCurrOrderInfo.grouptype << "  " << m_obj["grouptype"];
	}
	else
	{
		mCurrOrderInfo.issue = "";
	}
	mCurrOrderInfo.leftNum = m_obj["buynum"].toInt();
	mCurrOrderInfo.totalNum = m_obj["buynum"].toInt();
	mCurrOrderInfo.amount = m_obj["amount"].toInt();
	mCurrOrderInfo.orderId = m_obj["orderid"].toString();
	mCurrOrderInfo.ticketId = m_obj["ticketid"].toInt();
	mCurrOrderInfo.roomid = m_obj["roomid"].toInt();

	mCameraWidget->setUserInfo(mCurrOrderInfo.userName, QPixmap(":/loadingHead"));


	mHttpImage2.getData(m_obj["userhead"].toString(), [&](bool success, QPixmap pix){

		if (success)
		{
			mCameraWidget->setUserInfo(mCurrOrderInfo.userName, pix);
		}
	});
	
	ui.flowID->setText(m_obj["orderid"].toString());

	mCameraWidget->setTotalNumAndLeftNum(mCurrOrderInfo.amount, mCurrOrderInfo.leftNum);
	mCameraWidget->setIsGroup(mCurrOrderInfo.issue);
	mCameraWidget->startSwipe();

	ui.state->setText(QString::fromLocal8Bit("刮奖中"));
}

void SwipeCard::setDefaultDetails()
{
	ui.state->setText(QString::fromLocal8Bit("等待派单中"));
	ui.ticketNums->setText(QString("%1/%2").arg(0).arg(mCurrOrderInfo.totalNum));
	ui.ticketName->setText(QString::fromLocal8Bit("彩票名称"));
	ui.ticketID->setText(QString::number(0));
	ui.flowID->setText("0");

	ui.userName->setText(QString::fromLocal8Bit("用户名字"));
	ui.userID->setText("0");

	ui.buyTime->setText("0");
	ui.ticketNumber->setText("0");
}

void SwipeCard::onResultEnd()
{
	mIsAutoMode = true;
	if (mCurrOrderInfo.leftNum > 1)
	{
		qDebug() << QString::fromLocal8Bit("开奖完成，还有订单数量  继续开奖");
		delayTakePic();
		mCameraWidget->startSwipe();
		mCurrOrderInfo.leftNum--;
		ui.state->setText(QString::fromLocal8Bit("刮奖中"));
		ui.ticketNums->setText(QString("%1/%2").arg(mCurrOrderInfo.leftNum).arg(mCurrOrderInfo.amount));
		mCameraWidget->setTotalNumAndLeftNum(mCurrOrderInfo.amount, mCurrOrderInfo.leftNum);
	}
	else
	{	
		QString newOrderId = m_obj["orderid"].toString();
		if (newOrderId.compare(ui.flowID->text()) != 0)
		{
			qDebug() << QString::fromLocal8Bit("开奖完成，换新订单");
			delayTakePic();
			setDetails();
		}
		else
		{
			qDebug() << QString::fromLocal8Bit("开奖完成，无新订单");
			setDefaultDetails();
		}
	}
}

void SwipeCard::onRecgnizeCode(QString result, QString imagePath)
{
	handleTicketCode(result);
}

int SwipeCard::callbackFromCollection(QJsonObject& obj)
{
	std::string msgType = obj["msgType"].toString().toStdString();

	if (msgType == "piccaptured")
	{
		mImagePath = obj["fileName"].toString();
		mCutAndRecgnize.setImagePath(obj["fileName"].toString());
		mCutAndRecgnize.setTicketSizePos(mLotteryManager.getTicketSizeAndCodePos(mCurrOrderInfo.ticketId));
		if (mIsAutoMode)
		{			
			mCutAndRecgnize.recognizeCode();
		}
		else
		{
			if (!mIsBusy)
			{
				mIsBusy = true;
				mCutAndRecgnize.onlyCutPic();
				uploadImage();
			}			
		}
	}
	else if (msgType == "authenticate")
	{
		ui.swipe->setText(ui.swipe->text() + QString::fromLocal8Bit(" 已连接"));
	}
	else
	{
		qDebug() << "error...--->" << QString::fromStdString(msgType);
		return 0;
	}
	return 1;
}

void SwipeCard::vertifyCode(QString &str)
{
	ui.state->setText(QString::fromLocal8Bit("数据上传中"));
	ui.ticketNumber->setText(str);
	repaint();

	QUrlQuery params;
	params.addQueryItem("ticketscode", str);
	params.addQueryItem("token", g_userConfig()->GetToken());
	QString url = "/awards/testLotteryVerification";
	if (g_userConfig()->GetDomain().contains("tcrel"))
	{
		url = "/awards/lotteryVerification";
	}

	m_Http.postData(url, params.toString().toLocal8Bit(), [=](bool success, QJsonObject json)
	{
		if (success) {
			int ret_status = json["draw_return_status"].toInt();
			ui.state->setText(json["memo"].toString());
			repaint();
			//ret_status = 4;
			/* O- 不可兑奖  21- 兑奖中 1- 中奖 2- 未中奖 3- 已兑奖 4- 特大奖
				 5- 异地票
				6- 保安区无效7- 兑奖失败8-兑奖超时
			*/
			if (ret_status == 1 || ret_status == 2 || ret_status == 4) {
				mIsBigPrize = (ret_status == 4);
				//中奖.未中奖
				QJsonObject awardReq;
				awardReq["msgid"] = award_result_req;
				awardReq["roomid"] = mCurrOrderInfo.roomid;
				awardReq["token"] = g_userConfig()->GetToken();
				awardReq["operatorid"] = g_userConfig()->GetAccount();
				awardReq["orderid"] = mCurrOrderInfo.orderId;
				awardReq["ticketcode"] = str;
				awardReq["isgroup"] = mCurrOrderInfo.isGroup;
				if (mCurrOrderInfo.isGroup)
				{
					awardReq["grouptype"] = mCurrOrderInfo.grouptype;
				}
				awardReq["amount"] = json["amount"].toInt();
				net->sendMsg(awardReq);
			}
			else {
				mIsBigPrize = false;
				delayTakePic();
				ui.state->setText(json["desc"].toString());
			}
		}
		else {
			mIsBigPrize = false;
			delayTakePic();
			ui.state->setText(QString("网络错误 ").append(json["desc"].toString()));
		}
	});
}

void SwipeCard::onManualModeClicked()
{
}

void SwipeCard::onSubmitManualCode()
{
	QString manualCode = ui.lineEdit->text();
	if (manualCode.length() > 18)
	{
		//getAward(manualCode);
		//vertifyCode(manualCode);
	}

	QString inputStr = ui.lineEdit->text();
	if (inputStr.contains("dbljoy"))
	{
		QString secretCode = inputStr.mid(6, -1);
		qDebug() << "=====> " << inputStr;
		qDebug() << "=======>  " << secretCode;
		int num = secretCode.toInt();

		resizeCamera(num);
		g_userConfig()->setCustomWH(num);
	}

/*
	mCameraWidget->setMinimumHeight(653);
	mCameraWidget->setMaximumHeight(653);*/

}

void SwipeCard::onResetButton() 
{
	mIsAutoMode = true;
	ui.lineEdit->setEnabled(false);
	ui.lineEdit->clear();
	//ui.manualMode->setEnabled(true);

	if (mCameraWidget && !mCameraWidget->isFree())
	{
		delayTakePic();
	}
}

void SwipeCard::retryTakePic(){
	mIsAutoMode = true;
	delayTakePic();
	ui.state->setText(QString::fromLocal8Bit("拿错票了，请换票来刮！"));
}


void SwipeCard::takePic()
{
	ui.state->setText(QString::fromLocal8Bit("拍照中…"));
	repaint();
	QJsonObject msg;
	msg["msgType"] = "capturepic";
	send_message(msg);	
}

//其他进程发过来的消息。
bool SwipeCard::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	MSG* msg = reinterpret_cast<MSG*>(message);
	if (msg->message == WM_COPYDATA){
		COPYDATASTRUCT *p = reinterpret_cast<COPYDATASTRUCT*>(msg->lParam);
		char *c = static_cast<char*>(p->lpData);

		std::string temp(c);
		int nhead = temp.find("{");
		int nend = temp.rfind("}");
		std::string temp_1 = temp.substr(nhead, nend + 1);

		QString strContent = QString::fromStdString(temp_1);

		QJsonParseError jsonError;
		QJsonDocument doc = QJsonDocument::fromJson(strContent.toLocal8Bit(), &jsonError);
		if (jsonError.error == QJsonParseError::NoError)
		{
			QJsonObject obj = doc.object();
			int result = callbackFromCollection(obj);

			if (reset == 0)
			{
				//自己模块的消息
			}
		}
		else
		{
			qDebug() << jsonError.errorString() << "--->" << strContent;
		}

		qDebug() << "--nativeEvent-->>" << strContent;
	}
	return QDialog::nativeEvent(eventType, message, result);
}

void SwipeCard::keyPressEvent(QKeyEvent *keyValue)
{
	if (mCameraWidget && !mCameraWidget->isFree())
	{		
	}	
}

void SwipeCard::updateInput()
{
	mCount++;
	if (mCount > 1)
	{
		mInputString.clear();
	}	
}

void SwipeCard::handleTicketCode(QString code)
{
	if (code.length() > 20)
	{
		vertifyCode(code);
	}
	else
	{
		qDebug() << "recognize failed!";
		appendRemoveFile(mImagePath);
		ui.state->setText(QString::fromLocal8Bit("识别失败，请重试!"));
		delayTakePic();
	}
}

void SwipeCard::appendRemoveFile(QString fileStr)
{
	if (mPicStrToDel.size() >= 10 || fileStr.length() == 0 )
	{
		for (QVector<QString>::iterator iter = mPicStrToDel.begin(); iter != mPicStrToDel.end(); iter++)
		{
			qDebug() << "remove file : " << *iter;
			QFile file(*iter);
			if (file.exists())
			{
				bool a = file.remove();
				QString debugInfo = "delete pic ::: " + *iter + (a ? "  succeed" : "failed");
				qDebug() << debugInfo;
			}
			QString newFileName = iter->replace(".jpg", "_upload.jpg");
			QFile file1(newFileName);
			if (file1.exists())
			{
				bool a = file1.remove();
				QString debugInfo = "delete pic ::: " + *iter + (a ? "  succeed" : "failed");
				qDebug() << debugInfo;
			}
		}
		mPicStrToDel.clear();
	}
	if (fileStr.length() > 0)
	{
		mPicStrToDel.append(fileStr);
	}
}

void SwipeCard::mousePressEvent(QMouseEvent *event)
{
	mousePosition = event->pos();
	//只对标题栏范围内的鼠标事件进行处理

	if (mousePosition.y() <= pos_min_y)
		return;
	if (mousePosition.y() >= pos_max_y)
		return;
	isMousePressed = true;
}

void SwipeCard::mouseMoveEvent(QMouseEvent *event)
{
	if (isMousePressed == true)
	{
		QPoint movePot = event->globalPos() - mousePosition;
		//move(movePot);
	}
}

void SwipeCard::mouseReleaseEvent(QMouseEvent *event)
{
	isMousePressed = false;
}

void SwipeCard::mouseDoubleClickEvent(QMouseEvent *event)
{
	//双击标题栏同样可以放大缩小
	mousePosition = event->pos();
	//只对标题栏范围内的鼠标事件进行处理

	if (mousePosition.y() <= pos_min_y)
		return;
	if (mousePosition.y() >= pos_max_y)
		return;

	if (isMax)
	{
		this->showNormal();
		isMax = false;
		ui.maxBtn->setButtonIcon("max");
	}
	else
	{
		this->showMaximized();
		isMax = true;
		ui.maxBtn->setButtonIcon("max_re");
	}
}

void SwipeCard::showEvent(QShowEvent *event)
{
	QDialog::showEvent(event);
	this->setAttribute(Qt::WA_Mapped);
	mSeatIndex = g_userConfig()->GetSeatIndex();
	mCameraWidget->setSeatIndex(QString::number(mSeatIndex));
	ui.portID->setText(QString::number(mSeatIndex));
	mTimer = new QTimer(this);
	mTimer->setInterval(1000);
	connect(mTimer, &QTimer::timeout, this, &SwipeCard::updateInput);
	mTimer->start();
	mLotteryManager.updateAllTickets();
	this->showMaximized();
	isMax = true;
	ui.maxBtn->setButtonIcon("max_re");
}

void SwipeCard::closeEvent(QCloseEvent *e)
{
	QDialog::closeEvent(e);
	QJsonObject msg;
	msg["msgType"] = "killme";
	send_message(msg);
}

bool SwipeCard::eventFilter(QObject *target, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Escape)
			return true;
	}
	return QDialog::eventFilter(target, event);
}