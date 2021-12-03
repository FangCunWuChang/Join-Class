#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QCoreApplication>
#include <QTimer>
#include "logger.h"
#include <QHash>
#include <QByteArray>
#include "CJsonObject/CJsonObject.hpp"
#include <QVector>
#include <QMap>

class TimerClock : public QObject {
	Q_OBJECT
public:
	TimerClock(QObject* parent = Q_NULLPTR) : QObject(parent)
	{
		connect(&this->timer, &QTimer::timeout, this, &TimerClock::onTimeOut);
	}

	void setup(QString name, quint32 timeout)
	{
		this->name = name;
		this->timer.start(timeout);
	}

	QString getName()
	{
		return this->name;
	}

	void reset()
	{
		this->timer.stop();
		this->timer.start();
	}

	void stop()
	{
		this->timer.stop();
	}
private:
	QTimer timer;
	QString name;
private slots:
	void onTimeOut()
	{
		this->timer.stop();
		emit this->timeOut(this->name);
	}
signals:
	void timeOut(QString name);
};

class LinkPortServer : public QObject
{
	Q_OBJECT

	LinkPortServer(QObject *parent = Q_NULLPTR);
	static LinkPortServer server;
public:
	~LinkPortServer();
	static LinkPortServer& getClass();

	bool init(const neb::CJsonObject& config);

	bool startlisten();
	void stoplisten();

	void wait();

private:
	QString getLocalIP();

	QString localIP;
	quint32 port = 12345;

	quint32 disconnectedWait = 1000;
	quint32 receiveWait = 3000;

	QTcpServer* tcpServer = nullptr;
	QList<QTcpSocket*> tcpClient;
	QTcpSocket* currentClient = nullptr;

	struct tempclass {
		quint32 size = 0;
		QByteArray buffer;
	};

	QHash<QString, tempclass> inTemp;
	QVector<TimerClock*> timers;

	QMap<QString,bool> bytesWrittenFlags;

private slots:
	void onNewConnection();
	void onSocketStateChange(QAbstractSocket::SocketState socketState);
	void onClientDisconnected();
	void onSocketReadyRead();
	void onTimerTimeOut(QString name);
	void onSocketBytesWritten(qint64 bytes);
public slots:
	void socketOut(QString peeraddress, const QByteArray& data);
signals:
	void socketIn(QString peeraddress, const QByteArray& data);
};
