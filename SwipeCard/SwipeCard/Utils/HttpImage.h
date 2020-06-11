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


class HttpImage : public QObject
{
	Q_OBJECT
public:

	virtual ~HttpImage();
	HttpImage();

	void getData(const QString url, std::function<void(bool, QPixmap pixmap)> callback);
	//void uploadImage(const QString head, QJsonObject &obj, const QByteArray &data, std::function<void(bool, QJsonObject json)>);
protected:
	void requestFinished(QNetworkReply *reply, const QByteArray data, const int statusCode);

public slots:
	void serviceRequestFinished(QNetworkReply *reply);

private:
	//void upload(const QString head, QJsonObject &obj, const QByteArray &data);

	QNetworkAccessManager m_networkManager;
	std::function<void(bool, QPixmap pixmap)> checkCallback;
//	std::function<void(bool, QJsonObject json)> uploadCallback;
};