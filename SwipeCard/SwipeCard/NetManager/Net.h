#pragma once
#include "qobject.h"
#include "QWebSocket"
#include "QTimer"
//#include <QtSocketIo/QSocketIOClient>

class Net :
	public QObject
{
	Q_OBJECT
public:
	Net(QObject *parent = Q_NULLPTR);
	~Net();
	bool open(QString &address);
	void sendMsg(QJsonObject content);
	QJsonObject getJsonObjectFromString(const QString jsonString);
	QString getStringFromJsonObject(const QJsonObject& jsonObject);

signals:
	void disconnect();
	void newOrder(QJsonObject &obj);
	void connected();
	void awardResult(QJsonObject &obj);
	void managerSendAward(QJsonObject &obj);

private slots:
	void onConnected();
	void handleTextMessage(const QString &message);
	void socketDisconnected();
	void heartBeating();

	//void onGetAttribute(QString age, QString sex, QString image);
private:
	QWebSocket mSocket;
	bool m_debug;
	QTimer *mTimer;

	//AttributeDetect* m_attribute_thread;
};

