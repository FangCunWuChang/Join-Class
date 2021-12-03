#include "getConfig.h"

QMutex GetConfig::mutex;

neb::CJsonObject GetConfig::config()
{
	GetConfig::mutex.lock();
	neb::CJsonObject result;
	QFile file(QCoreApplication::applicationDirPath() + "/configs/config.json");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QByteArray data = file.readAll();
		file.close();

		result.Parse(QString::fromUtf8(data).toStdString());
	}
	GetConfig::mutex.unlock();
	return result;
}

bool GetConfig::setConfig(const neb::CJsonObject& config)
{
	GetConfig::mutex.lock();
	QDir dir(QCoreApplication::applicationDirPath() + "/configs");
	if (!dir.exists()) {
		dir.mkdir(QCoreApplication::applicationDirPath() + "/configs");
	}
	QFile file(QCoreApplication::applicationDirPath() + "/configs/config.json");
	if (file.exists()) {
		file.remove();
	}
	if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
		QString data = QString::fromStdString(config.ToFormattedString());
		file.write(data.toUtf8());
		file.close();
	}
	else {
		GetConfig::mutex.unlock();
		return false;
	}
	GetConfig::mutex.unlock();
	return true;
}

bool GetConfig::checkConfig()
{
	QFile file(QCoreApplication::applicationDirPath() + "/configs/config.json");
	return file.exists();
}