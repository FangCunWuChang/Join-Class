#pragma once

#include <QDialog>
#include <QMessageBox>
#include "CJsonObject/CJsonObject.hpp"
namespace Ui { class ServerManage; };

class ServerManage final : public QDialog
{
	Q_OBJECT

public:
	ServerManage(QWidget *parent = Q_NULLPTR);
	~ServerManage();

	void init(neb::CJsonObject& config);

	bool isAccepted();
	neb::CJsonObject& getConfig();

private:
	Ui::ServerManage *ui;
	neb::CJsonObject config;

	bool accepted = false;

private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};
