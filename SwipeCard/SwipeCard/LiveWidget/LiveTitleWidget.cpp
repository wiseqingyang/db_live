#include "LiveTitleWidget.h"
#include "QPainter"

LiveTitleWidget::LiveTitleWidget()
{
	mBk = QPixmap(":/titleBk");
	mTitlePerson = QPixmap(":/titlePerson");
	mTitleStr = QString::fromLocal8Bit("");
}

void LiveTitleWidget::setIssue(QString issueStr)
{
	mTitleStr = issueStr + QString::fromLocal8Bit("ÆÚ");
}


LiveTitleWidget::~LiveTitleWidget()
{
}

void LiveTitleWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	float scale = height() * 1.0 / mBk.height() / 10;
	painter.drawPixmap(QRect(0, 0, width(), mBk.height()*scale), mBk);

	float rate = mBk.height()*1.0 / mTitlePerson.height()*0.8;
	//QRect rect = QRect(0.7*width(), mBk.height()*scale*0.1, mTitlePerson.width()*rate*scale, mTitlePerson.height()*rate*scale);
	//painter.drawPixmap(rect, mTitlePerson);

	QRect rect = QRect(0.65*width(), mBk.height()*scale*0.15, mTitlePerson.width()*rate*scale, mTitlePerson.height()*rate*scale);

	painter.setPen(Qt::white);
	painter.setFont(QFont("Times", 120 * scale, QFont::Bold));
	painter.drawText(QPoint(rect.x(), rect.center().y()+35*scale), mTitleStr);
}
