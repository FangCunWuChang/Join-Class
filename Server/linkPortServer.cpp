#include "linkPortServer.h"

LinkPortServer::LinkPortServer(QObject *parent)
	: QObject(parent)
{
}

LinkPortServer::~LinkPortServer()
{
	if (this->tcpServer != nullptr) {
		if (this->tcpServer->isListening()) {
		this->stoplisten();
		}
		this->tcpServer->deleteLater();
		Logger::log("LinkPort Server exit!");
	}
}

LinkPortServer LinkPortServer::server;

LinkPortServer& LinkPortServer::getClass()
{
	return LinkPortServer::server;
}

bool LinkPortServer::init(const neb::CJsonObject& config)
{
	quint32 port = 0, disconnectedTime = 0, recieveTime = 0;
	config.Get("localPort", port);
	config.Get("disconnectTimeOut", disconnectedTime);
	config.Get("receiveTimeOut", recieveTime);
	this->port = port;
	this->disconnectedWait = disconnectedTime;
	this->receiveWait = recieveTime;

	this->tcpServer = new QTcpServer(this);
	connect(this->tcpServer, &QTcpServer::newConnection, this, &LinkPortServer::onNewConnection);
	QString hostName = QHostInfo::localHostName();
	this->localIP = this->getLocalIP();
	Logger::log(QString::asprintf("Local host name:%s", qPrintable(hostName)));
	if (!this->localIP.isEmpty()) {
		Logger::log(QString::asprintf("Local host address:%s", qPrintable(this->localIP)));
		return true;
	}
	else {
		Logger::log(QString::asprintf("Error:Can't get local host address!"));
		return false;
	}

}

QString LinkPortServer::getLocalIP()
{
	QString hostName = QHostInfo::localHostName();
	QHostInfo hostInfo = QHostInfo::fromName(hostName);

	QString localIP = "";
	QList<QHostAddress> addList = hostInfo.addresses();

	if (!addList.isEmpty()) {
		for (int i = 0; i < addList.size(); i++) {
			QHostAddress aHost = addList.at(i);
			if (QAbstractSocket::IPv4Protocol == aHost.protocol()) {
				localIP = aHost.toString();
				break;
			}
		}
	}
	if (localIP.isEmpty()) {
		localIP = QHostAddress(QHostAddress::Any).toString();
	}
	return localIP;
}

void LinkPortServer::onNewConnection()
{
	this->currentClient = this->tcpServer->nextPendingConnection();
	tcpClient.append(this->currentClient);
	connect(this->currentClient, &QTcpSocket::readyRead, this, &LinkPortServer::onSocketReadyRead);
	connect(this->currentClient, &QTcpSocket::disconnected, this, &LinkPortServer::onClientDisconnected);
	connect(this->currentClient, &QTcpSocket::stateChanged, this, &LinkPortServer::onSocketStateChange);
	Logger::log(QString::asprintf("Connected %s:%d", qPrintable(this->currentClient->peerAddress().toString()), this->currentClient->peerPort()));
}

