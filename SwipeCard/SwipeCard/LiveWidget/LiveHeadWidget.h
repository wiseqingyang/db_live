#pragma once
#include "qwidget.h"
class LiveHeadWidget :
	public QWidget
{
	Q_OBJECT
public:
	LiveHeadWidget();
	~LiveHeadWidget();

	void setUserInfo(QString userName, QPixmap headPix);
	void setUserName(QString userName);
protected:
	virtual void paintEvent(QPaintEvent *event);
private:
	QPixmap mBkFrame;
	QPixmap mHeadPix;
	QString mUserName;

	QPixmap PixmapToRound(const QPixmap &src, int radius);
};

