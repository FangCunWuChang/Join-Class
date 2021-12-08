#include "ServerManage.h"
#include "ui_ServerManage.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

ServerManage::ServerManage(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ServerManage();
	ui->setupUi(this);
}

ServerManage::~ServerManage()
{
	delete ui;
}

void ServerManage::init(neb::CJsonObject& config)
{
	this->config = config;
	this->accepted = false;

	quint32 port = 12345;
	quint32 disconnectWait = 1000;
	quint32 receiveWait = 3000;
	config.Get("localPort", port);
	config.Get("disconnectTimeOut", disconnectWait);
	config.Get("receiveTimeOut", receiveWait);

	ui->port->setValue(port);
	ui->disconnectWait->setValue(disconnectWait);
	ui->receiveWait->setValue(receiveWait);
}

void ServerManage::on_buttonBox_accepted()
{
	QMessageBox::StandardButton result = QMessageBox::warning(this, "警告", "修改服务端配置可能会导致服务端发生不可预料的异常并失去连接\n仍要继续吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (result != QMessageBox::Yes) {
		return;
	}

	this->config.Replace("localPort", ui->port->value());
	this->config.Replace("disconnectTimeOut", ui->disconnectWait->value());
	this->config.Replace("receiveTimeOut", ui->receiveWait->value());

	this->accepted = true;
	this->close();
}

void ServerManage::on_buttonBox_rejected()
{
	this->close();
}

bool ServerManage::isAccepted()
{
	return this->accepted;
}

neb::CJsonObject& ServerManage::getConfig()
{
	return this->config;
}