#include "AddStudent.h"
#include "ui_AddStudent.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

AddStudent::AddStudent(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::AddStudent();
	ui->setupUi(this);
}

AddStudent::~AddStudent()
{
	delete ui;
}

void AddStudent::on_buttonBox_accepted()
{
	if (ui->id->text().isEmpty()) {
		QMessageBox::warning(this, "出错", "请输入学号");
		return;
	}
	for (auto& c : ui->id->text()) {
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			QMessageBox::warning(this, "出错", "学号需由数字或英文大小写字母构成");
			return;
		}
	}
	if (ui->name->text().isEmpty()) {
		QMessageBox::warning(this, "出错", "请输入姓名");
		return;
	}
	this->acceptFlag = true;
	this->close();
}

void AddStudent::on_buttonBox_rejected()
{
	this->close();
}

bool AddStudent::isAccepted()
{
	return this->acceptFlag;
}

JC::Student AddStudent::getStudent()
{
	JC::Student student;
	student.set_id(ui->id->text().toStdString());
	student.set_name(ui->name->text().toStdString());
	return student;
}