#include "UserWindow.h"
#include "ui_UserWindow.h"

UserWindow::UserWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::UserWindow();
	ui->setupUi(this);
}

UserWindow::~UserWindow()
{
	delete ui;
}

void UserWindow::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
}