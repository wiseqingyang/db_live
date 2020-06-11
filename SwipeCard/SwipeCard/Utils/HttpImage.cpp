
#include "HttpImage.h"
//#include "MessageDef.h"


HttpImage::~HttpImage()
{
}

HttpImage::HttpImage()
{
	QObject::connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));
}

void HttpImage::getData(const QString url, std::function<void(bool, QPixmap pixmap)> callback)
{

	this->checkCallback = callback;

	QNetworkRequest rq;

	QSslConfiguration conf = rq.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	conf.setProtocol(QSsl::TlsV1SslV3);

	rq.setSslConfiguration(conf);

	QString uurl_tmp(url);
	if (!url.startsWith("http"))
		uurl_tmp = "http://" + url;
	
	rq.setUrl(QUrl(uurl_tmp));
	m_networkManager.get(rq);

	qDebug() << "HttpUint..get url:" << uurl_tmp;
}


void HttpImage::serviceRequestFinished(QNetworkReply *reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	qDebug() << "HttpImage...serviceRequestFinished...statusCode:" << statusCode;

	if (reply->error() == QNetworkReply::NoError) {
		requestFinished(reply, reply->readAll(), statusCode);
	}
	else {
		requestFinished(reply, "", statusCode);
	}

	// At the end of that slot, we won't need it anymore
	reply->deleteLater();
}

void HttpImage::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
{
	if (statusCode == 200)
	{
		QPixmap pixmap;
		pixmap.loadFromData(data);
		this->checkCallback(true, pixmap);
	}
	else
	{
		QPixmap pixmap;
		this->checkCallback(false, pixmap);
	}
}

//
//
//void HttpImage::uploadImage(const QString head, QJsonObject &obj, const QByteArray &data, std::function<void(bool, QJsonObject json)> callback)
//{
//	this->checkCallback = callback;
//	this->upload(head, obj, data);
//}
//
//
//void HttpImage::upload(const QString head, QJsonObject &obj, const QByteArray &data)
//{
//	QString url = g_domain + head;
//
//	QNetworkRequest rq;
//	rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
//	QString file_name = QString::number(obj["account"].toInt());
//	QString orderId = obj["orderId"].toString();
//	rq.setRawHeader("account", file_name.toLocal8Bit());
//	rq.setRawHeader("orderid", orderId.toLocal8Bit());
//	rq.setRawHeader("suffix", obj["suffix"].toString().toLocal8Bit());
//	rq.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
//
//	rq.setUrl(QUrl(url));
//
//	m_networkManager.post(rq, data);
//
//	qDebug() << "upload..post :" << url;
//}
