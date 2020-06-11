#include "LiveLeftNumWidget.h"
#include "QPainter"

LiveLeftNumWidget::LiveLeftNumWidget()
{
	totalNum = 1;
	leftNum = 1;
	drawStr = "1";
}


LiveLeftNumWidget::~LiveLeftNumWidget()
{
}

void LiveLeftNumWidget::setTotalNumAndLeftNum(int totalNum, int leftNum)
{
	this->totalNum = totalNum;
	this->leftNum = leftNum;
	if (totalNum == 1)
	{
		drawStr = "1";
	}
	else
	{
		drawStr = QString("%1/%2").arg(leftNum).arg(totalNum);
	}
}

void LiveLeftNumWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	QColor bkColor(0, 0, 0, 65);
	painter.setPen(bkColor);
	painter.setBrush(bkColor);
	painter.drawRect(0, height()-120, 230, 120);

	painter.setPen(Qt::white);
	painter.setFont(QFont("Microsoft YaHei", 50, QFont::Light));
	painter.drawText(QPoint(100-drawStr.length()*15, height()-30), drawStr);
	//	painter.drawText(QPoint(10, 32+20 * factor), mUserName);
}
