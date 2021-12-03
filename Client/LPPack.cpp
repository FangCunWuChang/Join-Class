#include "LPPack.h"

QMutex LPPack::priMutex;

QMutex LPPack::pubMutex;

#define log(s) qDebug(qPrintable(s))
#define Logger

bool LPPack::packRequestWithPrivate(JC::Net::Request& data, QByteArray& pack)
{
	QString key = LPPack::getPrivateKey();
	QByteArray dataArray(data.ByteSize(), 0);
	data.SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Pack Request");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPEncrypted;
	if (RSASignature::private_encrypt(fingerPrint, key, fPEncrypted) <= 0) {
		Logger::log("Encrypt error");
		return false;
	}

	JC::Net::RequestPack protoPack;
	protoPack.mutable_pack()->CopyFrom(data);
	protoPack.mutable_signature()->append(QString::fromUtf8(fPEncrypted).toStdString());

	pack.resize(protoPack.ByteSize());
	protoPack.SerializeToArray(pack.data(), pack.size());
	Logger::log("Done");
	return true;
}

bool LPPack::unpackRequestWithPrivate(QByteArray& pack, JC::Net::Request& data)
{
	QString key = LPPack::getPrivateKey();
	JC::Net::RequestPack protoPack;
	protoPack.ParseFromArray(pack.constData(), pack.size());

	QByteArray fPEncrypted = QString::fromStdString(protoPack.signature()).toUtf8();
	QByteArray dataArray(protoPack.pack().ByteSize(), 0);
	protoPack.pack().SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Unpack Request");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPDecrypted;
	if (RSASignature::private_decrypt(fPEncrypted, key, fPDecrypted) <= 0) {
		Logger::log("Decrypt error");
		return false;
	}

	Logger::log(QString::asprintf("Signature:%s", qPrintable(QString::fromUtf8(fPDecrypted))));

	if (fPDecrypted != fingerPrint) {
		Logger::log("Signature mismatch");
		return false;
	}

	data.CopyFrom(protoPack.pack());
	Logger::log("Done");
	return true;
}

bool LPPack::packRequestWithPubilc(JC::Net::Request& data, QByteArray& pack)
{
	QString key = LPPack::getPublicKey();
	QByteArray dataArray(data.ByteSize(), 0);
	data.SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Pack Request");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPEncrypted;
	if (RSASignature::public_encrypt(fingerPrint, key, fPEncrypted) <= 0) {
		Logger::log("Encrypt error");
		return false;
	}

	JC::Net::RequestPack protoPack;
	protoPack.mutable_pack()->CopyFrom(data);
	protoPack.mutable_signature()->append(QString::fromUtf8(fPEncrypted).toStdString());

	pack.resize(protoPack.ByteSize());
	protoPack.SerializeToArray(pack.data(), pack.size());
	Logger::log("Done");
	return true;
}

bool LPPack::unpackRequestWithPubilc(QByteArray& pack, JC::Net::Request& data)
{
	QString key = LPPack::getPublicKey();
	JC::Net::RequestPack protoPack;
	protoPack.ParseFromArray(pack.constData(), pack.size());

	QByteArray fPEncrypted = QString::fromStdString(protoPack.signature()).toUtf8();
	QByteArray dataArray(protoPack.pack().ByteSize(), 0);
	protoPack.pack().SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Unpack Request");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPDecrypted;
	if (RSASignature::public_decrypt(fPEncrypted, key, fPDecrypted) <= 0) {
		Logger::log("Decrypt error");
		return false;
	}

	Logger::log(QString::asprintf("Signature:%s", qPrintable(QString::fromUtf8(fPDecrypted))));

	if (fPDecrypted != fingerPrint) {
		Logger::log("Signature mismatch");
		return false;
	}

	data.CopyFrom(protoPack.pack());
	Logger::log("Done");
	return true;
}

