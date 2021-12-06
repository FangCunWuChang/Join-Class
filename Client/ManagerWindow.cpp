#include "ManagerWindow.h"
#include "ui_ManagerWindow.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

ManagerWindow::ManagerWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::ManagerWindow();
	ui->setupUi(this);
}

ManagerWindow::~ManagerWindow()
{
	delete ui;
}

void ManagerWindow::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
}

void  ManagerWindow::on_actionServer_Information_triggered(bool checked)
{
	Q_UNUSED(checked);
	LinkPortClient lpc;

	lpc.init(this->config);

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

	QMessageBox::information(this, "服务器信息", QString::fromStdString(answer.mesage()) + "\n" + QString::fromStdString(this->config("server")) + ":" + QString::fromStdString(this->config("port")));
}

void ManagerWindow::on_actionServer_Restart_triggered(bool checked)
{
	Q_UNUSED(checked);
	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_RESTART);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());

	QByteArray data;
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "出错", "无法与相应服务器通信");
		return;
	}

	QByteArray answerData = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerData, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
		return;
	}

	QMessageBox::information(this, "重启服务端", "已重启服务端应用");
	return;
}

void ManagerWindow::on_actionServer_Shutdown_triggered(bool checked)
{
	Q_UNUSED(checked);

	QMessageBox::StandardButton result = QMessageBox::warning(this, "关闭服务端", "确认要关闭服务端应用？\n此操作会使你完全失去与服务端的连接\n并且此操作不可逆", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (result != QMessageBox::Yes) {
		return;
	}

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_SHUTDOWN);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());

	QByteArray data;
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "出错", "无法与相应服务器通信");
		return;
	}

	QByteArray answerData = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerData, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
		return;
	}

	QMessageBox::information(this, "关闭服务端", "已关闭服务端应用");
	emit this->initConfig(this->config);
	emit this->showConfig();
	this->close();
	return;
}

void ManagerWindow::on_actionLog_Manage_triggered(bool checked)
{
	Q_UNUSED(checked);
	LogView lv(this);
	lv.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	lv.init(this->config, this->userName, this->userPassword);
	lv.exec();
}

void ManagerWindow::on_actionServer_Config_triggered(bool checked)
{
	Q_UNUSED(checked);

}

void ManagerWindow::on_classes_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous)
{
	Q_UNUSED(current);
	Q_UNUSED(previous);
	int currentIndex = ui->classes->currentRow();
	if (currentIndex >= 0 && currentIndex < ui->classes->rowCount()) {
		ui->editClass->setEnabled(true);
		ui->removeClass->setEnabled(true);
		ui->viewClass->setEnabled(true);
	}
	else {
		ui->editClass->setEnabled(false);
		ui->removeClass->setEnabled(false);
		ui->viewClass->setEnabled(false);
	}
}

void ManagerWindow::on_students_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous)
{
	Q_UNUSED(current);
	Q_UNUSED(previous);
	int currentIndex = ui->students->currentRow();
	if (currentIndex >= 0 && currentIndex < ui->students->rowCount()) {
		ui->editStudent->setEnabled(true);
		ui->removeStudent->setEnabled(true);
		ui->viewStudent->setEnabled(true);
	}
	else {
		ui->editStudent->setEnabled(false);
		ui->removeStudent->setEnabled(false);
		ui->viewStudent->setEnabled(false);
	}
}

void ManagerWindow::on_users_currentItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous)
{
	Q_UNUSED(current);
	Q_UNUSED(previous);
	int currentIndex = ui->users->currentRow();
	if (currentIndex >= 0 && currentIndex < ui->users->rowCount()) {
		QTableWidgetItem* current = ui->users->item(currentIndex, 0);
		if (current->text() != this->userName) {
			ui->removeUser->setEnabled(true);
			ui->resetUser->setEnabled(true);
			ui->typeUser->setEnabled(true);
		}
		else {
			ui->removeUser->setEnabled(false);
			ui->resetUser->setEnabled(false);
			ui->typeUser->setEnabled(false);
		}
	}
	else {
		ui->removeUser->setEnabled(false);
		ui->resetUser->setEnabled(false);
		ui->typeUser->setEnabled(false);
	}
}

void ManagerWindow::on_addClass_clicked()
{
	int currentIndex = ui->users->currentRow();
	AddClass ac(this);
	ac.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	ac.exec();
}
