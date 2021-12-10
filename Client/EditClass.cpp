#include "EditClass.h"
#include "ui_AddClass.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

EditClass::EditClass(QWidget *parent)
	: AddClass(parent)
{
	ui->id->setReadOnly(true);
	this->setWindowTitle("±à¼­¿Î³Ì");
	ui->groupBox->setTitle("±à¼­¿Î³Ì");
}

void EditClass::init(JC::Class& clas)
{
	ui->id->setText(QString::fromStdString(clas.id()));
	ui->name->setText(QString::fromStdString(clas.name()));
	ui->type->setCurrentIndex(clas.type());
	ui->timeTotal->setValue(clas.timeclass() + clas.timeexper());
	ui->timeClass->setValue(clas.timeclass());
	ui->timeExp->setValue(clas.timeexper());
	ui->credit->setValue(clas.credit());
	ui->term->setText(QString::fromStdString(clas.term()));
}