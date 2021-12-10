#pragma once

#include <QString>
#include "openssl/opensslv.h"
#include "google/protobuf/stubs/common.h"

static double _JC_Version = 0.1;

static QString Key_Sema = "{1CEEFAE0-5862-4887-B7A9-F243061FF840}";
static QString Key_SM = "{A2BB0A3C-5205-4D5B-82AD-1A39AD389338}";

static QString _JC_Message =
"Join Class Client " + QString::number(::_JC_Version) +
"\n(C)2021 WuChang. All rights reserved." +
"\n����ʱ�䣺" + QString(__DATE__) + " " + QString(__TIME__) +
"\nQt��汾��" + QString(QT_VERSION_STR) +
"\nOpenSSL��汾��" + QString(OPENSSL_VERSION_TEXT) +
"\nProtobuf��汾��" + QString::fromStdString(google::protobuf::internal::VersionString(google::protobuf::internal::kMinHeaderVersionForLibrary)) +
"\nCJsonObject��汾���ް汾��" +
"\n��Դҳ�棺https://github.com/FangCunWuChang/Join-Class" +
"\n������Դ��ʹ��GPLv3Э��" +
"\nͼ����Դ�ںϷʹ�ҵ��ѧ�ۺ���Ϣ�Ż� https://ehall.hfut.edu.cn:9000/aggr.hyperlink/ed1c66eede6741c1bb7729fee674cf61.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=minio%2F20211206%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20211206T132757Z&X-Amz-Expires=604800&X-Amz-SignedHeaders=host&X-Amz-Signature=da2315dba641103bc84fb98849e20d9482ba700938d42e405f75cbddafe24000";