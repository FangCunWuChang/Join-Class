#include "singleManager.h"

SingleManager::SingleManager()
{

}

SingleManager SingleManager::single;

SingleManager& SingleManager::getClass()
{
	return SingleManager::single;
}

void SingleManager::initSM(QString key)
{
#ifdef Q_OS_LINUX
	QSharedMemory SMT(key);
	if (SMT.attach()) {
		SMT.detach();
	}
#endif // Q_OS_LINUX

	this->sharedMemory.setKey(key);
}

void SingleManager::initSema(QString key)
{
	this->sema.setKey(key, 1, QSystemSemaphore::Open);
}

bool SingleManager::check()
{
	return this->sharedMemory.attach();
}

bool SingleManager::start()
{
	return this->sharedMemory.create(1);
}

void SingleManager::lock()
{
	this->sema.acquire();
}

void SingleManager::unlock()
{
	this->sema.release();
}

bool SingleManager::exit()
{
	return this->sharedMemory.detach();
}
