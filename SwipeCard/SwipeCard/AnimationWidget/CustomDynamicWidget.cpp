#include "CustomDynamicWidget.h"


CustomDynamicWidget::CustomDynamicWidget(QWidget *parent) :QWidget(parent), mCount(0), isLoop_(false)
{
}

/*
* 设置动画
* setAnimation
* _pix：图片
* _count：图片帧数
* _msec：切换速度 （毫秒 1秒 = 1000毫秒）
*/
void CustomDynamicWidget::setAnimation(const QPixmap &_pix, const short _count, const int _msec) 
{
	count_ = _count;
	currentIndex_ = 0;

	if (!pixList_.empty()) 
	{
		pixList_.clear();
	}
	else 
	{
		/*  顺时针动画关联  */
		clockTimer_ = new QTimer(this);
		clockTimer_->setInterval(_msec);
		connect(clockTimer_, &QTimer::timeout, this, &CustomDynamicWidget::updateClockwise);
		clockTimer_->start();

		/*  逆时针动画关联  */
		/*counterclockTimer_ = new QTimer(this);
		counterclockTimer_->setInterval(_msec);
		connect(counterclockTimer_, SIGNAL(timeout()), this, SLOT(updateCounterclockwise()));*/
	}

	/*  链式动画图标分离  */
	int num = _pix.width() / 1;
	count_ = num;
	for (short i = 0; i != num; ++i)
	{
		pixList_.append(_pix.copy(i * (_pix.width() / num), 0,
			_pix.width() / _count, _pix.height()));
	}

	currentPix_ = pixList_.at(0);
	this->setGeometry(0, 0, currentPix_.width(), currentPix_.height());

	update();

}

void CustomDynamicWidget::updateClockwise(void) {

	if (currentIndex_ < count_ && currentIndex_ >= 0) 
	{
		/*  更新帧  */
		currentPix_ = pixList_.at(currentIndex_);
		update();

		/*  判断帧数  */
		if (currentIndex_ >= (count_ - 1)) {
			if (isLoop_) {
				currentIndex_ = 0;
				return;
			}
		}

		/*  跳帧  */
		++currentIndex_;
	}
	else {
		clockTimer_->stop();
		currentIndex_ = 0;
		emit clockwiseFinished();
	}	
}

void CustomDynamicWidget::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	painter.drawPixmap(rect(), currentPix_);
}
