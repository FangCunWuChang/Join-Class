#pragma once

#include <QString>
#include <QFile>
#include <QCoreApplication>
#include <QVariant>
#include <QMutex>
#include "CJsonObject/CJsonObject.hpp"
#include "logger.h"

class GetConfig final
{
public:
	static neb::CJsonObject config();
	static bool setConfig(const neb::CJsonObject& config);
private:
	static QMutex mutex;
};

