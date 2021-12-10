#include "ViewClass.h"
#include "ui_ViewClass.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

ViewClass::ViewClass(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ViewClass();
	ui->setupUi(this);
}

ViewClass::~ViewClass()
{
	delete ui;
}

void ViewClass::init(JC::Class& clas, QStringList idL, QStringList nL)
{
	ui->id->setText(QString::fromStdString(clas.id()));
	ui->name->setText(QString::fromStdString(clas.name()));
	switch (clas.type())
	{
	case JC::Class_Type::Class_Type_PUBLIC:
		ui->type->setText("������");
		break;
	case JC::Class_Type::Class_Type_REQUIRED:
		ui->type->setText("רҵ���޿�");
		break;
	case JC::Class_Type::Class_Type_OPTIONAL:
		ui->type->setText("ѡ�޿�");
		break;
	}
	ui->timeTotal->setText(QString::number(clas.timeclass() + clas.timeexper()));
	ui->timeClass->setText(QString::number(clas.timeclass()));
	ui->timeExper->setText(QString::number(clas.timeexper()));
	ui->credit->setText(QString::number(clas.credit()));
	ui->term->setText(QString::fromStdString(clas.term()));
	ui->stuCount->setText(QString::number(idL.size()));
	QString list = " \tѧ��\t����\n";
	for (int i = 0; i < idL.size(); i++) {
		list += QString::number(i + 1);
		list += "\t";
		list += idL.at(i);
		list += "\t";
		list += nL.at(i);
		list += "\n";
	}
	ui->stuList->setPlainText(list);
}

void ViewClass::on_exportList_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "ѡ�񱣴�λ��", QDir::currentPath(), "csv����ļ�(*.csv)");
	if (fileName.isEmpty()) {
		return;
	}

	QFileInfo fi(fileName);
	QDir::setCurrent(fi.absolutePath());

	QString data;

	//data += QString("\"ѧ��\",\"����\"\r\n");

	QStringList lineList = ui->stuList->toPlainText().split('\n', Qt::SkipEmptyParts);
	for (auto& s : lineList) {
		if (!s.isEmpty()) {
			QStringList subSL = s.split('\t', Qt::SkipEmptyParts);
			if (subSL.size() == 3) {
				data += QString(subSL.at(1)).prepend('\"').append('\"');
				data += ",";
				data += QString(subSL.at(2)).prepend('\"').append('\"');
				data += "\r\n";
			}
		}
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		QMessageBox::warning(this, "����", "�޷����ļ�");
		return;
	}
	file.write(data.toLocal8Bit());
	file.close();
	QMessageBox::information(this, "�������", "�ѵ��������" + fileName);
}