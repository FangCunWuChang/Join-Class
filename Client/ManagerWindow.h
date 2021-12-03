#pragma once

#include <QMainWindow>
#include <QString>
#include "CJsonObject/CJsonObject.hpp"
namespace Ui { class ManagerWindow; };

class ManagerWindow : public QMainWindow
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
};
