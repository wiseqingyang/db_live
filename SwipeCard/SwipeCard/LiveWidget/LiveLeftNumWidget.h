#pragma once
#include "QWidget"
class LiveLeftNumWidget :
	public QWidget
{
	Q_OBJECT;
public:
	LiveLeftNumWidget();
	~LiveLeftNumWidget();
	void setTotalNumAndLeftNum(int totalNum, int leftNum);

private:
	virtual void paintEvent(QPaintEvent *event);

private:
	int totalNum;
	int leftNum;
	QString drawStr;
};

