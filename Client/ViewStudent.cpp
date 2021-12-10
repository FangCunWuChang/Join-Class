#include "ViewStudent.h"
#include "ui_ViewStudent.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

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
	ui->id->setText(QString::fromStdString(student.id()));
	ui->name->setText(QString::fromStdString(student.name()));
	ui->count->setText(QString::number(classes.size()));

	QString claList = " \t课程编号\t课程名称\t类型\t总学时\t授课学时\t实验学时\t学分\t开课学期\n";
	double creditSum = 0;
	for (int i = 0; i < classes.size(); i++) {
		claList += QString::number(i + 1);
		claList += "\t";
		claList += QString::fromStdString(classes.at(i).id());
		claList += "\t";
		claList += QString::fromStdString(classes.at(i).name());
		claList += "\t";
		switch (classes.at(i).type())
		{
		case JC::Class_Type::Class_Type_PUBLIC:
			claList += "公共课";
			break;
		case JC::Class_Type::Class_Type_REQUIRED:
			claList += "专业必修课";
			break;
		case JC::Class_Type::Class_Type_OPTIONAL:
			claList += "选修课";
			break;
		}
		claList += "\t";
		claList += QString::number(classes.at(i).timeclass() + classes.at(i).timeexper());
		claList += "\t";
		claList += QString::number(classes.at(i).timeclass());
		claList += "\t";
		claList += QString::number(classes.at(i).timeexper());
		claList += "\t";
		claList += QString::number(classes.at(i).credit());
		claList += "\t";
		claList += QString::fromStdString(classes.at(i).term());
		claList += "\n";
		creditSum += classes.at(i).credit();
	}

	ui->credit->setText(QString::number(creditSum));
	ui->claList->setPlainText(claList);
}

void ViewStudent::on_exportList_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "选择保存位置", QDir::currentPath(), "csv表格文件(*.csv)");
	if (fileName.isEmpty()) {
		return;
	}

	QFileInfo fi(fileName);
	QDir::setCurrent(fi.absolutePath());

	QString data;

	//data += QString("\"课程编号\",\"课程名称\",\"类型\",\"总学时\",\"授课学时\",\"实验学时\",\"学分\",\"开课学期\"\r\n");

	QStringList lineList = ui->claList->toPlainText().split('\n', Qt::SkipEmptyParts);
	for (auto& s : lineList) {
		if (!s.isEmpty()) {
			QStringList subSL = s.split('\t', Qt::SkipEmptyParts);
			if (subSL.size() == 9) {
				data += QString(subSL.at(1)).prepend('\"').append('\"');
				for (int i = 2; i <= 8; i++) {
					data += ",";
					data += QString(subSL.at(i)).prepend('\"').append('\"');
				}
				data += "\r\n";
			}
		}
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		QMessageBox::warning(this, "出错", "无法打开文件");
		return;
	}
	file.write(data.toLocal8Bit());
	file.close();
	QMessageBox::information(this, "导出表格", "已导出表格至" + fileName);
}
