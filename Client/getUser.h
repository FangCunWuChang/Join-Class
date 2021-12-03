#pragma once

#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include "CJsonObject/CJsonObject.hpp"

class GetUser
{
public:
	static bool check();
	static neb::CJsonObject getUser();
	static bool setUser(neb::CJsonObject& user);
};

