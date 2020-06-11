#include "swipecard.h"
#include <QtWidgets/QApplication>
#include "LoginDialog.h"
#include "QDir"

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();

	QString text;
	switch (type)
	{
	case QtDebugMsg:
		text = QString("Debug:");
		break;

	case QtWarningMsg:
		text = QString("Warning:");
		break;

	case QtCriticalMsg:
		text = QString("Critical:");
		break;

	case QtFatalMsg:
		text = QString("Fatal:");
	}

	QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

	static QString fileName = QString("./log/") + QDateTime::currentDateTime().toString("MMddhhmmss") + ".txt";

	/*QFile file(fileName);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();*/
	int length = fileName.length();
	bool isLegal = false;
	if (length > 0 && length < 500 )
	{
		isLegal = true;
	}
	
	if (isLegal)
	{
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
		{ 
			mutex.unlock();
			return;
		}
		else
		{
			QTextStream text_stream(&file);
			text_stream << message << "\r\n";
			file.flush();
			file.close();
		}	
	}
	mutex.unlock();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//注册MessageHandler  
	qInstallMessageHandler(outputMessage);

	//打印日志到文件中  
	qDebug("This is a debug message");
	LoginDialog l;	

	if (l.exec() == QDialog::Accepted)
	{
		SwipeCard w;
		w.show();
		w.connectSocket();
		return a.exec();
	}
	return 0;
}
