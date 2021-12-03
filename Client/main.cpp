#include <QtWidgets/QApplication>
#include <QDateTime>
#include <QMessageBox>
#include "version.h"
#include "singleManager.h"
#include "getConfig.h"
#include "SetConfig.h"
#include "Login.h"
#include "UserWindow.h"
#include "ManagerWindow.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#define log(s) qDebug(qPrintable(s))
#define Logger

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SingleManager::getClass().initSema(::Key_Sema);
    SingleManager::getClass().lock();

    SingleManager::getClass().initSM(::Key_SM);

    bool runFlag = false;
    if (SingleManager::getClass().check()) {
        runFlag = true;
    }
    else {
        if (!SingleManager::getClass().start()) {
            QMessageBox::warning(nullptr, "出错", "无法建立共享内存");
            return 2;
        }
    }

    SingleManager::getClass().unlock();

    if (!runFlag) {
        //Logger::push();
        Logger::log(QString::asprintf("Join Classes Client %.1f", ::_JC_Version));
        Logger::log(QString::asprintf("Start Time:%s", qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))));
        Logger::log(QString::asprintf(" "));

        SetConfig setconfig;
        setconfig.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
        Login login;
        login.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
        ManagerWindow mw;
        UserWindow uw;
        mw.setWindowState(Qt::WindowMaximized);
        uw.setWindowState(Qt::WindowMaximized);

        auto connectMan = [&login, &setconfig]()->void {
            QObject::disconnect(&setconfig, &SetConfig::configOK, &login, &Login::show);
            QObject::connect(&setconfig, &SetConfig::closed, &login, &Login::show); 
        };

        QObject::connect(&login, &Login::initConfig, &setconfig, &SetConfig::init);
        QObject::connect(&login, &Login::wannaConfig, &setconfig, &SetConfig::show);
        QObject::connect(&setconfig, &SetConfig::configData, &login, &Login::init);
        QObject::connect(&login, &Login::initWindow, &mw, &ManagerWindow::init);
        QObject::connect(&login, &Login::initWindow, &uw, &UserWindow::init);
        QObject::connect(&login, &Login::showManager, &mw, &ManagerWindow::show);
        QObject::connect(&login, &Login::showUser, &uw, &UserWindow::show);
        QObject::connect(&mw, &ManagerWindow::initConfig, &setconfig, &SetConfig::init);
        QObject::connect(&mw, &ManagerWindow::showConfig, &setconfig, &SetConfig::show);
        QObject::connect(&login, &Login::showManager, connectMan);
        
        if (GetConfig::checkConfig()) {
            QObject::connect(&setconfig, &SetConfig::closed, &login, &Login::show);
            neb::CJsonObject config = GetConfig::config();
            login.init(config);
            login.show();
        }
        else {
            QObject::connect(&setconfig, &SetConfig::configOK, &login, &Login::show);
            setconfig.show();
        }

        int stopCode = a.exec();
       
        Logger::log("Application Finished!");
        return stopCode;
    }
    else {
        QMessageBox::warning(nullptr, "出错", "程序已经在运行");
        return 1;
    }
}
