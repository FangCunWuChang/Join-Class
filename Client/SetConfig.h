#pragma once

#include <QDialog>
#include <QMessageBox>
#include "CJsonObject/CJsonObject.hpp"
#include "LPPack.h"
#include "linkPortClient.h"
#include "pb/pb.h"
#include "getConfig.h"

namespace Ui { class SetConfig; };

class SetConfig : public QDialog 
{
	Q_OBJECT

public:
	SetConfig(QWidget *parent = Q_NULLPTR);
	~SetConfig();


private:
	Ui::SetConfig *ui;

protected:
	void closeEvent(QCloseEvent* event)override;

private slots:
	void on_address_textChanged(const QString& text);
	void on_port_valueChanged(int i);
	void on_connectWait_valueChanged(int i);
	void on_receiveWait_valueChanged(int i);
	void on_check_clicked();

	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

signals:
	void configOK();
	void configData(neb::CJsonObject& config);
	void closed();

public slots:
	void init(neb::CJsonObject& config);
};
