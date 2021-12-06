#include "ServerManage.h"
#include "ui_ServerManage.h"

ServerManage::ServerManage(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ServerManage();
	ui->setupUi(this);
}

ServerManage::~ServerManage()
{
	delete ui;
}
