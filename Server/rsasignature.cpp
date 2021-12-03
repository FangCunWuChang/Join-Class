#include "rsasignature.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

extern "C" {
#include "openssl/applink.c"
};

RSASignature::RSASignature()
{

}

/**
 * @brief RSASignature::createRSA 载入密钥
 * @param key 密钥
 * @param publi 公钥1 私钥0
 * @return
 */
RSA* RSASignature::createRSA(unsigned char* key, int publi)
{
	RSA* rsa = NULL;
	BIO* keybio;
	keybio = BIO_new_mem_buf(key, -1);
	if (keybio == NULL)
	{
		qDebug() << "Failed to create key BIO";
		return 0;
	}
	if (publi)
	{
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	}
	if (rsa == NULL)
	{
		qDebug() << "Failed to create RSA";
	}

	return rsa;
}
/**
 * @brief RSASignature::public_encrypt 公钥加密
 * @param data 待加密数据
 * @param data_len 待加密的数据长度
 * @param key 公钥
 * @param encrypted 加密后的数据
 * @return 加密长度
 */
int RSASignature::public_encrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* encrypted)
{
	RSA* rsa = createRSA(key, 1);
	if (rsa == NULL)
		return 0;
	int result = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);
	return result;
}
/**
 * @brief RSASignature::private_decrypt 私钥解密
 * @param enc_data 待解密数据
 * @param data_len 待解密数据长度
 * @param key 私钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::private_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted)
{
	RSA* rsa = createRSA(key, 0);
	if (rsa == NULL)
		return 0;
	int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, padding);
	return result;
}

/**
 * @brief RSASignature::private_encrypt 私钥加密
 * @param data 待加密数据
 * @param data_len 待加密数据长度
 * @param key 私钥
 * @param encrypted 加密后数据
 * @return
 */
int RSASignature::private_encrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* encrypted)
{
	RSA* rsa = createRSA(key, 0);
	if (rsa == NULL)
		return 0;
	//qDebug()<<RSA_size(rsa);
	int result = RSA_private_encrypt(data_len, data, encrypted, rsa, padding);
	return result;
}
/**
 * @brief RSASignature::public_decrypt 公钥解密
 * @param enc_data 待解密数据
 * @param data_len 待解密数据长度
 * @param key 公钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::public_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted)
{
	RSA* rsa = createRSA(key, 1);
	if (rsa == NULL)
		return 0;
	//qDebug()<<RSA_size(rsa);
	int  result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, padding);
	return result;
}
/**
 * @brief RSASignature::public_encrypt 公钥加密
 * @param data 待加密数据
 * @param keystr 公钥
 * @param encrypted 加密后数据
 * @return
 */
int RSASignature::public_encrypt(QByteArray& data, QString& keystr, QByteArray& encrypted)
{
	QByteArray keydata = keystr.toLocal8Bit();
	unsigned char* key = (unsigned char*)strdup(keydata.constData());//密钥
	RSA* rsa = createRSA(key, 1);
	if (rsa == NULL)
		return 0;
	free(key);
	int rsasize = RSA_size(rsa);
	int exppadding = rsasize;
	int result = -1;
	QByteArray decdata = data.toBase64(QByteArray::Base64Encoding);
	QByteArray signByteArray;
	int data_len = decdata.length();
	if (data_len > exppadding - 11)
		exppadding = exppadding - 11;
	int b = 0;
	int s = data_len / (exppadding);//片数
	if (data_len % (exppadding))
		s++;
	for (int i = 0; i < s; i++)
	{
		QByteArray subdata = { 0 };
		subdata.resize(exppadding);
		for (int j = 0; j < exppadding; j++)
		{
			if (i * exppadding + j > data_len)
				break;
			if (j + static_cast<long long>(i) * exppadding < decdata.size()) {
				subdata[j] = decdata[j + static_cast<qsizetype>(i) * exppadding];
			}
			else {
				subdata[j] = 0;
			}
		}
		unsigned char* smldata = (unsigned char*)strdup(subdata.constData());//数据分片
		unsigned char smlencrypted[1024] = { 0 };//片段加密数据
		b += RSA_public_encrypt(exppadding, smldata, smlencrypted, rsa, padding);
		if (b > 0)
		{
			QByteArray subarray = QByteArray::fromRawData((const char*)smlencrypted, rsasize);
			signByteArray.append(subarray);
		}
		free(smldata);
	}
	encrypted.append(signByteArray.toHex());
	result = b;
	return result;
}
/**
 * @brief RSASignature::private_decrypt 私钥解密
 * @param data 待解密数据
 * @param keystr 私钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::private_decrypt(QByteArray& data, QString& keystr, QByteArray& decrypted)
{
	QByteArray keydata = keystr.toLocal8Bit();
	unsigned char* key = (unsigned char*)strdup(keydata.constData());//密钥
	RSA* rsa = createRSA(key, 0);
	if (rsa == NULL)
		return 0;
	free(key);
	int rsasize = RSA_size(rsa);
	int result = -1;
	QByteArray encdata = QByteArray::fromHex(data);
	QByteArray signByteArray;
	int data_len = encdata.length();
	int b = 0;
	int s = data_len / (rsasize);//片数
	if (data_len % (rsasize))
		s++;
	for (int i = 0; i < s; i++)
	{
		QByteArray subdata = { 0 };
		subdata.resize(rsasize);
		for (int j = 0; j < rsasize; j++)
		{
			if (i * rsasize + j > data_len)
				break;
			if (j + static_cast<long long>(i) * rsasize < encdata.size()) {
				subdata[j] = encdata[j + static_cast<qsizetype>(i) * rsasize];
			}
			else {
				subdata[j] = 0;
			}
		}
		unsigned char* smldata = (unsigned char*)subdata.data();//(unsigned char*)strdup(subdata.constData());//数据分片
		unsigned char smlencrypted[1024] = { 0 };//片段加密数据
		b += RSA_private_decrypt(rsasize, smldata, smlencrypted, rsa, padding);
		if (b > 0)
		{
			QByteArray decdata((char*)smlencrypted);
			signByteArray.append(decdata);
		}
	}
	QByteArray b1 = QByteArray::fromBase64(signByteArray, QByteArray::Base64Encoding);
	decrypted.append(b1);
	result = b;
	return result;
}
/**
 * @brief RSASignature::private_encrypt 私钥加密
 * @param data 待加密数据
 * @param keystr 私钥
 * @param encrypted 解密后的数据
 * @return
 */
