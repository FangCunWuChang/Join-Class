#pragma once

#include <QDialog>
#include "CJsonObject/CJsonObject.hpp"
#include "LPPack.h"
#include "linkPortClient.h"
#include "pb/pb.h"
#include "getUser.h"
#include "rsasignature.h"
#include <QMessageBox>

namespace Ui { class Login; };

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();

private:
	Ui::Login *ui;
	neb::CJsonObject config;

	bool passSave = false;
	QString passSaved = "";

protected:
	void showEvent(QShowEvent* event)override;

private slots:
	void on_config_clicked();
	void on_login_clicked();

public slots:
	void init(neb::CJsonObject& config);

signals:
	void initConfig(neb::CJsonObject& config);
	void wannaConfig();

	void initWindow(neb::CJsonObject& config, QString userId, QString userPassword);
	void showManager();
	void showUser();
};
