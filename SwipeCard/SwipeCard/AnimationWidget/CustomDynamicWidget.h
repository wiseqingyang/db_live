#ifndef CUSTOMDYNAMICWIDGET_H
#define CUSTOMDYNAMICWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QDebug>

/*
* ˵����
*   ��ǰ�������һ����ʽͼƬ
*   ��ʽͼƬĬ��Ϊ8֡��Ĭ��0.1�벥��һ֡
*   ���Ը����Լ���Ҫ�������������
*   �����Զ��иѭ������ÿһ֡
*
* ʾ����
*   �������� CustomDynamicWidget* dynamicAnima_
*   ui->setupUi(this);
*   dynamicAnima_ = new CustomDynamicWidget(this);
*   dynamicAnima_->setAnimation(QPixmap(":/res/loading.png"),8,100);
*   dynamicAnima_->setGeometry(100,100,300,300);
*   dynamicAnima_->show();
*   // ˳ʱ�벥��ÿ֡
*   dynamicAnima_->startClockwise();
*/

class QTimer;
class CustomDynamicWidget : public QWidget
{
	Q_OBJECT
public:

	explicit CustomDynamicWidget(QWidget *parent = 0);
	/*
	* ���ö���ͼ��
	* ��������setAnimation
	* ���� _pix��ͼ��ʵ��
	* ���� _count��ͼ��ʵ������֡��
	* ���� _msec��������֡�ٶ� (���뼶)
	*/
	void setAnimation(const QPixmap& _pix, const short _count = 8, const int _msec = 100);

	/*  ��ʼ����(˳ʱ��)  */
	//void startClockwise(void);
	/*  ��ʼ����(��ʱ��)  */
	//void startCounterclockwise(void);
	/*  ֹͣ����  */
	//void stop(void);
	/*  ���ö�������ѭ��  */
	void setLoop(const bool _isLoop = false) { isLoop_ = _isLoop; }

signals:

	/*  ˳ʱ�붯������  */
	void clockwiseFinished(void);
	/*  ��ʱ�붯������  */
	void counterclockwiseFinished(void);

	private slots:

	/*  ˳ʱ�붯����  */
	void updateClockwise(void);
	/*  ��ʱ�붯����  */
	//void updateCounterclockwise(void);

protected:

	void paintEvent(QPaintEvent *);

private:
	int mCount;
	/*  ����(�Ƿ�����ѭ��)  */
	bool isLoop_;
	/*  ͼ���б�����  */
	short count_;
	/*  ��ǰչʾ��ͼ���±�  */
	short currentIndex_;
	/*  ����˳ʱ��۶�ʱ��  */
	QTimer *clockTimer_;
	/*  ������ʱ��۶�ʱ��  */
	QTimer *counterclockTimer_;
	/*  ��ǰչʾ��ͼ��  */
	QPixmap currentPix_;
	/*  ͼ���б�  */
	QList<QPixmap> pixList_;

};

#endif // CUSTOMDYNAMICWIDGET_H