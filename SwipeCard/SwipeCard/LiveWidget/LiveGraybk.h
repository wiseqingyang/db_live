#pragma once
#include "qwidget.h"
class LiveGraybk :
	public QWidget
{
	Q_OBJECT
public:
	LiveGraybk();
	~LiveGraybk();


protected:
	virtual void paintEvent(QPaintEvent *event);

private:

};

