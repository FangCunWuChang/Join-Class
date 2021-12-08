#pragma once

#include <QDialog>
#include <QMessageBox>
#include "pb/pb.h"
namespace Ui { class AddClass; };

class AddClass : public QDialog
{
	Q_OBJECT

public:
	AddClass(QWidget *parent = Q_NULLPTR);
	~AddClass();

	JC::Class getClass();
	bool isAccepted();

protected:
	Ui::AddClass* ui;

private:
	bool acceptFlag = false;

private slots:
	void on_timeClass_valueChanged(double d);
	void on_timeExp_valueChanged(double d);

	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};
