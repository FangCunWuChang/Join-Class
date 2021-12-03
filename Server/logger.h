#pragma once

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QCoreApplication>
#include <QMutex>
#include <QStringList>
#include <QDir>

class Logger
{
public:
    static void log(QString text);
    static void clean();
    static void push();
    static QStringList getList();
    static QString getData(QString name);
    static bool clearLogs();
private:
    static QMutex logLocker;
};
