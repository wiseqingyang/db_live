#pragma once
#include "qwidget.h"
class LiveTitleWidget :
	public QWidget
{
	Q_OBJECT
public:
	LiveTitleWidget();
	~LiveTitleWidget();
	virtual void paintEvent(QPaintEvent *event);
	void setIssue(QString issueStr);

private:
	QPixmap mBk;
	QPixmap mTitlePerson;
	QString mTitleStr;
};

