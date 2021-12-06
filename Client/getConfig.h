#pragma once

#include <QString>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QVariant>
#include <QMutex>
#include "CJsonObject/CJsonObject.hpp"

class GetConfig final
{
public:
	static neb::CJsonObject config();
	static bool setConfig(const neb::CJsonObject& config);

	static bool checkConfig();

private:
	static QMutex mutex;
};

