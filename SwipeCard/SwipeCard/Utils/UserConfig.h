#pragma once
#include "qobject.h"
#include "QJsonArray"

typedef struct resolutionSIZE
{
	qlonglong cx;
	qlonglong cy;
}Size;

class UserConfig :
	public QObject
{
	Q_OBJECT
public:
	static UserConfig* Instance();
	UserConfig();
	~UserConfig();

	QString GetToken(){ return m_strToken; }
	QString GetMobile(){ return m_strMobile; }
	QString GetScoket(){ return m_socketUrl; }
	QString GetCameraName(){ return m_cameraName; }
	QJsonArray GetRoomArray(){ return mRoomIdArr; }
	int GetAccount(){ return m_account; }
	//int GetRoomId(){ return m_roomId; }
	int GetSeatIndex(){ return m_seatIndex; }
	int GetAnchorId() { return m_anchorid; }
	int getCustomWH() { return customWH; }
	QString GetDomain() { return m_domain; }
	QString GetImageUrl() { return m_imageUrl; }

	void LoadConfigInfo(QJsonObject json);
	void LoadConfig(void);
	void SaveConfig(void);
	void SetMobile(QString mobile);
	void SetCameraName(QString name);
	void SetDomain(QString domain);
	void SetImgUrl(QString domain);
	void SetAnchorId(int anchorId);
	void setCustomWH(int customWH);

private:
	static UserConfig* mInstance;

	QString m_strIniPath;

	int m_account;
	QString m_strUserName;
	QString m_strMobile;
	QString m_liveRoomName;   //直播房间名字..  

	QString m_socketUrl;
	QString m_strToken;
	QString m_cameraName;
	QString m_domain;
	QString m_imageUrl;
	int m_anchorid;
	int m_roomId;
	int m_seatIndex;
	int customWH;
	QJsonArray mRoomIdArr;

	bool loadConfigInternal();
};
extern UserConfig* g_userConfig();

