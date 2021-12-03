#include "getUser.h"

bool GetUser::check()
{
	QFile file(QCoreApplication::applicationDirPath() + "/users/user.json");
	return file.exists();
}

neb::CJsonObject GetUser::getUser()
{
	neb::CJsonObject user;
	QFile file(QCoreApplication::applicationDirPath() + "/users/user.json");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QByteArray data = file.readAll();
		file.close();

		user.Parse(QString::fromUtf8(data).toStdString());
	}
	return user;
}

bool GetUser::setUser(neb::CJsonObject& user)
{
	QDir dir(QCoreApplication::applicationDirPath() + "/users");
	if (!dir.exists()) {
		dir.mkdir(QCoreApplication::applicationDirPath() + "/users");
	}

	QFile file(QCoreApplication::applicationDirPath() + "/users/user.json");
	if (file.exists()) {
		if (!file.remove()) {
			return false;
		}
	}

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		return false;
	}

	QByteArray data = QString::fromStdString(user.ToFormattedString()).toUtf8();
	file.write(data);

	file.close();
	return true;
}
