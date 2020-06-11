#pragma once
#include "QWidget"
#include "QPixmap"


class LivePortWidget :
	public QWidget
{
	Q_OBJECT
public:
	LivePortWidget();
	~LivePortWidget();
	void setIndex(QString index);

protected:
	virtual void paintEvent(QPaintEvent *event);
private:
	QPixmap mBkMap;
	QString mPortIndex;
};

