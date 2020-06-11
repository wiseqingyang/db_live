#pragma once


#include <QtNetwork/qnetworkaccessmanager.h> 
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QFile>
#include <QUrl>
#include <QTimer>
#include <QJsonParseError>
#include <QTextCodec>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonArray>
#include <QSettings>
#include <functional>
#include <QUrlQuery>

class HttpUint : public QObject
{
	Q_OBJECT
public:
	
	virtual ~HttpUint();
	HttpUint();


	void getData(const QString head, QString content, std::function<void(bool, QJsonObject json)>);
	void postData(const QString head, const QByteArray &data, std::function<void(bool, QJsonObject json)>);
	void uploadImage(const QString head, QJsonObject &obj, const QByteArray &data, std::function<void(bool, QJsonObject json)>);

protected:
	void requestFinished(QNetworkReply *reply, const QByteArray data, const int statusCode);

public slots:
	void serviceRequestFinished(QNetworkReply *reply);

private:
	void post(const QString head, const QByteArray &data);
	void get(const QString head, QString content);
	void upload(const QString head, QJsonObject &obj, const QByteArray &data);

private:
	
	//用于接收服务器房间数据的网络请求
	QNetworkAccessManager m_networkManager;
	std::function<void(bool, QJsonObject json)> checkCallback;
//	QString m_certificatePath;

//	QSslConfiguration m_sslConfig;

};

	