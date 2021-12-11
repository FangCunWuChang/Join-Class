#include <QtCore/QCoreApplication>
#include "logger.h"
#include "version.h"
#include "getConfig.h"
#include "CJsonObject/CJsonObject.hpp"
#include "singleManager.h"
#include "linkPortServer.h"
#include "PackParser.h"
#include <QObject>
#include "users.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SingleManager::getClass().initSema(::Key_Sema);
    SingleManager::getClass().lock();

    SingleManager::getClass().initSM(::Key_SM);

    bool runFlag = false;
    if (SingleManager::getClass().check()) {
        runFlag = true;
    }
    else {
        if (!SingleManager::getClass().start()) {
            qInfo("Can't create shared memory!");
            return 2;
        }
    }

    SingleManager::getClass().unlock();

    if (!runFlag) {
        Logger::push();
        Logger::log(QString::asprintf("Join Classes Server Service %.1f", ::_JC_Version));
        Logger::log(QString::asprintf("Start Time:%s", qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))));
        Logger::log(QString::asprintf(" "));

        neb::CJsonObject configs = GetConfig::config();

        PackParser parser;

        QObject::connect(&parser, &PackParser::dataOut, &(LinkPortServer::getClass()), &LinkPortServer::socketOut);
        QObject::connect(&(LinkPortServer::getClass()), &LinkPortServer::socketIn, &parser, &PackParser::dataIn);

        if (!LinkPortServer::getClass().init(configs)) {
            Logger::log("Can't Initialize LinkPort Server!");
            return 3;
        }
        if (!LinkPortServer::getClass().startlisten()) {
            Logger::log("Can't Start LinkPort Server!");
            return 4;
        }

        Users::init();

        int stopCode = a.exec();
        Logger::log("Application Finished!");
        return stopCode;
    }
    else {
        qInfo("Application is already running!");
        return 1;
    }
}