void LinkPortServer::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
	QTcpSocket* currentClient = (QTcpSocket*)sender();
	switch (socketState) {
	case QAbstractSocket::UnconnectedState:
		Logger::log(QString::asprintf("UnconnectedState %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		break;
	case QAbstractSocket::HostLookupState:
		Logger::log(QString::asprintf("HostLookupState %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		break;
	case QAbstractSocket::ConnectingState:
		Logger::log(QString::asprintf("ConnectingState %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		break;
	case QAbstractSocket::ConnectedState:
		Logger::log(QString::asprintf("ConnectedState %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		break;
	case QAbstractSocket::BoundState:
		Logger::log(QString::asprintf("BoundState %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		break;
	case QAbstractSocket::ClosingState:
		Logger::log(QString::asprintf("ClosingState %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		break;
	case QAbstractSocket::ListeningState:
		Logger::log(QString::asprintf("ListeningState %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		break;
	}
}

void LinkPortServer::onClientDisconnected()
{
	QTcpSocket* currentClient = (QTcpSocket*)sender();
	if (currentClient->state() == QAbstractSocket::UnconnectedState) {
		this->inTemp.remove(currentClient->peerAddress().toString());
		Logger::log(QString::asprintf("Disconnected %s:%d", qPrintable(currentClient->peerAddress().toString()), currentClient->peerPort()));
		currentClient->deleteLater();
		for (int i = 0; i < this->tcpClient.size(); i++) {
			if (this->tcpClient.at(i) == currentClient) {
				this->tcpClient.removeAt(i);
				break;
			}
		}
	}
}

void LinkPortServer::onSocketReadyRead()
{
	QTcpSocket* currentClient = (QTcpSocket*)sender();
	QString address = currentClient->peerAddress().toString();

	if (this->inTemp.contains(address)) {
		for (int i = 0; i < this->timers.size(); i++) {
			if (this->timers.at(i)->getName() == address) {
				this->timers.at(i)->reset();
				break;
			}
		}
		tempclass tc = this->inTemp[address];
		QByteArray data = currentClient->readAll();
		tc.buffer.append(data);
		this->inTemp.insert(address, tc);
		Logger::log("Get File Part:" + QString::number(data.size()));
	}
	else {

		TimerClock* timerclock = new TimerClock(this);
		timerclock->setup(address, this->receiveWait);
		connect(timerclock, &TimerClock::timeOut, this, &LinkPortServer::onTimerTimeOut);
		this->timers.append(timerclock);

		tempclass tc;

		quint64 bsize = currentClient->size();

		quint32 sizet = 0;
		currentClient->read((char*)&sizet, sizeof(sizet));
		tc.size = sizet;

		tc.buffer = currentClient->read(bsize - sizeof(quint32));

		Logger::log("Get Size Head:" + QString::number(tc.size));
		Logger::log("Get File Part:" + QString::number(tc.buffer.size()));
		this->inTemp.insert(address, tc);
	}


	if ((quint32)this->inTemp[address].buffer.size() == this->inTemp[address].size) {
		for (int i = 0; i < this->timers.size(); i++) {
			if (this->timers.at(i)->getName() == address) {
				this->timers.at(i)->stop();
				this->timers.at(i)->deleteLater();
				this->timers.removeAt(i);
				break;
			}
		}
		tempclass tc = this->inTemp[address];
		this->inTemp.remove(address);
		Logger::log(QString::asprintf("Get from %s:%d Size:%s", qPrintable(address), currentClient->peerPort(), qPrintable(QString::number(tc.size))));
		emit socketIn(address, tc.buffer);

	}
}

bool LinkPortServer::startlisten()
{
	QHostAddress addr(QHostAddress::Any);
	bool ok = this->tcpServer->listen(addr, this->port);
	if (ok) {
		Logger::log(QString::asprintf("Start listen... Address:%s Port:%s", qPrintable(addr.toString()), qPrintable(QString::number(this->port))));
		return true;
	}
	else {
		Logger::log(QString::asprintf("Error:Can't start listen... Address:%s Port:%s", qPrintable(addr.toString()), qPrintable(QString::number(this->port))));
		return false;
	}
}

void LinkPortServer::stoplisten()
{
	for (int i = 0; i < this->tcpClient.length(); i++) {
		this->tcpClient.at(i)->deleteLater();
	}
	this->tcpClient.clear();
	Logger::log(QString::asprintf("Stop listen... Address:%s Port:%s", qPrintable(this->tcpServer->serverAddress().toString()), qPrintable(QString::number(this->tcpServer->serverPort()))));
	this->tcpServer->close();
}

void LinkPortServer::onTimerTimeOut(QString name)
{
	Logger::log(QString::asprintf("Time out %s", qPrintable(name)));
	for (int i = 0; i < this->tcpClient.length(); i++) {
		if (this->tcpClient.at(i)->peerAddress().toString() == name) {
			this->tcpClient.at(i)->disconnectFromHost();
			bool ok = this->tcpClient.at(i)->waitForDisconnected(this->disconnectedWait);
			if (ok) {
				Logger::log(QString::asprintf("Stop listen %s", qPrintable(name)));
			}
			else {
				Logger::log(QString::asprintf("Can't stop listen %s", qPrintable(name)));
			}
			this->tcpClient.at(i)->deleteLater();
			this->tcpClient.removeAt(i);
			break;
		}
	}
	this->inTemp.remove(name);
	for (int i = 0; i < this->timers.size(); i++) {
		if (this->timers.at(i)->getName() == name) {
			this->timers.at(i)->deleteLater();
			this->timers.removeAt(i);
			break;
		}
	}
}

void LinkPortServer::socketOut(QString peeraddress, const QByteArray& data)
{
	bool ok = false;
	quint16 hostPort = 0;

	QByteArray buffer;
	quint32 size = data.size();
	buffer.append((char*)&size, sizeof(quint32));
	buffer.append(data);

	for (int i = 0; i < this->tcpClient.size(); i++) {
		if (this->tcpClient.at(i)->peerAddress().toString() == peeraddress) {
			QTcpSocket* currentClient = this->tcpClient.at(i);
			hostPort = currentClient->peerPort();
			QString address = currentClient->peerAddress().toString();
			if (currentClient->write(buffer) > -1) {
				this->bytesWrittenFlags.insert(address, true);
				connect(currentClient, &QTcpSocket::bytesWritten, this, &LinkPortServer::onSocketBytesWritten);
				QDateTime currentTime = QDateTime::currentDateTime();
				bool okf = true;
				while (!this->bytesWrittenFlags.value(address)) {
					QCoreApplication::processEvents();
					if (currentTime.msecsTo(QDateTime::currentDateTime()) > this->receiveWait) {
						okf = false;
						break;
					}
				}
				ok = okf;
				this->bytesWrittenFlags.remove(address);
			}
			break;
		}
	}
	if (ok) {
		Logger::log(QString::asprintf("Sent to %s:%d Size:%s", qPrintable(peeraddress), hostPort, qPrintable(QString::number(data.size()))));
	}
	else {
		Logger::log(QString::asprintf("Error:Can't send to %s:%d Size:%s", qPrintable(peeraddress), hostPort, qPrintable(QString::number(data.size()))));
	}
}

void LinkPortServer::onSocketBytesWritten(qint64 bytes)
{
	Q_UNUSED(bytes);
	QTcpSocket* currentClient = (QTcpSocket*)sender();
	QString address = currentClient->peerAddress().toString();
	this->bytesWrittenFlags.insert(address, true);
}

void LinkPortServer::wait()
{
	QDateTime currentTime = QDateTime::currentDateTime();
	while (currentTime.msecsTo(QDateTime::currentDateTime()) <= this->receiveWait) {
		QCoreApplication::processEvents();
	}
}