int RSASignature::private_encrypt(QByteArray& data, QString& keystr, QByteArray& encrypted)
{
	QByteArray keydata = keystr.toLocal8Bit();
	unsigned char* key = (unsigned char*)strdup(keydata.constData());//密钥
	RSA* rsa = createRSA(key, 0);
	if (rsa == NULL)
		return 0;
	free(key);
	int rsasize = RSA_size(rsa);
	int exppadding = rsasize;
	int result = -1;
	QByteArray decdata = data.toBase64(QByteArray::Base64Encoding);
	QByteArray signByteArray;
	int data_len = decdata.length();
	if (data_len > exppadding - 11)//padding占11位
		exppadding = exppadding - 11;
	int b = 0;
	int s = data_len / (exppadding);//片数
	if (data_len % (exppadding))
		s++;
	for (int i = 0; i < s; i++)
	{
		//分片加密
		QByteArray subdata = { 0 };
		subdata.resize(exppadding);
		for (int j = 0; j < exppadding; j++)
		{
			if (i * exppadding + j > data_len)
				break;
			if (j + static_cast<long long>(i) * exppadding < decdata.size()) {
				subdata[j] = decdata[j + static_cast<qsizetype>(i) * exppadding];
			}
			else {
				subdata[j] = 0;
			}
		}
		unsigned char* smldata = (unsigned char*)strdup(subdata.constData());//数据分片
		unsigned char smlencrypted[1024] = { 0 };//片段加密数据
		b += RSA_private_encrypt(exppadding, smldata, smlencrypted, rsa, padding);
		if (b > 0)
		{
			QByteArray subarray = QByteArray::fromRawData((const char*)smlencrypted, rsasize);
			signByteArray.append(subarray);
		}
		free(smldata);
	}
	encrypted.append(signByteArray.toHex());
	result = b;
	return result;
}
/**
 * @brief RSASignature::public_decrypt 公钥解密
 * @param data 待解密数据
 * @param keystr 公钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::public_decrypt(QByteArray& data, QString& keystr, QByteArray& decrypted)
{
	QByteArray keydata = keystr.toLocal8Bit();
	unsigned char* key = (unsigned char*)strdup(keydata.constData());//密钥
	RSA* rsa = createRSA(key, 1);
	if (rsa == NULL)
		return 0;
	free(key);
	int rsasize = RSA_size(rsa);
	int result = -1;
	QByteArray encdata = QByteArray::fromHex(data);
	QByteArray signByteArray;
	int data_len = encdata.length();
	int b = 0;
	int s = data_len / (rsasize);//片数
	if (data_len % (rsasize))
		s++;
	for (int i = 0; i < s; i++)
	{
		QByteArray subdata = { 0 };
		subdata.resize(rsasize);
		for (int j = 0; j < rsasize; j++)
		{
			if (i * rsasize + j > data_len)
				break;
			if (j + static_cast<long long>(i) * rsasize < encdata.size()) {
				subdata[j] = encdata[j + static_cast<qsizetype>(i) * rsasize];
			}
			else {
				subdata[j] = 0;
			}
		}
		unsigned char* smldata = (unsigned char*)subdata.data();//(unsigned char*)strdup(subdata.constData());//数据分片
		unsigned char smlencrypted[1024] = { 0 };//片段加密数据
		b += RSA_public_decrypt(rsasize, smldata, smlencrypted, rsa, padding);
		if (b > 0)
		{
			QByteArray decdata((char*)smlencrypted);
			signByteArray.append(decdata);
		}
	}
	QByteArray b1 = QByteArray::fromBase64(signByteArray, QByteArray::Base64Encoding);
	decrypted.append(b1);
	result = b;
	return result;
}

void RSASignature::hash512(QByteArray& data, QByteArray& hash)
{
	SHA512_CTX c{};
	unsigned char md[SHA512_DIGEST_LENGTH];
	SHA512((unsigned char*)data.constData(), data.size(), md);
	
	for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
		hash.append(QString::asprintf("%02x", md[i]).toUtf8());
	}
}