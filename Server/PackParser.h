#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QStringList>
#include <QCoreApplication>
#include <QProcess>
#include "linkPortServer.h"
#include "LPPack.h"
#include "logger.h"
#include "version.h"
#include "users.h"
#include "getConfig.h"
#include "CJsonObject/CJsonObject.hpp"
#include "tasks.h"
#include "singleManager.h"

class PackParser :
    public QObject
{
    Q_OBJECT
public:
    PackParser(QObject* parent = nullptr);

private:
    void parse(QString peeraddress, const JC::Net::Request& request);
    void send(QString peeraddress, const JC::Net::Answer& answer);

    void shutdown();
    void restart();

public slots:
    void dataIn(QString peeraddress, const QByteArray& data);
signals:
    void dataOut(QString peeraddress, const QByteArray& data);
};

