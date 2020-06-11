#include "LiveHeadWidget.h"
#include "QPainter"
#include "QBitmap"
#include "QLabel"

LiveHeadWidget::LiveHeadWidget()
{
	mBkFrame = QPixmap(":/headFrame");
	mUserName = QString::fromLocal8Bit("ÓÃ»§Ãû×Ö");
	mHeadPix = QPixmap(":/defaultHead");
	mHeadPix = PixmapToRound(mHeadPix, mHeadPix.width() / 2);
}


LiveHeadWidget::~LiveHeadWidget()
{
}

QPixmap LiveHeadWidget::PixmapToRound(const QPixmap &src, int radius)
{
	if (src.isNull()) {
		return QPixmap();
	}

	QSize size(2 * radius, 2 * radius);
	QBitmap mask(size);
	QPainter painter(&mask);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
	painter.setBrush(QColor(0, 0, 0));
	painter.drawRoundedRect(0, 0, size.width(), size.height(), size.width()/2, size.height()/2);

	QPixmap image = src.scaled(size);
	image.setMask(mask);
	return image;
}

void LiveHeadWidget::setUserInfo(QString userName, QPixmap headPix)
{
	if (!headPix.isNull())
	{
		mHeadPix = headPix;
		mHeadPix = PixmapToRound(mHeadPix, mHeadPix.width() / 2);
	}
	if (userName.length() > 5)
	{
		userName = userName.left(5) + QString::fromLocal8Bit("¡­");
	}
	mUserName = userName;
}

void LiveHeadWidget::setUserName(QString userName)
{
//	mHeadPix = QPixmap(":/defaultHead");
//	mHeadPix = PixmapToRound(mHeadPix, mHeadPix.width() / 2);

	mUserName = userName;
}
void LiveHeadWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	float factor = width()*1.0 / mBkFrame.width()*0.6;
	QRect bkRect = QRect(0, 0, mBkFrame.width()*factor, mBkFrame.height()*factor);
	painter.drawPixmap(bkRect, mBkFrame);

	QRect headRect = QRect(10.0, 0, bkRect.height() * 1.2, bkRect.height() * 1.2);
	painter.drawPixmap(headRect, mHeadPix);

	painter.setPen(Qt::yellow);
	painter.setFont(QFont("Microsoft YaHei", 80 * factor, QFont::Light));

	painter.drawText(QPoint(headRect.right() + 30, headRect.center().y() + 20 * factor), mUserName);
// 	QRect textRect = QRect(30,30, bkRect.right() - headRect.right() - 60, headRect.height()*0.4);
// 	painter.drawText(textRect, mUserName);
}
