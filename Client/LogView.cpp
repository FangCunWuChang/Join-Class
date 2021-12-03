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
