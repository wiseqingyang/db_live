#include "HttpUint.h"
#include "MessageDef.h"
#include "UserConfig.h"

HttpUint::HttpUint()
{
	QObject::connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));
//	m_certificatePath = QStringLiteral("Config/certificate.crt");
//	QList<QSslCertificate> certs = QSslCertificate::fromPath(certificatePath);
//	m_sslConfig.setProtocol(QSsl::TlsV1SslV3);
//	m_sslConfig.setCaCertificates(certs);
}


HttpUint::~HttpUint()
{
	m_networkManager.disconnect();
}


void HttpUint::get(QString head, QString content)
{
	QString domain = g_userConfig()->GetDomain();

	if (domain.length() == 0)
	{
		domain = g_domain;
	}

	QString url = domain + head + "?" + content;

	QNetworkRequest rq;

	QSslConfiguration conf = rq.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	conf.setProtocol(QSsl::TlsV1SslV3);
	rq.setSslConfiguration(conf);

	rq.setUrl(QUrl(url));
	m_networkManager.get(rq);

	qDebug() << "HttpUint..get url:" << url;
}

void HttpUint::post(const QString head, const QByteArray &data)
{
	QString domain = g_userConfig()->GetDomain();

	if (domain.length() == 0)
	{
		domain = g_domain;
	}

	QString url = domain + head + "?";

	QNetworkRequest rq;

	QSslConfiguration conf = rq.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	conf.setProtocol(QSsl::TlsV1SslV3);
	rq.setSslConfiguration(conf);

	rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	rq.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

	rq.setUrl(QUrl(url));

	m_networkManager.post(rq, data);

	qDebug() << "HttpUint..post url:" << url;
}

void HttpUint::serviceRequestFinished(QNetworkReply *reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	qDebug() << "HttpUint...serviceRequestFinished...statusCode:" << statusCode;

	if (reply->error() == QNetworkReply::NoError) {
		requestFinished(reply, reply->readAll(), statusCode);
	}
	else {
		requestFinished(reply, "", statusCode);
	}

	// At the end of that slot, we won't need it anymore
	reply->deleteLater();
}

void HttpUint::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
{
	if (statusCode == 200)
	{

		qDebug() << "HttpUint..requestFinished:" << QString::fromUtf8(data);

		QJsonParseError jsonError;
		QJsonObject json_object = QJsonDocument::fromJson(data, &jsonError).object();
		if (jsonError.error == QJsonParseError::NoError) {
			
			this->checkCallback(true, json_object);
		}
	}
	else
	{
		
		QJsonObject json;
		this->checkCallback(false, json);
	}
}

void HttpUint::getData(const QString head, QString content, std::function<void(bool, QJsonObject json)> callback)
{
	this->checkCallback = callback;
	get(head, content);
}

void HttpUint::postData(const QString head, const QByteArray &data, std::function<void(bool, QJsonObject json)> callback)
{
	this->checkCallback = callback;
	post(head, data);
}


void HttpUint::uploadImage(const QString head, QJsonObject &obj, const QByteArray &data, std::function<void(bool, QJsonObject json)> callback)
{
	this->checkCallback = callback;
	this->upload(head, obj, data);
}


void HttpUint::upload(const QString head, QJsonObject &obj, const QByteArray &data)
{

	QString imageUrl = g_userConfig()->GetImageUrl();

	if (imageUrl.length() == 0)
	{
		imageUrl = g_imageUrl;
	}

	QString url = head;
	if (!head.startsWith("http"))
	{
		url = imageUrl + head;
	}	

	QNetworkRequest rq;

	QSslConfiguration conf = rq.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	conf.setProtocol(QSsl::TlsV1SslV3);
	rq.setSslConfiguration(conf);

	rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	QString file_name = QString::number(obj["account"].toInt());
	QString orderid = obj["orderid"].toString();
	rq.setRawHeader("account", file_name.toLocal8Bit());
	rq.setRawHeader("orderid", orderid.toLocal8Bit());
	rq.setRawHeader("suffix", obj["suffix"].toString().toLocal8Bit());
	rq.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

	rq.setUrl(QUrl(url));

	m_networkManager.post(rq, data);

	qDebug() << "upload..post :" << url;
}