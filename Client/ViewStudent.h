#pragma once

#include <QDialog>
#include <QVector>
#include "pb/pb.h"
#include <QStringList>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
namespace Ui { class ViewStudent; };

class ViewStudent : public QDialog
{
	Q_OBJECT

public:
	ViewStudent(QWidget *parent = Q_NULLPTR);
	~ViewStudent();

	void init(JC::Student& student, QVector<JC::Class>& classes);

private:
	Ui::ViewStudent *ui;

private slots:
	void on_exportList_clicked();
};
