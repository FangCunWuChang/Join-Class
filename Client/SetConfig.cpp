#include "SetConfig.h"
#include "ui_SetConfig.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

SetConfig::SetConfig(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::SetConfig();
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

SetConfig::~SetConfig()
{
	delete ui;
}

void SetConfig::on_address_textChanged(const QString& text)
{
	Q_UNUSED(text);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void SetConfig::on_port_valueChanged(int i)
{
	Q_UNUSED(i);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void SetConfig::on_connectWait_valueChanged(int i)
{
	Q_UNUSED(i);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void SetConfig::on_receiveWait_valueChanged(int i)
{
	Q_UNUSED(i);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void SetConfig::on_check_clicked()
{
	LinkPortClient lpc;
	
	neb::CJsonObject configObject;
	configObject.Add("server", ui->address->text().toStdString());
	configObject.Add("port", ui->port->value());
	configObject.Add("connectTimeOut", ui->connectWait->value());
	configObject.Add("receiveTimeOut", ui->receiveWait->value());

	lpc.init(configObject);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_TEST);

	QByteArray data;
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "出错", "无法与相应服务器通信");
		return;
	}

	QByteArray answerData = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerData, answer);

	if (answer.type() != JC::Net::Answer_Type::Answer_Type_DONE) {
		QMessageBox::warning(this, "出错", "请求被服务器拒绝");
		return;
	}

	qDebug("Message:%s", answer.mesage().c_str());
	QMessageBox::information(this, "服务器信息", QString::fromStdString(answer.mesage()));
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void SetConfig::on_buttonBox_accepted()
{
	neb::CJsonObject configObject;
	configObject.Add("server", ui->address->text().toStdString());
	configObject.Add("port", ui->port->value());
	configObject.Add("connectTimeOut", ui->connectWait->value());
	configObject.Add("receiveTimeOut", ui->receiveWait->value());
	if (GetConfig::setConfig(configObject)) {
		QMessageBox::information(this, "服务器设置", "已写入服务器设置");
		emit this->configData(configObject);
		emit this->configOK();
	}
	else {
		QMessageBox::warning(this, "服务器设置", "未能成功写入服务器设置");
	}
	this->close();
}

void SetConfig::on_buttonBox_rejected()
{
	this->close();
}

void SetConfig::init(neb::CJsonObject& config)
{
	ui->address->setText(QString::fromStdString(config("server")));
	quint32 port = 12345, connectWait = 1000, receiveWait = 3000;
	config.Get("port", port);
	config.Get("connectTimeOut", connectWait);
	config.Get("receiveTimeOut", receiveWait);
	ui->port->setValue(port);
	ui->connectWait->setValue(connectWait);
	ui->receiveWait->setValue(receiveWait);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void SetConfig::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	emit this->closed();
}