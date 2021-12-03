#pragma once

#include <QSharedMemory>
#include <QString>
#include <QSystemSemaphore>

class SingleManager
{
	SingleManager();
	static SingleManager single;

public:
	static SingleManager& getClass();

	void initSM(QString key);
	void initSema(QString key);
	bool check();
	bool start();
	bool exit();

	void lock();
	void unlock();

private:
	QSharedMemory sharedMemory;
	QSystemSemaphore sema = QSystemSemaphore("");
};

