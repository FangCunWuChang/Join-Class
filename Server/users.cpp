#include "users.h"

QMutex Users::mutex;

bool Users::login(QString name, QByteArray passWord)
{
	Users::mutex.lock();
	QFile file(QCoreApplication::applicationDirPath() + "/users/" + name + ".dat");
	if (!file.exists()) {
		Users::mutex.unlock();
		return false;
	}
	if (!file.open(QIODevice::ReadOnly)) {
		Users::mutex.unlock();
		return false;
	}
	QByteArray data = file.readAll();
	file.close();

	Users::mutex.unlock();

	JC::User user;
	user.ParseFromArray(data.constData(), data.size());

	QByteArray pass = QString::fromStdString(user.password()).toUtf8();

	return pass == passWord;
}

JC::User_Type Users::getType(QString name)
{
	Users::mutex.lock();
	QFile file(QCoreApplication::applicationDirPath() + "/users/" + name + ".dat");
	if (!file.exists()) {
		Users::mutex.unlock();
		return JC::User_Type::User_Type_USER;
	}
	if (!file.open(QIODevice::ReadOnly)) {
		Users::mutex.unlock();
		return JC::User_Type::User_Type_USER;
	}
	QByteArray data = file.readAll();
	file.close();

	Users::mutex.unlock();

	JC::User user;
	user.ParseFromArray(data.constData(), data.size());

	return user.type();
}

bool Users::find(QString name)
{
	Users::mutex.lock();
	QFile file(QCoreApplication::applicationDirPath() + "/users/" + name + ".dat");
	if (!file.exists()) {
		Users::mutex.unlock();
		return false;
	}
	if (!file.open(QIODevice::ReadOnly)) {
		Users::mutex.unlock();
		return false;
	}
	QByteArray data = file.readAll();
	file.close();

	Users::mutex.unlock();

	JC::User user;
	user.ParseFromArray(data.constData(), data.size());

	return name == QString::fromStdString(user.id());
}

void Users::init()
{
	Users::mutex.lock();
	bool haveAdmin = false;
	QDir dir(QCoreApplication::applicationDirPath() + "/users");
	if (!dir.exists()) {
		dir.mkdir(QCoreApplication::applicationDirPath() + "/users");
		Logger::log("Created users directory");
	}
	else {
		dir.setFilter(QDir::Files);
		for (int i = 0; i < dir.count(); i++) {
			if (dir[i].endsWith(".dat")) {
				QFile file(QCoreApplication::applicationDirPath() + "/users/" + dir[i]);
				if (file.open(QIODevice::ReadOnly)) {
					QByteArray data = file.readAll();
					JC::User user;
					user.ParseFromArray(data.constData(), data.size());
					file.close();
					if (user.type() == JC::User_Type::User_Type_MANAGER) {
						haveAdmin = true;
						break;
					}
				}
			}
		}
	}
	if (!haveAdmin) {
		JC::User user;
		user.set_id("admin");
		user.set_type(JC::User_Type::User_Type_MANAGER);

		QByteArray passdata = QString("12345678").toUtf8();
		QByteArray passhash;
		RSASignature::hash512(passdata, passhash);
		user.set_password(QString::fromUtf8(passhash).toStdString());

		QByteArray data(user.ByteSize(), 0);
		user.SerializeToArray(data.data(), data.size());

		QFile file(QCoreApplication::applicationDirPath() + "/users/admin.dat");
		if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
			file.write(data);
			file.close();
			Logger::log("Created administrator user");
			Logger::log("id:admin");
			Logger::log("password:12345678");
		}
		else {
			Logger::log("Can't create administrator user");
		}
	}
	Users::mutex.unlock();
}

bool Users::setUser(const JC::User& user)
{
	Users::mutex.lock();
	QString name = QString::fromStdString(user.id());
	QFile file(QCoreApplication::applicationDirPath() + "/users/" + name + ".dat");
	if (file.exists()) {
		file.remove();
	}
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		Users::mutex.unlock();
		return false;
	}
	QByteArray data(user.ByteSize(), 0);
	user.SerializeToArray(data.data(), data.size());
	file.write(data);
	file.close();

	Users::mutex.unlock();
	return true;
}

bool Users::setPass(const JC::User& user)
{
	Users::mutex.lock();
	QString name = QString::fromStdString(user.id());
	QFile file(QCoreApplication::applicationDirPath() + "/users/" + name + ".dat");
	if (!file.exists()) {
		Users::mutex.unlock();
		return false;
	}
	if (!file.open(QIODevice::ReadOnly)) {
		Users::mutex.unlock();
		return false;
	}
	QByteArray dataCurrent = file.readAll();
	JC::User userCurrent;
	userCurrent.ParseFromArray(dataCurrent.constData(), dataCurrent.size());
	file.close();

	userCurrent.set_password(user.password());

	file.remove();
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		Users::mutex.unlock();
		return false;
	}
	QByteArray data(userCurrent.ByteSize(), 0);
	userCurrent.SerializeToArray(data.data(), data.size());
	file.write(data);
	file.close();

	Users::mutex.unlock();
	return true;
}

const QVector<JC::User> Users::getUsers()
{
	Users::mutex.lock();
	QVector<JC::User> result;
	QDir dir(QCoreApplication::applicationDirPath() + "/users");
	if (dir.exists()) {
		dir.setFilter(QDir::Files);
		for (int i = 0; i < dir.count(); i++) {
			if (dir[i].endsWith(".dat")) {
				QFile file(QCoreApplication::applicationDirPath() + "/users/" + dir[i]);
				if (file.open(QIODevice::ReadOnly)) {
					QByteArray data = file.readAll();
					JC::User user;
					user.ParseFromArray(data.constData(), data.size());
					result.append(user);
					file.close();
				}
			}
		}
	}
	Users::mutex.unlock();
	return result;
}

bool Users::removeUser(QString name)
{
	Users::mutex.lock();
	QFile file(QCoreApplication::applicationDirPath() + "/users/" + name + ".dat");
	if (file.exists()) {
		if (!file.remove()) {
			Users::mutex.unlock();
			return false;
		}
	}
	else {
		Users::mutex.unlock();
		return false;
	}
	Users::mutex.unlock();
	return true;
}