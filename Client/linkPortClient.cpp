#include "linkPortClient.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

LinkPortClient::LinkPortClient(QObject *parent)
	: QObject(parent)
{
}

LinkPortClient::~LinkPortClient()
{
}

void LinkPortClient::init(neb::CJsonObject& sets)
{
    this->address = QString::fromStdString(sets("server"));
    sets.Get("port", this->port);
    sets.Get("connectTimeOut", this->connectedWait);
    sets.Get("receiveTimeOut", this->receiveWait);
}

bool LinkPortClient::send(const QByteArray& data)
{
    this->TCP_dataflag = false;
    this->result = QByteArray();
    this->TCP_backflag = false;
    this->TCP_timeout = false;
    this->inTemp = tempclass();
    this->tempOn = false;

    QTcpSocket socket;
    connect(&socket, &QTcpSocket::readyRead, this, &LinkPortClient::on_socket_readyRead);
    socket.connectToHost(this->address, this->port);
    if (!socket.waitForConnected(this->connectedWait)) {
        return false;
    }

    quint32 sizet = data.size();
    QByteArray temparray;
    temparray.append((char*)&sizet, sizeof(quint32));
    temparray.append(data);

    socket.write(temparray);

    qDebug(QString::asprintf("Sent to:%s:%d Size:%s", qPrintable(this->address), this->port, qPrintable(QString::number(data.size()))).toStdString().c_str());

    this->currentTime = QTime::currentTime();
    while (!this->TCP_backflag) {
        QCoreApplication::processEvents();
        if (this->currentTime.msecsTo(QTime::currentTime()) > this->receiveWait) {
            this->TCP_timeout = true;
            break;
        }
    }

    if (this->TCP_timeout) {
        socket.disconnectFromHost();
        disconnect(&socket, &QTcpSocket::readyRead, this, &LinkPortClient::on_socket_readyRead);
        return false;
    }

    if (!this->TCP_dataflag) {
        socket.disconnectFromHost();
        disconnect(&socket, &QTcpSocket::readyRead, this, &LinkPortClient::on_socket_readyRead);
        return false;
    }
    socket.disconnectFromHost();
    disconnect(&socket, &QTcpSocket::readyRead, this, &LinkPortClient::on_socket_readyRead);
    return true;
}

void LinkPortClient::on_socket_readyRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString address = socket->peerAddress().toString();


    if (this->tempOn) {
        QByteArray data = socket->readAll();
        this->inTemp.buffer.append(data);
        this->currentTime = QTime::currentTime();
        qDebug(QString("Get File Part:" + QString::number(data.size())).toStdString().c_str());
    }
    else {
        quint64 bsize = socket->size();

        quint32 sizet = 0;
        socket->read((char*)&sizet, sizeof(sizet));
        this->inTemp.size = sizet;

        this->inTemp.buffer = socket->read(bsize - sizeof(quint32));

        this->tempOn = true;
        this->currentTime = QTime::currentTime();

        qDebug(QString("Get Size Head:" + QString::number(this->inTemp.size)).toStdString().c_str());
        qDebug(QString("Get File Part:" + QString::number(this->inTemp.buffer.size())).toStdString().c_str());
    }


    if ((quint32)this->inTemp.buffer.size() == this->inTemp.size) {
        qDebug(QString(QString::asprintf("Get from:%s:%d Size:%s", qPrintable(address), socket->peerPort(), qPrintable(QString::number(inTemp.size)))).toStdString().c_str());
        this->TCP_dataflag = true;
        this->result = inTemp.buffer;
        this->TCP_backflag = true;
    }
}

const QByteArray& LinkPortClient::getResult()
{
    return this->result;
}