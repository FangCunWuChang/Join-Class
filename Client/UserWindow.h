#pragma once

#include <QMainWindow>
#include <QString>
#include "CJsonObject/CJsonObject.hpp"
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
};
