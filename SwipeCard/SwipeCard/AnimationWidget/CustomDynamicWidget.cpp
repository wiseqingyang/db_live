#include "CustomDynamicWidget.h"


CustomDynamicWidget::CustomDynamicWidget(QWidget *parent) :QWidget(parent), mCount(0), isLoop_(false)
{
}

/*
* ���ö���
* setAnimation
* _pix��ͼƬ
* _count��ͼƬ֡��
* _msec���л��ٶ� ������ 1�� = 1000���룩
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
		/*  ˳ʱ�붯������  */
		clockTimer_ = new QTimer(this);
		clockTimer_->setInterval(_msec);
		connect(clockTimer_, &QTimer::timeout, this, &CustomDynamicWidget::updateClockwise);
		clockTimer_->start();

		/*  ��ʱ�붯������  */
		/*counterclockTimer_ = new QTimer(this);
		counterclockTimer_->setInterval(_msec);
		connect(counterclockTimer_, SIGNAL(timeout()), this, SLOT(updateCounterclockwise()));*/
	}

	/*  ��ʽ����ͼ�����  */
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
		/*  ����֡  */
		currentPix_ = pixList_.at(currentIndex_);
		update();

		/*  �ж�֡��  */
		if (currentIndex_ >= (count_ - 1)) {
			if (isLoop_) {
				currentIndex_ = 0;
				return;
			}
		}

		/*  ��֡  */
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
