#ifndef MESSAGEDEF_H
#define MESSAGEDEF_H


#include <QJsonObject>
#include <QJsonArray>
#include <QKeyEvent>
#include <qDebug>
#include <QMessageBox>
//#include "CustomEvent.h"

//#define TEST_LOTTERY

#ifdef TEST_LOTTERY
const QString g_domain = "http://test-lottery.dbljoy.com/";
const QString g_imageUrl = "https://test-tc.dbljoy.com/";
#else
const QString g_domain = "https://lottery.dbljoy.com/";
const QString g_imageUrl = "https://tc.dbljoy.com/";
#endif


//自定义事件
const QEvent::Type CustomEvent_upload_image = (QEvent::Type)5001;
const QEvent::Type CustomEvent_Lottery = (QEvent::Type)5002;

#endif