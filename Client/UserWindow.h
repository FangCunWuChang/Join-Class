#pragma once

#include <QMainWindow>
#include <QString>
#include "CJsonObject/CJsonObject.hpp"
#include "linkPortClient.h"
#include "LPPack.h"
#include "version.h"
#include "pb/pb.h"
#include <QMessageBox>
#include <QInputDialog>
namespace Ui { class UserWindow; };

class UserWindow final : public QMainWindow
{
	Q_OBJECT

public:
	UserWindow(QWidget *parent = Q_NULLPTR);
	~UserWindow();

private:
	Ui::UserWindow *ui;

	neb::CJsonObject config;
	QString userName, userPassword;

public slots:
	void init(neb::CJsonObject& config, QString userName, QString userPassword);

private slots:
	void on_actionRefresh_Classes_triggered(bool checked);
	void on_actionChange_Password_triggered(bool checked);
	void on_actionLogout_triggered(bool checked);
	void on_actionAbout_triggered(bool checked);
	void on_actionAbout_Qt_triggered(bool checked);

signals:
	void initLogin(neb::CJsonObject& config);
	void showLogin();
};
