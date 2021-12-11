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
#include "EditClass.h"
#include "ViewClass.h"
#include "AddStudent.h"
#include "EditStudent.h"
#include <QVector>
#include "ViewStudent.h"
#include "RegUsers.h"
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QDateTime>
#include <QPainter>

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

	void resetCredit(int row);

	void clearClasses();
	void clearStudents();
	void clearUsers();

	void refreshUserList();

	void parseTask(JC::Task& task);

	QTimer timer = QTimer(this);

protected:
	void closeEvent(QCloseEvent* event)override;
	void paintEvent(QPaintEvent* event)override;

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
	void on_removeClass_clicked();
	void on_viewClass_clicked();

	void on_addStudent_clicked();
	void on_editStudent_clicked();
	void on_removeStudent_clicked();
	void on_viewStudent_clicked();

	void on_refreshUser_clicked();
	void on_addUser_clicked();
	void on_removeUser_clicked();
	void on_resetUser_clicked();
	void on_typeUser_clicked();
	void on_regUser_clicked();

	void on_actionGet_Task_triggered(bool checked);
	void on_actionSet_Task_triggered(bool checked);
	void on_actionNew_Task_triggered(bool checked);
	void on_actionSave_Task_triggered(bool checked);
	void on_actionLoad_Task_triggered(bool checked);

	void on_tabWidget_currentChanged(int index);

	void on_timerTimeOut();

signals:
	void initConfig(neb::CJsonObject& config);
	void showConfig();
	void initLogin(neb::CJsonObject& config);
	void showLogin();
};
