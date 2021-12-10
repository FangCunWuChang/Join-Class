#include "EditStudent.h"
#include "ui_AddStudent.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

EditStudent::EditStudent(QWidget *parent)
	: AddStudent(parent)
{
	ui->id->setReadOnly(true);
	this->setWindowTitle("编辑学生");
	ui->groupBox->setTitle("编辑学生");
}

void EditStudent::init(JC::Student& student)
{
	ui->id->setText(QString::fromStdString(student.id()));
	ui->name->setText(QString::fromStdString(student.name()));
}