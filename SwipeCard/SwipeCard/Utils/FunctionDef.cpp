#include "FunctionDef.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QProcess>
#include <QDir>
#include "QDebug"


int QStringToChar(char* target, QString& src)
{
	QByteArray ba = src.toLocal8Bit();
	int len = ba.size();
	char *msg = ba.data();

	strncpy(target, msg, len);

	return len;
}

int get_encoder_clsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders  
	UINT  size = 0;         // size of the image encoder array in bytes  

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure  

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure  

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success  
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure  
}

//��
QImage *sharpen(QImage * origin)
{
	QImage * newImage = new QImage(*origin);

	int kernel[3][3] = { { 0, -1, 0 },
	{ -1, 5, -1 },
	{ 0, -1, 0 } };
	int kernelSize = 3;
	int sumKernel = 1;
	int r, g, b;
	QColor color;

	for (int x = kernelSize / 2; x<newImage->width() - (kernelSize / 2); x++){
		for (int y = kernelSize / 2; y<newImage->height() - (kernelSize / 2); y++){

			r = 0;
			g = 0;
			b = 0;

			for (int i = -kernelSize / 2; i <= kernelSize / 2; i++){
				for (int j = -kernelSize / 2; j <= kernelSize / 2; j++){
					color = QColor(origin->pixel(x + i, y + j));
					r += color.red()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
					g += color.green()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
					b += color.blue()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
				}
			}

			r = qBound(0, r / sumKernel, 255);
			g = qBound(0, g / sumKernel, 255);
			b = qBound(0, b / sumKernel, 255);

			newImage->setPixel(x, y, qRgb(r, g, b));

		}
	}
	return newImage;
}

//ģ��
QImage *blur(QImage * origin)
{
	QImage * newImage = new QImage(*origin);
		
	int kernel[5][5] = { { 0, 0, 1, 0, 0 },
	{ 0, 1, 3, 1, 0 },
	{ 1, 3, 7, 3, 1 },
	{ 0, 1, 3, 1, 0 },
	{ 0, 0, 1, 0, 0 } };
	int kernelSize = 5;
	int sumKernel = 27;
	int r, g, b;
	QColor color;

	for (int x = kernelSize / 2; x<newImage->width() - (kernelSize / 2); x++){
		for (int y = kernelSize / 2; y<newImage->height() - (kernelSize / 2); y++){

			r = 0;
			g = 0;
			b = 0;

			for (int i = -kernelSize / 2; i <= kernelSize / 2; i++){
				for (int j = -kernelSize / 2; j <= kernelSize / 2; j++){
					color = QColor(origin->pixel(x + i, y + j));
					r += color.red()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
					g += color.green()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
					b += color.blue()*kernel[kernelSize / 2 + i][kernelSize / 2 + j];
				}
			}

			r = qBound(0, r / sumKernel, 255);
			g = qBound(0, g / sumKernel, 255);
			b = qBound(0, b / sumKernel, 255);

			newImage->setPixel(x, y, qRgb(r, g, b));

		}
	}
	return newImage;
}


/*
��ɫ������ͨ����ɣ��죬�̣�����������ˣ�RGB����Ψһһ����ʾɫ�ʵķ�ʽ�����������ʹ��HSL��ʽ��ʾɫ�� - hue��ɫ�ࣩ, saturation�����Ͷȣ�, lightness�����ȣ���
���͵�ͼ��ӵ�и�����������ɫ��ͨ����ȽϺÿ���������һ��Ҫ��ס����Ҫ���ñ��Ͷȣ���Ϊ�����׳���ʧ�档
*/
//���Ͷ�
QImage * saturation(int delta, QImage * origin){
	QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

	QColor oldColor;
	QColor newColor;
	int h, s, l;

	for (int x = 0; x<newImage->width(); x++){
		for (int y = 0; y<newImage->height(); y++){
			oldColor = QColor(origin->pixel(x, y));

			newColor = oldColor.toHsl();
			h = newColor.hue();
			s = newColor.saturation() + delta;
			l = newColor.lightness();

			//we check if the new value is between 0 and 255  
			s = qBound(0, s, 255);

			newColor.setHsl(h, s, l);

			newImage->setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
		}
	}

	return newImage;
}

