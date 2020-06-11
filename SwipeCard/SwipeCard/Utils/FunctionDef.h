#pragma once

#include <QString>
#include <comdef.h>
#include <GdiPlus.h>
#include <QImage>

extern int QStringToChar(char* target, QString& src);
extern int get_encoder_clsid(const WCHAR* format, CLSID* pClsid);
extern QImage *sharpen(QImage *origin);
extern QImage *saturation(int delta, QImage *origin);
extern QImage *lightness(int delta, QImage *origin);
extern QImage *greyScale(QImage * origin);
extern QImage *convertToBinary(int threshold, QImage *rgbImage);
extern void send_message(QJsonObject& obj);
extern void start_prcoess();
extern HWND ifWindowsExist();


const QString txt_information = QString::fromLocal8Bit("提示");


//高拍仪的sdk
//#include "eloamDll.h"
//#pragma comment(lib, "eloamDll.lib")