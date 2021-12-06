#ifndef RSASIGNATURE_H
#define RSASIGNATURE_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <QString>
#include <QDebug>
#include <QByteArray>

#pragma comment(lib, "openssl/openssl.lib")
#pragma comment(lib, "openssl/libcrypto.lib")
#pragma comment(lib, "openssl/libssl.lib")

class RSASignature final
{
public:
	RSASignature();
	static RSA* createRSA(unsigned char* key, int publi);
	static int public_encrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* encrypted);
	static int private_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted);
	static int private_encrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* encrypted);
	static int public_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted);
	static int public_encrypt(QByteArray& data, QString& keystr, QByteArray& encrypted);
	static int private_decrypt(QByteArray& data, QString& keystr, QByteArray& decrypted);
	static int private_encrypt(QByteArray& data, QString& keystr, QByteArray& encrypted);
	static int public_decrypt(QByteArray& data, QString& keystr, QByteArray& decrypted);
	static void hash512(QByteArray& data, QByteArray& hash);
private:
	constexpr static int padding = RSA_PKCS1_PADDING;
};

#endif // RSASIGNATURE_H
