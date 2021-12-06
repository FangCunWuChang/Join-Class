#pragma once

#include <QFile>
#include <QDir>
#include <QString>
#include <QByteArray>
#include <QMutex>
#include <QCoreApplication>
#include "pb/pb.h"
#include "logger.h"
#include "rsasignature.h"
#include <QVector>

class Users final
{
public:
	static bool login(QString name, QByteArray passWord);
	static JC::User_Type getType(QString name);
	static bool find(QString name);

	static void init();

	static bool setUser(const JC::User& user);
	static bool setPass(const JC::User& user);

	static const QVector<JC::User> getUsers();

	static bool removeUser(QString name);

private:
	static QMutex mutex;
};

