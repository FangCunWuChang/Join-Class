#pragma once

#include <QDialog>
namespace Ui { class LogView; };

class LogView : public QDialog
{
	Q_OBJECT

public:
	LogView(QWidget *parent = Q_NULLPTR);
	~LogView();

private:
	Ui::LogView *ui;
};
