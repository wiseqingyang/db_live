#pragma once
#include <QPushButton>
#include <QIcon>
#include <QPixmap>

class ImageButton :
	public QPushButton
{
public:
	ImageButton(QWidget * parent);
	~ImageButton();

	void setButtonIcon(const QString& iconFileName, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off);

private:
	QString iconFilePath;
};

