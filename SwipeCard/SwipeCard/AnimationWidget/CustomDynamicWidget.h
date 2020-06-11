#ifndef CUSTOMDYNAMICWIDGET_H
#define CUSTOMDYNAMICWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QDebug>

/*
* 说明：
*   当前类是针对一张链式图片
*   链式图片默认为8帧，默认0.1秒播放一帧
*   可以根据自己需要进行设置与更改
*   进行自动切割，循环播放每一帧
*
* 示例：
*   类内声明 CustomDynamicWidget* dynamicAnima_
*   ui->setupUi(this);
*   dynamicAnima_ = new CustomDynamicWidget(this);
*   dynamicAnima_->setAnimation(QPixmap(":/res/loading.png"),8,100);
*   dynamicAnima_->setGeometry(100,100,300,300);
*   dynamicAnima_->show();
*   // 顺时针播放每帧
*   dynamicAnima_->startClockwise();
*/

class QTimer;
class CustomDynamicWidget : public QWidget
{
	Q_OBJECT
public:

	explicit CustomDynamicWidget(QWidget *parent = 0);
	/*
	* 设置动画图标
	* 函数名：setAnimation
	* 参数 _pix：图标实例
	* 参数 _count：图标实例动画帧数
	* 参数 _msec：动画切帧速度 (毫秒级)
	*/
	void setAnimation(const QPixmap& _pix, const short _count = 8, const int _msec = 100);

	/*  开始动画(顺时针)  */
	//void startClockwise(void);
	/*  开始动画(逆时针)  */
	//void startCounterclockwise(void);
	/*  停止动画  */
	//void stop(void);
	/*  设置动画无限循环  */
	void setLoop(const bool _isLoop = false) { isLoop_ = _isLoop; }

signals:

	/*  顺时针动画结束  */
	void clockwiseFinished(void);
	/*  逆时针动画结束  */
	void counterclockwiseFinished(void);

	private slots:

	/*  顺时针动画槽  */
	void updateClockwise(void);
	/*  逆时针动画槽  */
	//void updateCounterclockwise(void);

protected:

	void paintEvent(QPaintEvent *);

private:
	int mCount;
	/*  动画(是否无限循环)  */
	bool isLoop_;
	/*  图标列表数量  */
	short count_;
	/*  当前展示的图标下标  */
	short currentIndex_;
	/*  控制顺时针槽定时器  */
	QTimer *clockTimer_;
	/*  控制逆时针槽定时器  */
	QTimer *counterclockTimer_;
	/*  当前展示的图标  */
	QPixmap currentPix_;
	/*  图标列表  */
	QList<QPixmap> pixList_;

};

#endif // CUSTOMDYNAMICWIDGET_H