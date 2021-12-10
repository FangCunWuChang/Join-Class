#pragma once

#include <QDialog>
#include <QMessageBox>
#include "pb/pb.h"
namespace Ui { class AddStudent; };

class AddStudent : public QDialog
{
	Q_OBJECT

public:
	AddStudent(QWidget *parent = Q_NULLPTR);
	~AddStudent();

	bool isAccepted();
	JC::Student getStudent();

private:
	bool acceptFlag = false;

protected:
	Ui::AddStudent* ui;

private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};
