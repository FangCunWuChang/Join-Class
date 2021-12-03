#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QTime>
#include <QCoreApplication>
#include <QHash>
#include <QHostAddress>
#include <QTimer>
#include "CJsonObject/CJsonObject.hpp"

class LinkPortClient : public QObject
{
	Q_OBJECT

public:
	LinkPortClient(QObject *parent = Q_NULLPTR);
	~LinkPortClient();

    void init(neb::CJsonObject& sets);
    bool send(const QByteArray& data);
    const QByteArray& getResult();
private:
    QString address = "127.0.0.1";
    quint32 port = 12345;
    quint32 receiveWait = 3000;
    quint32 connectedWait = 1000;

    bool TCP_dataflag = false;
    bool TCP_backflag = false;
    bool TCP_timeout = false;
    QByteArray result;

    struct tempclass {
        quint32 size = 0;
        QByteArray buffer;
    };

	bool tempOn = false;
    tempclass inTemp;
	QTime currentTime;
signals:

private slots:
    void on_socket_readyRead();
};
