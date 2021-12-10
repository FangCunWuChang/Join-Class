#pragma once

#include <QDialog>
#include "pb/pb.h"
#include <QStringList>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
namespace Ui { class ViewClass; };

class ViewClass final : public QDialog
{
	Q_OBJECT

public:
	ViewClass(QWidget *parent = Q_NULLPTR);
	~ViewClass();

	void init(JC::Class& clas, QStringList idL, QStringList nL);
private:
	Ui::ViewClass *ui;

private slots:
	void on_exportList_clicked();
};
