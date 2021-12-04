#include "LogView.h"
#include "ui_LogView.h"

LogView::LogView(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::LogView();
	ui->setupUi(this);
}

LogView::~LogView()
{
	delete ui;
}

void LogView::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
}

void LogView::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);
}