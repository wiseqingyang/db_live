#include "LoginDialog.h"
#include <QUrlQuery>
#include <QDir>
#include "UserConfig.h"
#include "QDebug"
#include "noFocusFrameDelegate.h"
#include "QCameraInfo"

LoginDialog::LoginDialog(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.m_OK, &QPushButton::clicked, this, &LoginDialog::accept);
	connect(ui.m_Cancel, &QPushButton::clicked, this, &LoginDialog::OnCancel);

	g_userConfig()->LoadConfig();
	ui.mobile->setText(g_userConfig()->GetMobile());
	QString cameraName = g_userConfig()->GetCameraName();
	int index = 0;

	QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	foreach(const QCameraInfo &cameraInfo, cameras) {
		mCameras.push_back(cameraInfo.description());
		if (cameraName.compare(cameraInfo.description()) == 0)
		{
			index = cameras.indexOf(cameraInfo);
		}
	}

	mCamerasModel = new QStringListModel(this);
	mCamerasModel->setStringList(mCameras);
	mListView = new QListView(this);
	ui.comboBox->setView(mListView);
	ui.comboBox->setModel(mCamerasModel);
	ui.comboBox->setItemDelegate(new NoFocusFrameDelegate(this));
	ui.comboBox->setCurrentIndex(index);
}

LoginDialog::~LoginDialog()
{
//	delete m_webSocket;
}

void LoginDialog::OnCancel()
{
	this->done(QDialog::Rejected);
	this->close();
}

void LoginDialog::accept()
{	
	QString moblie = ui.mobile->text();
	QString passwd = ui.passwd->text();

	if (moblie.isEmpty() || passwd.isEmpty())
	{
		QMessageBox::warning(NULL, "warning", QString::fromLocal8Bit("输入信息不完整！！"), QMessageBox::Yes);
		return;
	}	

	ui.m_OK->setEnabled(false);

	QUrlQuery params;
	params.addQueryItem("mobile", moblie);
	params.addQueryItem("password", passwd);

	m_Http.postData("/mul/login", params.toString().toLocal8Bit(), [&](bool success, QJsonObject json)
	{
		ui.m_OK->setEnabled(true);
		if (success)
		{
			int error = json["errcode"].toInt();
			if (error == 0)
			{
				qDebug() << "login ret json:" << json;
				int cameraIndex = ui.comboBox->currentIndex();
				g_userConfig()->LoadConfigInfo(json);
				g_userConfig()->SetMobile(ui.mobile->text());
				g_userConfig()->SetCameraName(mCameras.at(cameraIndex));
				this->done(QDialog::Accepted);
				this->close();
			}
			else
			{
				QMessageBox::warning(NULL, "warning", QString::fromLocal8Bit("密码错误！！"), QMessageBox::Yes);
			}
		}
		else
		{
			QMessageBox::warning(NULL, "warning", QString::fromLocal8Bit("未知错误"), QMessageBox::Yes);
		}
	});
}