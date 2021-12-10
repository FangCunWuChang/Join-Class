#pragma once

#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QByteArray>
#include <QMutex>
#include <QDir>
#include "pb/pb.h"
#include "QStringList"

class Tasks final
{
public:
	static JC::Task getTask();
	static bool setTask(const JC::Task& task);

	static JC::Student getStudent(QString id);
	static bool setClasses(QString id, QStringList classes);
private:
	static QMutex mutex;
};

