#include "logger.h"

QMutex Logger::logLocker;

void Logger::log(QString text)
{
    qInfo("%s",qPrintable(text));
    text.replace('\n',' ');
    Logger::logLocker.lock();
    QFile file(QCoreApplication::applicationDirPath()+"/logs/JC_Server_latest.log");
    if(file.open(QIODevice::ReadWrite|QIODevice::Text)){
        file.seek(file.size());
        file.write(QString("["+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"]"+text+"\n").toUtf8());
        file.close();
    }
    Logger::logLocker.unlock();
}

void Logger::clean()
{
    Logger::logLocker.lock();
    QFile file(QCoreApplication::applicationDirPath()+"/logs/JC_Server_latest.log");
    if(file.exists()){
        file.remove();
    }
    Logger::logLocker.unlock();
}

void Logger::push()
{
    Logger::logLocker.lock();

    QDir dir(QCoreApplication::applicationDirPath() + "/logs");
    if (!dir.exists()) {
        dir.mkdir(QCoreApplication::applicationDirPath() + "/logs");
    }

    QFile file(QCoreApplication::applicationDirPath()+"/logs/JC_Server_latest.log");
    if(file.exists()){
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QString data=file.readAll();
            file.close();

            int left=data.indexOf('['),right=data.indexOf(']');
            QString dateline=data.mid(left+1,right-left-1);
            dateline.replace(" ","_");
            dateline.replace(":","_");
            dateline.replace("-","_");
            if(!file.copy(QCoreApplication::applicationDirPath()+"/logs/JC_Server_"+dateline+".log")){
                qInfo("!!!%s",qPrintable(QCoreApplication::applicationDirPath()+"/logs/JC_Server_"+dateline+".log"));
            }
        }
        file.remove();
    }
    Logger::logLocker.unlock();
}

QStringList Logger::getList()
{
    Logger::logLocker.lock();
    QStringList result;

    QDir dir(QCoreApplication::applicationDirPath()+"/logs");
    dir.setFilter(QDir::Files);

    for (int i = 0; i < dir.count(); i++) {
        QString filename = dir[i];
        if (filename.startsWith("JC_Server_") && filename.endsWith(".log")) {
            result.append(filename);
        }
    }

    Logger::logLocker.unlock();
    return result;
}

QString Logger::getData(QString name)
{
    Logger::logLocker.lock();
    QString data;

    QFile file(QCoreApplication::applicationDirPath() + "/logs/" + name);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        data = QString::fromUtf8(file.readAll());
        file.close();
    }

    Logger::logLocker.unlock();
    return data;
}

bool Logger::clearLogs()
{
    Logger::logLocker.lock();
    QDir dir(QCoreApplication::applicationDirPath() + "/logs");
    dir.setFilter(QDir::Files);
    bool okF = true;
    for (int i = 0; i < dir.count(); i++) {
        if (dir[i].startsWith("JC_Server_") && dir[i].endsWith(".log")) {
            if (dir[i] != "JC_Server_latest.log") {
                QFile file(QCoreApplication::applicationDirPath() + "/logs/" + dir[i]);
                okF = okF && file.remove();
            }
        }
    }
    Logger::logLocker.unlock();
    return okF;
}