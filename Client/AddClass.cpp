#include "AddClass.h"
#include "ui_AddClass.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

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

void AddClass::on_buttonBox_accepted()
{
	if (ui->id->text().isEmpty()) {
		QMessageBox::warning(this, "出错", "请输入课程编号");
		return;
	}
	for (auto& c : ui->id->text()) {
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			QMessageBox::warning(this, "出错", "课程编号需由数字或英文大小写字母构成");
			return;
		}
	}
	if (ui->name->text().isEmpty()) {
		QMessageBox::warning(this, "出错", "请输入课程名称");
		return;
	}
	if (ui->term->text().isEmpty()) {
		QMessageBox::warning(this, "出错", "请输入开课学期");
		return;
	}
	this->acceptFlag = true;
	this->close();
}

void AddClass::on_buttonBox_rejected()
{
	this->close();
}

JC::Class AddClass::getClass()
{
	JC::Class clas;

	clas.set_id(ui->id->text().toStdString());
	clas.set_name(ui->name->text().toStdString());
	switch (ui->type->currentIndex())
	{
	case 0:
		clas.set_type(JC::Class_Type::Class_Type_PUBLIC);
		break;
	case 1:
		clas.set_type(JC::Class_Type::Class_Type_REQUIRED);
		break;
	case 2:
		clas.set_type(JC::Class_Type::Class_Type_OPTIONAL);
		break;
	}
	clas.set_timeclass(ui->timeClass->value());
	clas.set_timeexper(ui->timeExp->value());
	clas.set_credit(ui->credit->value());
	clas.set_term(ui->term->text().toStdString());
	return clas;
}

bool AddClass::isAccepted()
{
	return this->acceptFlag;
}
