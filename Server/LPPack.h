#pragma once

#include "logger.h"
#include "pb/pb.h"
#include "rsasignature.h"
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QCoreApplication>
#include <QMutex>

class LPPack final
{
public:
	static bool packRequestWithPrivate(JC::Net::Request& data, QByteArray& pack);
	static bool unpackRequestWithPrivate(QByteArray& pack, JC::Net::Request& data);
	static bool packRequestWithPubilc(JC::Net::Request& data, QByteArray& pack);
	static bool unpackRequestWithPubilc(QByteArray& pack, JC::Net::Request& data);

	static bool packAnswerWithPrivate(JC::Net::Answer& data, QByteArray& pack);
	static bool unpackAnswerWithPrivate(QByteArray& pack, JC::Net::Answer& data);
	static bool packAnswerWithPubilc(JC::Net::Answer& data, QByteArray& pack);
	static bool unpackAnswerWithPubilc(QByteArray& pack, JC::Net::Answer& data);

private:
	static QString getPrivateKey();
	static QString getPublicKey();

	static QMutex priMutex;
	static QMutex pubMutex;
};

