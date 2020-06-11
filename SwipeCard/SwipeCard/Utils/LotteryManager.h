#pragma once
#include "qobject.h"
#include "UserConfig.h"
#include "QJsonArray"
#include "HttpUint.h"

class LotteryManager :
	public QObject
{
	Q_OBJECT
public:
	LotteryManager();
	~LotteryManager();

	void updateTickets();
	void updateAllTickets();
	QJsonArray getTicketSizeAndCodePos(int ticketId);
	bool isWaterMarkPortrait(int ticketId);

private:
	QJsonArray m_TicketsArray;
	HttpUint m_Http;
};

