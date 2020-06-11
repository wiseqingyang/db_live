#pragma once
#include "qwidget.h"
#include "QTimer"

struct EffectConf
{
	QString fileName;
	bool isLeft;
	float totalTime;//∫¡√Î
	float fromScale;
	float toScale;
};

struct DataConfig
{
	bool showing;
	QRect rect;
	QRect realRect;
	QPixmap drawPix;
	int effectIndex;
	float currScale;
	int timeIndex;
};


class LiveBigAward :
	public QWidget
{
	Q_OBJECT;
public:
	LiveBigAward();
	~LiveBigAward();
	void showBigAward(int bonus);
private:
	void paintEvent(QPaintEvent*);
	void updateAnimationData();
	QPixmap getPixmapWithRect(QRect &pixRect, QPixmap &pixmap);

	QVector<EffectConf> effConfigs[2];
	int mWinAmount;
	QTimer* mTimer;
	DataConfig mDataConfg;
};

