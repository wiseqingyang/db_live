#include "Net.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "UserConfig.h"
#include "MessageIdDef.h"
#include "FunctionDef.h"

Net::Net(QObject *parent ) :QObject(parent)
{
	connect(&mSocket, &QWebSocket::connected, this, &Net::onConnected);
	connect(&mSocket, &QWebSocket::disconnected, this, &Net::socketDisconnected);
}


Net::~Net()
{
	mSocket.close();
}

bool Net::open(QString &address)
{
	QString path = QString("ws://%1").arg(address);
	QUrl url = QUrl(path);
	qDebug() << "attempt connect to url:" << path;
	mSocket.open(url);

	qDebug() << QString("socket state is %1").arg(mSocket.state());
	return true;
}

void Net::onConnected()
{
	connect(&mSocket, &QWebSocket::textMessageReceived, this, &Net::handleTextMessage);
	qDebug() << "connect succeed:" ;
	mTimer = new QTimer(this);
	mTimer->setInterval(100000);
	connect(mTimer, &QTimer::timeout, this, &Net::heartBeating);
	mTimer->start();
	emit connected();
}

void Net::handleTextMessage(const QString &message)
{
	QJsonObject obj = getJsonObjectFromString(message);

	qDebug() << QString("receive Msg====> %1").arg(message);
	int msgid = obj["msgid"].toInt();
	qDebug() << "msgid:" << msgid;
	switch (msgid)
	{
	case beting_notify_to_operator:
		emit newOrder(obj);
		break;

	case join_room_ret:
		break;

	case award_result_ret:
		emit awardResult(obj);
		break;

	case manager_lottery_notify:
		emit managerSendAward(obj);
		break;

	default:
		break;
	}
}

void Net::heartBeating(){
	QJsonObject msg;
	msg["msgid"] = heart_beating;
	sendMsg(msg);

	QJsonObject msg1;
	msg1["msgType"] = "heaartBeat";
	send_message(msg1);
	//高拍仪进程间也需要心跳
}

void Net::socketDisconnected()
{
	QWebSocket *pClient = qobject_cast<QWebSocket*>(sender());

	qDebug() << pClient->errorString();
	emit disconnect();
}

void Net::sendMsg(QJsonObject msgTab)
{
	QString content = QString(QJsonDocument(msgTab).toJson());
	content.replace(" ", "");
	content.replace("\n", "");
	qDebug() << "send Msg" << content;
	QByteArray postData = content.toUtf8();

	mSocket.sendTextMessage(postData);
}

/*void Net::onGetAttribute(QString age, QString sex, QString image)
{
	QString str_send_info = QString("{\"age\":\"%1\",\"sex\":\"%2\",\"image\":\"%3\"}")
		.arg(age)
		.arg(sex)
		.arg(image);

	QByteArray postData = str_send_info.toUtf8();

	/*m_current_clients->sendTextMessage(postData);
	m_current_clients->flush();
}*/

QJsonObject Net::getJsonObjectFromString(const QString jsonString){
	QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
	if (jsonDocument.isNull()){
		qDebug() << "===> please check the string " << jsonString.toLocal8Bit().data();
	}
	QJsonObject jsonObject = jsonDocument.object();
	return jsonObject;
}

// QJson >> QString  
QString Net::getStringFromJsonObject(const QJsonObject& jsonObject){
	return QString(QJsonDocument(jsonObject).toJson());
}
