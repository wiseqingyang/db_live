#include "RecognizeThread.h"
#include "QStringList"
#include "QPixmap"
#include "FunctionDef.h"

RecognizeThread::RecognizeThread()
{
}


RecognizeThread::~RecognizeThread()
{
}

void RecognizeThread::setImagePath(QString filePath)
{
	this->imagePath = filePath;
}

void RecognizeThread::setTicketSizePos(QJsonArray sizeAndPos)
{
	mSizeAndPos = sizeAndPos;
}

void RecognizeThread::run()
{
	QString fileName = imagePath;
	QString newFileName = "";
	qDebug() << "receive msg from collection " << fileName;

	qDebug() << "mSizeAndPos:::   " << mSizeAndPos;
	//QSize sizeTab[] = { QSize(875, 440), QSize(875, 550), QSize(875, 875), QSize(875, 1300), QSize(875, 1740), QSize(875, 2185) };
	QPoint originPoint = QPoint(1934, 2592);
	QSize descSize = QSize(mSizeAndPos.at(0).toInt(), mSizeAndPos.at(1).toInt());

	QPixmap pic = QPixmap(fileName);
	QPixmap descPic = pic.copy(QRect(QPoint(originPoint.x() - descSize.width(), originPoint.y() - descSize.height()), descSize));

	newFileName = fileName.replace(".jpg", "_upload.jpg");
	descPic.save(newFileName, "JPG");
	emit updatePic("savePic", newFileName);

	QImage image1(newFileName);
	int posIndex = mSizeAndPos.at(2).toInt();
	QString str = decodeImageBottom(&image1, posIndex);
	qDebug() << "recognized :: " << str;
	emit onRecognized(str, newFileName);
}

QString RecognizeThread::decodeImageBottom(QImage* originImg, int posIndex)
{
	qDebug() << "recongnize tic " << originImg << " index " << posIndex;
	enum DecodeResult
	{
		ONE_29_31,		//ʶ���29λ�����ս��
		DOUBlE_18_11,	//ʶ���18λ��11λ�������
		HALF_18,		//ʶ���һ��18λ���
		HALF_11,		//ʶ���һ��11λ���
		NONE			//û�н��

	};

	DecodeResult resultState = NONE;
	float margins = 15;
	QImage img = originImg->copy(QRect(margins, margins, originImg->width() - margins * 2, originImg->height() - margins * 2));
	int w = img.width();
	int h = img.height();
	img.save("11.jpg");

	float discernW = 160;
	float discernH = discernW;

	//�����Ͻ�˳ʱ��
	QRect rect[] = {
		QRect(0, 0, discernW, discernH),
		QRect(w - discernW, 0, discernW, discernH),
		QRect(w - discernW, h - discernH, discernW, discernH),
		QRect(0, h - discernH, discernW, discernH),
	};

	QImage m = img.copy(rect[posIndex]);
	QImage *m1 = new QImage(m);
	m1 = lightness(100, m1);
	//m_Zxing.setDecoder(DecoderFormat_DATA_MATRIX);
	QString info = m_Zxing.decodeImage(*m1);
	
	m.save("qrcode.jpg");
	qDebug() << QString::fromLocal8Bit("��һ�� ʶ��λ�ã�") + posIndex + QString::fromLocal8Bit("ʶ������") + info;
	emit updatePic("qrcodePic", "qrcode.jpg");
	emit updatePic("qrcodePic1", "");
	delete m1;
	//m.save(name);

	if (info.length() == 0)
	{
		posIndex = posIndex == 2 ? 3 : 2;//Ϊ�˼��ݶ�ά�����2��λҲ��3��λ�����
		QImage m = img.copy(rect[posIndex]);
		QImage *m1 = new QImage(m);
		m1 = lightness(100, m1);
		//m_Zxing.setDecoder(DecoderFormat_DATA_MATRIX);
		info = m_Zxing.decodeImage(*m1);

		//m.save("qrcode.jpg");
		qDebug() << QString::fromLocal8Bit("�ڶ��� ʶ��λ�ã�") + posIndex + QString::fromLocal8Bit("ʶ������") + info;
		//emit updatePic("qrcodePic", "qrcode.jpg");
		delete m1;
	}

	if (info.length() > 0 ) {
		if (info.length() == 29 || info.length() == 31)
		{
			return info;
		}
		else {
			QRect secondQRrect;
			QRect tmpRect = rect[posIndex];

			if (tmpRect.x() == 0)
			{
				secondQRrect = QRect(tmpRect.x() + tmpRect.width()*0.8, tmpRect.y(), tmpRect.width(), tmpRect.height());
			}
			else
			{
				secondQRrect = QRect(tmpRect.x() - tmpRect.width()*0.8, tmpRect.y(), tmpRect.width(), tmpRect.height());
			}

			QString tmpInfo = info;
			QImage m = img.copy(secondQRrect);
			QImage *m1 = new QImage(m);
			m1 = lightness(100, m1);
			QString info2 = m_Zxing.decodeImage(*m1);

			m.save("qrcode1.jpg");
			emit updatePic("qrcodePic1", "qrcode1.jpg");
			delete m1;

			if (info2.length() > 0)
			{
				tmpInfo = QString("%1%2").arg(tmpInfo).arg(info2);
			}

			return tmpInfo;
			
		}
	}
	else {
		return "";
	}
}
