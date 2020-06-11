#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QtWidgets/QDialog>
#include "ui_LoginDialog.h"
#include "Utils/HttpUint.h"
#include "QStringListModel"
#include "QListView"


class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = 0);
	~LoginDialog();


public Q_SLOTS:
	void accept();
	void OnCancel();
private:

	Ui::LoginDialog ui;

	HttpUint m_Http;
	QStringListModel *mCamerasModel;
	QStringList mCameras;
	QListView *mListView;
};

#endif // LOGINDIALOG_H
