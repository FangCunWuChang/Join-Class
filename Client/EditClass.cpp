#include "EditClass.h"
#include "ui_AddClass.h"

EditClass::EditClass(QWidget *parent)
	: AddClass(parent)
{
	ui->id->setReadOnly(true);
}

void EditClass::init(JC::Class& clas)
{
	ui->id->setText(QString::fromStdString(clas.id()));
	ui->name->setText(QString::fromStdString(clas.name()));
	ui->type->setCurrentIndex(clas.type());
	ui->timeTotal->setValue(clas.timeclass() + clas.timeexper());
}