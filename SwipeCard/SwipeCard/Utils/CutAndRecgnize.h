#pragma once
#include "qobject.h"
#include "QJsonArray"
#include "Discern.h"
#include "qzxing.h"
#include "QVector"
#include "RecognizeThread.h"

class CutAndRecgnize :
	public QObject
{
	Q_OBJECT
public:
	CutAndRecgnize();
	~CutAndRecgnize();

	void setImagePath(QString imagePath);
	void setTicketSizePos(QJsonArray sizeAndPos);
	void recognizeCode();
	QString decodeImageBottom(QImage* originImg, int posIndex);
	void addWaterMark(QString imagePath, bool isPortrait);
	void onlyCutPic();

private slots:
	void onRecognizedCode(QString result, QString imagePath);
	void updatePicture(QString picType, QString imageName);

signals :
	void updatePic(QString picType, QString imageName);
	void onRecognizedTicketCode(QString result, QString imagePath);

private:
	QString mImagePath;
	QJsonArray mSizeAndPos;
	Discern mDis;
	RecognizeThread mRecognizeThread;

	QZXing m_Zxing;

	QVector<sRectInfo> m_RectInfo;     //±£¥Ê–≈œ¢
};

