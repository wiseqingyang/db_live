#include "LiveGraybk.h"
#include "QPainter"

LiveGraybk::LiveGraybk() 
{
}

LiveGraybk::~LiveGraybk()
{
}

void LiveGraybk::paintEvent(QPaintEvent *)
{
	QColor bkColor(29, 28, 38, 255);
	QPainter painter(this);
	painter.setBrush(bkColor);
	//QRect rect(-width()*0.1, -height()*0.1, width()*1.1, height()*1.1);
	painter.drawRect(rect());	
}

