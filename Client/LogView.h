#pragma once

#include <QDialog>
#include "CJsonObject/CJsonObject.hpp"
#include "pb/pb.h"
#include "LPPack.h"
#include "linkPortClient.h"
namespace Ui { class LogView; };

class LogView : public QDialog
{
	Q_OBJECT

public:
	LogView(QWidget *parent = Q_NULLPTR);
	~LogView();

	void init(neb::CJsonObject& config, QString userName, QString userPassword);

private:
	Ui::LogView *ui;

	neb::CJsonObject config;
	QString userName, userPassword;

protected:
	void showEvent(QShowEvent* event)override;
};
