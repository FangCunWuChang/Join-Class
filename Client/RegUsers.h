#pragma once

#include <QDialog>
#include "CJsonObject/CJsonObject.hpp"
#include "pb/pb.h"
#include "linkPortClient.h"
#include "LPPack.h"
#include <QMessageBox>
#include <QThread>
#include <QStringList>
namespace Ui { class RegUsers; };

class SendObject final :public QObject
{
	Q_OBJECT

public:
	SendObject(QObject* parent = Q_NULLPTR);
	~SendObject();

	void init(neb::CJsonObject& config, QString userName, QString userPassword);
	void setList(QStringList list);

private:
	neb::CJsonObject config;
	QString userName, userPassword;
	QStringList idList;

public slots:
	void start();

signals:
	void percents(int percent);
	void warning(QString message);
	void finished();
};

class RegUsers final : public QDialog
{
	Q_OBJECT

public:
	RegUsers(QWidget *parent = Q_NULLPTR);
	~RegUsers();

	void init(neb::CJsonObject& config, QString userName, QString userPassword);
	void setList(QStringList list);

private:
	Ui::RegUsers *ui;

	neb::CJsonObject config;
	QString userName, userPassword;
	QStringList idList;

	SendObject *so = nullptr;
	QThread r_thread;

private slots:
	void on_percents(int percent);
	void on_warning(QString message);
	void on_finished();

	void on_start_clicked();

signals:
	void sigStart();
};
