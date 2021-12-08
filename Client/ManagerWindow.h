#pragma once

#include <QMainWindow>
#include <QString>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "CJsonObject/CJsonObject.hpp"
#include "AddClass.h"
#include "linkPortClient.h"
#include "pb/pb.h"
#include "LPPack.h"
#include "LogView.h"
#include "ServerManage.h"
#include "version.h"
#include <QInputDialog>

namespace Ui { class ManagerWindow; };

class ManagerWindow final : public QMainWindow
{
	Q_OBJECT

public:
	ManagerWindow(QWidget *parent = Q_NULLPTR);
	~ManagerWindow();

private:
	Ui::ManagerWindow *ui;

	neb::CJsonObject config;
	QString userName, userPassword;

public slots:
	void init(neb::CJsonObject& config, QString userName, QString userPassword);

private slots:
	void on_actionServer_Information_triggered(bool checked);
	void on_actionServer_Restart_triggered(bool checked);
	void on_actionServer_Shutdown_triggered(bool checked);
	void on_actionLog_Manage_triggered(bool checked);
	void on_actionServer_Config_triggered(bool checked);
	void on_actionChange_Password_triggered(bool checked);
	void on_actionLogout_triggered(bool checked);
	void on_actionAbout_triggered(bool checked);
	void on_actionAbout_Qt_triggered(bool checked);

	void on_classes_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);
	void on_students_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);
	void on_users_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);

	void on_addClass_clicked();
	void on_editClass_clicked();

signals:
	void initConfig(neb::CJsonObject& config);
	void showConfig();
	void initLogin(neb::CJsonObject& config);
	void showLogin();
};
