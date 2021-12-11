#include "tasks.h"

QMutex Tasks::mutex;

JC::Task Tasks::getTask()
{
	Tasks::mutex.lock();
	JC::Task result;
	QFile file(QCoreApplication::applicationDirPath() + "/tasks/task.dat");
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray data = file.readAll();
		result.ParseFromArray(data.constData(), data.size());
		file.close();
	}
	Tasks::mutex.unlock();
	return result;
}

bool Tasks::setTask(const JC::Task& task)
{
	Tasks::mutex.lock();
	QDir dir(QCoreApplication::applicationDirPath() + "/tasks");
	if (!dir.exists()) {
		dir.mkdir(QCoreApplication::applicationDirPath() + "/tasks");
	}
	QFile file(QCoreApplication::applicationDirPath() + "/tasks/task.dat");
	if (file.exists()) {
		if (!file.remove()) {
			Tasks::mutex.unlock();
			return false;
		}
	}
	if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		QByteArray data(task.ByteSize(), 0);
		task.SerializeToArray(data.data(), data.size());
		file.write(data);
		file.close();
	}
	Tasks::mutex.unlock();
	return true;
}

JC::Student Tasks::getStudent(QString id)
{
	Tasks::mutex.lock();
	JC::Student result;
	JC::Task task;
	QFile file(QCoreApplication::applicationDirPath() + "/tasks/task.dat");
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray data = file.readAll();
		task.ParseFromArray(data.constData(), data.size());
		file.close();
	}
	Tasks::mutex.unlock();

	for (int i = 0; i < task.students_size(); i++) {
		if (task.students(i).id() == id.toStdString()) {
			result.CopyFrom(task.students(i));
			break;
		}
	}
	return result;
}

google::protobuf::RepeatedPtrField<JC::Class> Tasks::getClasses()
{
	Tasks::mutex.lock();
	google::protobuf::RepeatedPtrField<JC::Class> result;
	JC::Task task;
	QFile file(QCoreApplication::applicationDirPath() + "/tasks/task.dat");
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray data = file.readAll();
		task.ParseFromArray(data.constData(), data.size());
		file.close();
	}
	Tasks::mutex.unlock();

	result = task.classes();
	return result;
}

bool Tasks::setClasses(QString id, QStringList classes)
{
	Tasks::mutex.lock();
	JC::Task task;
	QFile file(QCoreApplication::applicationDirPath() + "/tasks/task.dat");
	if (file.exists()) {
		if (file.open(QIODevice::ReadOnly)) {
			QByteArray data = file.readAll();
			file.close();
			task.ParseFromArray(data.constData(), data.size());
		}
		else {
			Tasks::mutex.unlock();
			return false;
		}
		if (!file.remove()) {
			Tasks::mutex.unlock();
			return false;
		}
	}

	for (int i = 0; i < task.students_size(); i++) {
		if (task.students(i).id() == id.toStdString()) {
			task.mutable_students(i)->clear_classes();
			for (auto &str : classes) {
				task.mutable_students(i)->add_classes(str.toStdString());
			}
			break;
		}
	}

	if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		QByteArray data(task.ByteSize(), 0);
		task.SerializeToArray(data.data(), data.size());
		file.write(data);
		file.close();
	}
	else {
		Tasks::mutex.unlock();
		return false;
	}
	
	Tasks::mutex.unlock();
	return true;
}