bool LPPack::packAnswerWithPrivate(JC::Net::Answer& data, QByteArray& pack)
{
	QString key = LPPack::getPrivateKey();
	QByteArray dataArray(data.ByteSize(), 0);
	data.SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Pack Answer");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPEncrypted;
	if (RSASignature::private_encrypt(fingerPrint, key, fPEncrypted) <= 0) {
		Logger::log("Encrypt error");
		return false;
	}

	JC::Net::AnswerPack protoPack;
	protoPack.mutable_pack()->CopyFrom(data);
	protoPack.mutable_signature()->append(QString::fromUtf8(fPEncrypted).toStdString());

	pack.resize(protoPack.ByteSize());
	protoPack.SerializeToArray(pack.data(), pack.size());
	Logger::log("Done");
	return true;
}

bool LPPack::unpackAnswerWithPrivate(QByteArray& pack, JC::Net::Answer& data)
{
	QString key = LPPack::getPrivateKey();
	JC::Net::AnswerPack protoPack;
	protoPack.ParseFromArray(pack.constData(), pack.size());

	QByteArray fPEncrypted = QString::fromStdString(protoPack.signature()).toUtf8();
	QByteArray dataArray(protoPack.pack().ByteSize(), 0);
	protoPack.pack().SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Unpack Answer");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPDecrypted;
	if (RSASignature::private_decrypt(fPEncrypted, key, fPDecrypted) <= 0) {
		Logger::log("Decrypt error");
		return false;
	}

	Logger::log(QString::asprintf("Signature:%s", qPrintable(QString::fromUtf8(fPDecrypted))));

	if (fPDecrypted != fingerPrint) {
		Logger::log("Signature mismatch");
		return false;
	}

	data.CopyFrom(protoPack.pack());
	Logger::log("Done");
	return true;
}

bool LPPack::packAnswerWithPubilc(JC::Net::Answer& data, QByteArray& pack)
{
	QString key = LPPack::getPublicKey();
	QByteArray dataArray(data.ByteSize(), 0);
	data.SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Pack Answer");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPEncrypted;
	if (RSASignature::public_encrypt(fingerPrint, key, fPEncrypted) <= 0) {
		Logger::log("Encrypt error");
		return false;
	}

	JC::Net::AnswerPack protoPack;
	protoPack.mutable_pack()->CopyFrom(data);
	protoPack.mutable_signature()->append(QString::fromUtf8(fPEncrypted).toStdString());

	pack.resize(protoPack.ByteSize());
	protoPack.SerializeToArray(pack.data(), pack.size());
	Logger::log("Done");
	return true;
}

bool LPPack::unpackAnswerWithPubilc(QByteArray& pack, JC::Net::Answer& data)
{
	QString key = LPPack::getPublicKey();
	JC::Net::AnswerPack protoPack;
	protoPack.ParseFromArray(pack.constData(), pack.size());

	QByteArray fPEncrypted = QString::fromStdString(protoPack.signature()).toUtf8();
	QByteArray dataArray(protoPack.pack().ByteSize(), 0);
	protoPack.pack().SerializeToArray(dataArray.data(), dataArray.size());

	QByteArray fingerPrint;
	RSASignature::hash512(dataArray, fingerPrint);

	Logger::log("Unpack Answer");
	Logger::log(QString::asprintf("Data length:%d", dataArray.size()));
	Logger::log(QString::asprintf("Finger print:%s", qPrintable(QString::fromUtf8(fingerPrint))));

	QByteArray fPDecrypted;
	if (RSASignature::public_decrypt(fPEncrypted, key, fPDecrypted) <= 0) {
		Logger::log("Decrypt error");
		return false;
	}

	Logger::log(QString::asprintf("Signature:%s", qPrintable(QString::fromUtf8(fPDecrypted))));

	if (fPDecrypted != fingerPrint) {
		Logger::log("Signature mismatch");
		return false;
	}

	data.CopyFrom(protoPack.pack());
	Logger::log("Done");
	return true;
}

QString LPPack::getPrivateKey()
{
	LPPack::priMutex.lock();
	QString key;
	QFile file(QCoreApplication::applicationDirPath() + "/keys/pri.key");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		key = QString::fromUtf8(file.readAll());
		file.close();
	}
	LPPack::priMutex.unlock();
	return key;
}

QString LPPack::getPublicKey()
{
	LPPack::pubMutex.lock();
	QString key;
	QFile file(QCoreApplication::applicationDirPath() + "/keys/pub.key");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		key = QString::fromUtf8(file.readAll());
		file.close();
	}
	LPPack::pubMutex.unlock();
	return key;
}
