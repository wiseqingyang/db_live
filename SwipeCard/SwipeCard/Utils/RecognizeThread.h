#pragma once
#include "QThread"
#include "Discern.h"
#include "CutPicture.h"
#include "QJsonArray"
#include "qzxing.h"

class RecognizeThread :
	public QThread
{
	Q_OBJECT;
public:
	RecognizeThread();
	~RecognizeThread();
	void setImagePath(QString filePath);
	void setTicketSizePos(QJsonArray sizeAndPos);

signals:
	void onRecognized(QString result, QString imagePath);
	void cutResult(QString filePath);
	void updatePic(QString picType, QString imageName);

protected:
	void run();

private:
	QString imagePath;	
	QJsonArray mSizeAndPos;
	Discern mDis;
	CutPicture mCutPic;
	QZXing m_Zxing;
	QVector<sRectInfo> m_RectInfo;     //±£¥Ê–≈œ¢

	QString decodeImageBottom(QImage* originImg, int posIndex);
};

