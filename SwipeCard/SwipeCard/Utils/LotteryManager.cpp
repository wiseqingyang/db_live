#include "LotteryManager.h"
#include "QMessageBox"
#include "QUrlQuery"

LotteryManager::LotteryManager()
{
}


LotteryManager::~LotteryManager()
{
}

void LotteryManager::updateAllTickets()
{
	QUrlQuery params;

	m_Http.getData("/liveroom/getAllTickets", params.toString().toUtf8(), [&](bool success, QJsonObject json){
		if (success)
		{
			int error = json["errcode"].toInt();
			if (error == 0)
			{
				m_TicketsArray = json["ticketlist"].toArray();
			}
			else
			{
				QMessageBox::warning(NULL, "warning", json["errmsg"].toString(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			}
		}
	});
}

void LotteryManager::updateTickets()
{
	QUrlQuery params;
	//params.addQueryItem("token", g_userConfig()->GetToken());
	//params.addQueryItem("account", QString::number(g_userConfig()->GetAccount()));
	params.addQueryItem("anchorid", QString::number(g_userConfig()->GetAnchorId()));

	m_Http.getData("/mul/getLotteryTicketList", params.toString().toUtf8(), [&](bool success, QJsonObject json){
		if (success)
		{
			qDebug() <<"=====¡·"<< json;
			int error = json["errcode"].toInt();
			if (error == 0)
			{
				m_TicketsArray = json["lottery_ticket_list"].toArray();
			}
			else
			{
				QMessageBox::warning(NULL, "warning", json["errmsg"].toString(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			}
		}
	});
}

QJsonArray LotteryManager::getTicketSizeAndCodePos(int ticketId)
{	
	for (int i = 0; i < m_TicketsArray.count(); i++)
	{
		QJsonObject info = m_TicketsArray[i].toObject();
		if (ticketId == info["ticket_id"].toInt())
		{
			QString obj = info["ticket_size"].toString();
			QJsonDocument jsonDocument = QJsonDocument::fromJson(obj.toLocal8Bit().data());

			QJsonArray arr = jsonDocument.array();
			qDebug() << ticketId << "===========================>" << arr;
			return arr;
		}
	}
	return QJsonArray();
}

bool LotteryManager::isWaterMarkPortrait(int ticketId)
{
	for (int i = 0; i < m_TicketsArray.count(); i++)
	{
		QJsonObject info = m_TicketsArray[i].toObject();
		if (ticketId == info["ticket_id"].toInt())
		{
			int isPortrait = info["isvertical"].toInt();
			
			return isPortrait && true;
		}
	}
	return false;
}