QImage *lightness(int delta, QImage * origin){
	QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

	QColor oldColor;
	QColor newColor;
	int h, s, l;

	for (int x = 0; x < newImage->width(); x++){
		for (int y = 0; y < newImage->height(); y++){
			oldColor = QColor(origin->pixel(x, y));

			newColor = oldColor.toHsl();
			h = newColor.hue();
			s = newColor.saturation();
			l = newColor.lightness() + delta;

			//we check if the new value is between 0 and 255  
			l = qBound(0, l, 255);

			newColor.setHsl(h, s, l);

			newImage->setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
		}
	}

	return newImage;
}

QImage *convertToBinary(int threshold, QImage *rgbImage)
{
	QSize size = rgbImage->size();
	QImage *newImage = new QImage(size, QImage::Format_RGB32);
	int width = size.width();
	int height = size.width();
	for (int i = 0; i < size.width(); i++)
	for (int j = 0; j < size.height(); j++)
	{
		QRgb pixel = rgbImage->pixel(i, j);
		int r = qRed(pixel) * 0.3;
		int g = qGreen(pixel) * 0.59;
		int b = qBlue(pixel) * 0.11;
		int rgb = r + g + b;//�Ȱ�ͼ��ҶȻ���ת��Ϊ�Ҷ�ͼ��
		if (rgb > threshold)//Ȼ��ĳһ��ֵ���ж�ֵ��
		{
			rgb = 255;
		}
		else
		{
			rgb = 0;
		}
		QRgb newPixel = qRgb(rgb, rgb, rgb);
		newImage->setPixel(i, j, newPixel);
	}

	return newImage;
}


//��ɫ��
QImage * cool(int delta, QImage * origin){
	QImage *newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

	QColor oldColor;
	int r, g, b;

	for (int x = 0; x<newImage->width(); x++){
		for (int y = 0; y<newImage->height(); y++){
			oldColor = QColor(origin->pixel(x, y));

			r = oldColor.red();
			g = oldColor.green();
			b = oldColor.blue() + delta;

			//we check if the new value is between 0 and 255  
			b = qBound(0, b, 255);

			newImage->setPixel(x, y, qRgb(r, g, b));
		}
	}

	return newImage;
}

//ůɫ��
QImage *warm(int delta, QImage * origin){
	QImage *newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

	QColor oldColor;
	int r, g, b;

	for (int x = 0; x<newImage->width(); x++){
		for (int y = 0; y<newImage->height(); y++){
			oldColor = QColor(origin->pixel(x, y));

			r = oldColor.red() + delta;
			g = oldColor.green() + delta;
			b = oldColor.blue();

			//we check if the new values are between 0 and 255  
			r = qBound(0, r, 255);
			g = qBound(0, g, 255);

			newImage->setPixel(x, y, qRgb(r, g, b));
		}
	}

	return newImage;
}

//�Ҷ�ͼ
QImage * greyScale(QImage * origin){
	QImage * newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

	QColor oldColor;

	for (int x = 0; x<newImage->width(); x++){
		for (int y = 0; y<newImage->height(); y++){
			oldColor = QColor(origin->pixel(x, y));
			int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
			newImage->setPixel(x, y, qRgb(average, average, average));
		}
	}

	return newImage;
}


QProcess g_Prcoess;
void send_message(QJsonObject& obj)
{
	HWND hWnd = ::FindWindowW(NULL, L"InformationCollection");
	if (hWnd == NULL)
	{
		start_prcoess();
	}

	QString qstrSend = QString(QJsonDocument(obj).toJson());
	std::string strData = qstrSend.toStdString();

	COPYDATASTRUCT cpd = { 0 };
	cpd.cbData = strData.size();
	cpd.lpData = (PVOID)strData.data();
	::SendMessageW(hWnd, WM_COPYDATA, NULL, (LPARAM)&cpd);
}

HWND ifWindowsExist()
{
	HWND hWnd = ::FindWindowW(NULL, L"InformationCollection");
	if (hWnd == NULL)
	{
		start_prcoess();
	}

	return hWnd;
}

void start_prcoess()
{
	QString fullPath = QDir::currentPath() + "/infoCollection/InformationCollection.exe";
	qDebug() << "curr fullPath " << fullPath;
	g_Prcoess.start(fullPath);
}