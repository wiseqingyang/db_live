#include "LiveFreeWidget.h"
#include "QLabel"
#include "QLayout"
#include "QPainter"

LiveFreeWidget::LiveFreeWidget()
{
	mPixmap = QPixmap(":/showFree");	
}


LiveFreeWidget::~LiveFreeWidget()
{
}

QImage LiveFreeWidget::ScaleImage2Label(QImage qImage, QLabel* qLabel)
{
	QImage qScaledImage;
	QSize qImageSize = qImage.size();
	QSize qLabelSize = qLabel->size();
	double dWidthRatio = 1.0*qImageSize.width() / qLabelSize.width();
	double dHeightRatio = 1.0*qImageSize.height() / qLabelSize.height();
	if (dWidthRatio > dHeightRatio)
	{
		qScaledImage = qImage.scaledToWidth(qLabelSize.width());
	}
	else
	{
		qScaledImage = qImage.scaledToHeight(qLabelSize.height());
	}
	return qScaledImage;
}

void LiveFreeWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	float rate = rect().width() *1.0 / mPixmap.width();
	QRect destRect = QRect(0, (height() - mPixmap.height()*rate) / 2.0, width(), mPixmap.height()*rate);
	painter.drawPixmap(destRect, mPixmap);
}
