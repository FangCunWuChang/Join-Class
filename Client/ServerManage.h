#pragma once

#include <QDialog>
namespace Ui { class ServerManage; };

class ServerManage : public QDialog
{
	Q_OBJECT

public:
	ServerManage(QWidget *parent = Q_NULLPTR);
	~ServerManage();

private:
	Ui::ServerManage *ui;
};
