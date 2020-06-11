#include "LivePortWidget.h"
#include "QPainter"

LivePortWidget::LivePortWidget()
{
	mBkMap = QPixmap(":/portBk");
	mPortIndex = QString("0");
}


LivePortWidget::~LivePortWidget()
{
}

void LivePortWidget::setIndex(QString index)
{
	mPortIndex = index;
}

void LivePortWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	
	float scale = width() / 5.0 / (mBkMap.width()*1.0);
	QRect rect(width() - mBkMap.width()*scale, height() - mBkMap.height()*scale, mBkMap.width()*scale, mBkMap.height()*scale);
	painter.drawPixmap(rect, mBkMap);

	painter.setPen(QColor(Qt::white));
	painter.setFont(QFont("Microsoft YaHei", 110*scale)); 
	painter.drawText(QPoint(rect.x() + rect.width()*0.5, rect.y()+rect.height()*0.85), mPortIndex);
	//painter.drawText(QRect(rect.x() + rect.width() / 2, rect.y() + rect.height() / 2, rect.width() / 2, rect.height() / 2), mPortIndex);

}