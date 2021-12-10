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

	QString claList = " \t�γ̱��\t�γ�����\t����\t��ѧʱ\t�ڿ�ѧʱ\tʵ��ѧʱ\tѧ��\t����ѧ��\n";
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
			claList += "������";
			break;
		case JC::Class_Type::Class_Type_REQUIRED:
			claList += "רҵ���޿�";
			break;
		case JC::Class_Type::Class_Type_OPTIONAL:
			claList += "ѡ�޿�";
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
	QString fileName = QFileDialog::getSaveFileName(this, "ѡ�񱣴�λ��", QDir::currentPath(), "csv����ļ�(*.csv)");
	if (fileName.isEmpty()) {
		return;
	}

	QFileInfo fi(fileName);
	QDir::setCurrent(fi.absolutePath());

	QString data;

	//data += QString("\"�γ̱��\",\"�γ�����\",\"����\",\"��ѧʱ\",\"�ڿ�ѧʱ\",\"ʵ��ѧʱ\",\"ѧ��\",\"����ѧ��\"\r\n");

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
		QMessageBox::warning(this, "����", "�޷����ļ�");
		return;
	}
	file.write(data.toLocal8Bit());
	file.close();
	QMessageBox::information(this, "�������", "�ѵ��������" + fileName);
}
