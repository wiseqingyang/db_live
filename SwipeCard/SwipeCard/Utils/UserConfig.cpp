#include "UserConfig.h"
#include "QJsonObject"
#include "QSettings"
#include "MessageDef.h"
#include "QJsonDocument"

UserConfig*  UserConfig::mInstance = NULL;

UserConfig* g_userConfig()
{
	return UserConfig::Instance();
}

UserConfig* UserConfig::Instance()
{
	if (mInstance == NULL)
	{
		mInstance = new UserConfig();
	}
	return mInstance;
}

UserConfig::UserConfig()
{
	m_account = 0;
	m_roomId = 0;
	m_seatIndex = 0;
	customWH = 0;
	m_strIniPath = "config.ini";
}   

UserConfig::~UserConfig()
{
}

void UserConfig::LoadConfigInfo(QJsonObject json)
{
	loadConfigInternal();

	m_account = json["account"].toInt();
	m_strUserName = json["name"].toString();
	m_strToken = json["token"].toString();

	//m_roomId = json["roomId"].toInt();
	m_seatIndex = json["seatIndex"].toInt();
	m_socketUrl = json["sokceturl"].toString();
	m_anchorid = json["anchorid"].toInt();

	mRoomIdArr = json["roomarr"].toArray();

	SaveConfig();
}

void UserConfig::SetMobile(QString mobile)
{
	m_strMobile = mobile;
	SaveConfig();
}

void UserConfig::SetCameraName(QString name)
{
	m_cameraName = name;
	SaveConfig();
}

void UserConfig::SetDomain(QString domain)
{
	m_domain = domain;
	SaveConfig();
}

void UserConfig::LoadConfig(void)
{
	loadConfigInternal();
}

void UserConfig::SetImgUrl(QString imgeUrl) 
{
	m_imageUrl = imgeUrl;
	SaveConfig();
}

void UserConfig::SetAnchorId(int anchorId)
{
	m_anchorid = anchorId;
}

void UserConfig::setCustomWH(int customWH)
{
	this->customWH = customWH;
	SaveConfig();
}

void UserConfig::SaveConfig(void)
{
	QSettings *configIni = new QSettings(m_strIniPath, QSettings::IniFormat);
	if (m_account == 0 || m_strUserName.isEmpty() )
	{
		return;
	}

	configIni->setValue("/custom/customWH", customWH);
	configIni->setValue("/sUserRecords/kUserId", m_account);
	configIni->setValue("/sUserRecords/kUserName", m_strUserName);
	configIni->setValue("/sUserRecords/kMobile", m_strMobile);
	//configIni->setValue("/sUserRecords/kRoomId", m_roomId);
	configIni->setValue("/sUserRecords/kSeatIndex", m_seatIndex);
	configIni->setValue("/sUserRecords/imageUrl", m_imageUrl);
	configIni->setValue("/sUserRecords/domain", m_domain);

	configIni->setValue("/unUseImage", "https://tcrel-pic.dbljoy.com");
	configIni->setValue("/unUseDomain", "https://tcrel.dbljoy.com");
	configIni->setValue("/unUse-test-image", "https://test-pic.dbljoy.com");
	configIni->setValue("/unUse-test-domain", "https://test.dbljoy.com");
	configIni->setValue("/unUse-dev-image", "https://dev-pic.dbljoy.com");
	configIni->setValue("/unUse-dev-domain", "https://dev.dbljoy.com");
	if (m_cameraName.size()>0)
	{
		configIni->setValue("/sUserRecords/kCameraName", m_cameraName);
	}

	delete configIni;
}

bool UserConfig::loadConfigInternal()
{
	QSettings *configIni = new QSettings(m_strIniPath, QSettings::IniFormat);
	if (configIni == nullptr)
	{
		return false;
	}

	int strUserId = configIni->value("/sUserRecords/kUserId").toInt();
	QString strUserName = configIni->value("/sUserRecords/kUserName").toString();
	QString strMobile = configIni->value("/sUserRecords/kMobile").toString();
	m_cameraName = configIni->value("/sUserRecords/kCameraName").toString();
	m_domain = configIni->value("/sUserRecords/domain").toString();
	m_imageUrl = configIni->value("/sUserRecords/imageUrl").toString();
	customWH = configIni->value("/custom/customWH").toInt();

	if (strUserId == 0 || strUserName.isEmpty() || strMobile.isEmpty())
	{
		return false;
	}

	m_account = strUserId;
	m_strUserName = strUserName;
	m_strMobile = strMobile;

	return true;
}
