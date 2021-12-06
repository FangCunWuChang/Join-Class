#include "LogView.h"
#include "ui_LogView.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

LogView::LogView(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::LogView();
	ui->setupUi(this);
}

LogView::~LogView()
{
	delete ui;
}

void LogView::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
}

void LogView::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);
	ui->logs->clear();
	ui->text->clear();

	LinkPortClient lpc;
	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETLOGLIST);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());

	QByteArray requestPack;
	LPPack::packRequestWithPubilc(request, requestPack);

	if (!lpc.send(requestPack)) {
		QMessageBox::warning(this, "出错", "无法与服务器通信");
		return;
	}

	QByteArray answerPack = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerPack, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
		return;
	}

	for (int i = 0; i < answer.loglist_size(); i++) {
		ui->logs->addItem(QString::fromStdString(answer.loglist(i)));
	}
	ui->logs->setCurrentRow(answer.loglist_size() - 1);
}

void LogView::on_logs_currentRowChanged(int currentRow)
{
	ui->text->clear();
	if (currentRow >= 0 && currentRow < ui->logs->count()) {
		LinkPortClient lpc;
		lpc.init(this->config);

		JC::Net::Request request;
		request.set_type(JC::Net::Request_Type::Request_Type_GETLOGDATA);
		request.set_user(this->userName.toStdString());
		request.set_password(this->userPassword.toStdString());
		request.set_logname(ui->logs->item(currentRow)->text().toStdString());

		QByteArray requestPack;
		LPPack::packRequestWithPubilc(request, requestPack);

		if (!lpc.send(requestPack)) {
			QMessageBox::warning(this, "出错", "无法与服务器通信");
			return;
		}

		QByteArray answerPack = lpc.getResult();
		JC::Net::Answer answer;
		LPPack::unpackAnswerWithPubilc(answerPack, answer);

		if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
			QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
			return;
		}
		if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
			QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
			return;
		}

		ui->text->setPlainText(QString::fromStdString(answer.logdata()));
	}
}

void LogView::on_getLogs_clicked()
{
	ui->logs->clear();
	ui->text->clear();

	LinkPortClient lpc;
	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETLOGLIST);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());

	QByteArray requestPack;
	LPPack::packRequestWithPubilc(request, requestPack);

	if (!lpc.send(requestPack)) {
		QMessageBox::warning(this, "出错", "无法与服务器通信");
		return;
	}

	QByteArray answerPack = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerPack, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
		return;
	}

	for (int i = 0; i < answer.loglist_size(); i++) {
		ui->logs->addItem(QString::fromStdString(answer.loglist(i)));
	}
	ui->logs->setCurrentRow(answer.loglist_size() - 1);
}

void LogView::on_clearLogs_clicked()
{
	QMessageBox::StandardButton result = QMessageBox::warning(this, "警告", "此操作将会清空服务端的应用日志且不可逆\n仍要继续？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (result != QMessageBox::Yes) {
		return;
	}

	LinkPortClient lpc;
	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_CLEARLOGS);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());

	QByteArray requestPack;
	LPPack::packRequestWithPubilc(request, requestPack);

	if (!lpc.send(requestPack)) {
		QMessageBox::warning(this, "出错", "无法与服务器通信");
		return;
	}

	QByteArray answerPack = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerPack, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
		return;
	}

	QMessageBox::information(this, "清空日志", "已清空日志");
	this->on_getLogs_clicked();
}