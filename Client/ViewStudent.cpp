#include "ViewStudent.h"
#include "ui_ViewStudent.h"

ViewStudent::ViewStudent(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ViewStudent();
	ui->setupUi(this);
}

ViewStudent::~ViewStudent()
{
	delete ui;
}

void ViewStudent::init(JC::Student& student, QVector<JC::Class>& classes)
{

}

void ViewStudent::on_exportList_clicked()
{

}
