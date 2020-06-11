#pragma once
#include "qwidget.h"
#include "QLabel"
#include "QImage"
class LiveFreeWidget :
	public QWidget
{
	Q_OBJECT
public:
	LiveFreeWidget();
	~LiveFreeWidget();
	QImage ScaleImage2Label(QImage qImage, QLabel* qLabel);

	virtual void paintEvent(QPaintEvent *event);

private:
	QPixmap mPixmap;
};

