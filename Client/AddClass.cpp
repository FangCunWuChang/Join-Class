#include "AddClass.h"
#include "ui_AddClass.h"

AddClass::AddClass(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::AddClass();
	ui->setupUi(this);
}

AddClass::~AddClass()
{
	delete ui;
}

void AddClass::on_timeClass_valueChanged(double d)
{
	Q_UNUSED(d);
	ui->timeTotal->setValue(ui->timeClass->value() + ui->timeExp->value());
}

void AddClass::on_timeExp_valueChanged(double d)
{
	Q_UNUSED(d);
	ui->timeTotal->setValue(ui->timeClass->value() + ui->timeExp->value());
}
