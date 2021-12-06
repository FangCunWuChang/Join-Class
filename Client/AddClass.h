#pragma once

#include <QDialog>
namespace Ui { class AddClass; };

class AddClass final : public QDialog
{
	Q_OBJECT

public:
	AddClass(QWidget *parent = Q_NULLPTR);
	~AddClass();

private:
	Ui::AddClass *ui;

private slots:
	void on_timeClass_valueChanged(double d);
	void on_timeExp_valueChanged(double d);
};
