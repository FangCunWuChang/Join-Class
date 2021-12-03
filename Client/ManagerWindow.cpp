#include "ManagerWindow.h"
#include "ui_ManagerWindow.h"

ManagerWindow::ManagerWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::ManagerWindow();
	ui->setupUi(this);
}

ManagerWindow::~ManagerWindow()
{
	delete ui;
}

void ManagerWindow::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
}
