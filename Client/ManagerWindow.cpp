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

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETCONFIG);
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

	neb::CJsonObject configTemp;
	configTemp.Parse(answer.config());

	ServerManage sm(this);
	sm.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	sm.init(configTemp);
	sm.exec();

	if (!sm.isAccepted()) {
		return;
	}
	configTemp = sm.getConfig();

	request.Clear();
	request.set_type(JC::Net::Request_Type::Request_Type_SETCONFIG);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	request.set_config(configTemp.ToFormattedString());

	data.clear();
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "出错", "无法与相应服务器通信");
		return;
	}

	answerData.clear();
	answerData = lpc.getResult();
	answer.Clear();
	LPPack::unpackAnswerWithPubilc(answerData, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
		return;
	}

	QMessageBox::information(this, "服务器配置", "已保存服务器配置");

	request.Clear();
	request.set_type(JC::Net::Request_Type::Request_Type_RESTART);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());

	data.clear();
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "出错", "无法与相应服务器通信");
		return;
	}

	answerData.clear();
	answerData = lpc.getResult();
	answer.Clear();
	LPPack::unpackAnswerWithPubilc(answerData, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "权限不足", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "出错", QString::fromStdString(answer.mesage()));
		return;
	}

	QMessageBox::information(this, "服务器配置", "已重启服务端应用");
	emit this->initConfig(this->config);
	emit this->showConfig();
	this->close();
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
	int currentIndex = ui->classes->currentRow();
	AddClass ac(this);
	ac.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	ac.exec();
	if (ac.isAccepted()) {
		JC::Class clas = ac.getClass();
		for (int i = 0; i < ui->classes->rowCount(); i++) {
			if (ui->classes->itemAt(i, 0)->text() == QString::fromStdString(clas.id())) {
				QMessageBox::warning(this, "添加课程", "课程已存在");
				return;
			}
		}

		ui->classes->insertRow(currentIndex + 1);

		QTableWidgetItem* item = nullptr;

		item = new QTableWidgetItem(QString::fromStdString(clas.id()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 0, item);//编号

		item = new QTableWidgetItem(QString::fromStdString(clas.name()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 1, item);//名称

		switch (clas.type())
		{
		case JC::Class_Type::Class_Type_PUBLIC:
			item = new QTableWidgetItem("公共课");
			break;
		case JC::Class_Type::Class_Type_REQUIRED:
			item = new QTableWidgetItem("专业必修课");
			break;
		case JC::Class_Type::Class_Type_OPTIONAL:
			item = new QTableWidgetItem("选修课");
			break;
		}
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 2, item);//类型

		item = new QTableWidgetItem(QString::number(clas.timeclass() + clas.timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 3, item);//总学时

		item = new QTableWidgetItem(QString::number(clas.timeclass()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 4, item);//授课学时

		item = new QTableWidgetItem(QString::number(clas.timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 5, item);//实验学时

		item = new QTableWidgetItem(QString::number(clas.credit()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 6, item);//学分

		item = new QTableWidgetItem(QString::fromStdString(clas.term()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 7, item);//开课学期

		switch (clas.type())
		{
		case JC::Class_Type::Class_Type_PUBLIC:
			item = new QTableWidgetItem(QString::number(ui->students->rowCount()));
			break;
		case JC::Class_Type::Class_Type_REQUIRED:
			item = new QTableWidgetItem(QString::number(ui->students->rowCount()));
			break;
		case JC::Class_Type::Class_Type_OPTIONAL:
			item = new QTableWidgetItem("0");
			break;
		}
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			return;
		}
		ui->classes->setItem(currentIndex + 1, 8, item);//选课人数

		for (int i = 0; i < ui->students->rowCount(); i++) {
			QString SClasses = ui->students->itemAt(i, 4)->text();
			QStringList SCList = SClasses.split(';', Qt::SkipEmptyParts);
			if (clas.type() != JC::Class_Type::Class_Type_OPTIONAL) {
				if (!SCList.contains(QString::fromStdString(clas.id()))) {
					SCList.append(QString::fromStdString(clas.id()));
				}
			}
			else {
				if (SCList.contains(QString::fromStdString(clas.id()))) {
					SCList.removeAll(QString::fromStdString(clas.id()));
				}
			}
			ui->students->itemAt(i, 4)->setText(SCList.join(';'));
		}
	}
}

void ManagerWindow::on_editClass_clicked()
{
	int currentIndex = ui->classes->currentRow();

}

void ManagerWindow::on_actionChange_Password_triggered(bool checked)
{
	QString password =
		QInputDialog::getText(
			this,
			"修改密码",
			"请输入新密码",
			QLineEdit::EchoMode::Password,
			QString(),
			nullptr,
			Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint,
			Qt::InputMethodHint::ImhNone
		);
	if (password.isEmpty()) {
		QMessageBox::warning(this, "出错", "密码不得为空");
		return;
	}
	for (auto& c : password) {
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			QMessageBox::warning(this, "出错", "密码需由数字或英文大小写字母构成");
			return;
		}
	}
	QString passwordRepeat =
		QInputDialog::getText(
			this,
			"修改密码",
			"请再次输入新密码",
			QLineEdit::EchoMode::Password,
			QString(),
			nullptr,
			Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint,
			Qt::InputMethodHint::ImhNone
		);
	if (password != passwordRepeat) {
		QMessageBox::warning(this, "出错", "两次输入的密码不同");
		return;
	}

	QByteArray passwordData = password.toUtf8();
	QByteArray passwordHash;
	RSASignature::hash512(passwordData, passwordHash);

	if (passwordHash == this->userPassword.toUtf8()) {
		QMessageBox::warning(this, "出错", "密码与当前密码相同");
		return;
	}

	JC::User user;
	user.set_id(this->userName.toStdString());
	user.set_password(QString::fromUtf8(passwordHash).toStdString());
	user.set_type(JC::User_Type::User_Type_MANAGER);

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_CHANGEUSER);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	request.mutable_newuser()->CopyFrom(user);

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

	QMessageBox::information(this, "修改密码", "密码已更改，请重新登录");
	emit this->initLogin(this->config);
	emit this->showLogin();
	this->close();
}

void ManagerWindow::on_actionLogout_triggered(bool checked)
{
	emit this->initLogin(this->config);
	emit this->showLogin();
	this->close();
}

void ManagerWindow::on_actionAbout_triggered(bool checked)
{
	QString message =
		"Join Class Client " + QString::number(::_JC_Version) +
		"\n(C)2021 WuChang. All rights reserved." +
		"\n编译时间：" + QString(__DATE__) + " " + QString(__TIME__) +
		"\nQt库版本：" + QString(QT_VERSION_STR) +
		"\nOpenSSL库版本：" + QString(OPENSSL_VERSION_TEXT) +
		"\nProtobuf库版本：" + QString::fromStdString(google::protobuf::internal::VersionString(google::protobuf::internal::kMinHeaderVersionForLibrary)) +
		"\nCJsonObject库版本：无版本号" +
		"\n开源页面：https://github.com/FangCunWuChang/Join-Class" +
		"\n本程序源码使用GPLv3协议" +
		"\n图标来源于合肥工业大学综合信息门户 https://ehall.hfut.edu.cn:9000/aggr.hyperlink/ed1c66eede6741c1bb7729fee674cf61.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=minio%2F20211206%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20211206T132757Z&X-Amz-Expires=604800&X-Amz-SignedHeaders=host&X-Amz-Signature=da2315dba641103bc84fb98849e20d9482ba700938d42e405f75cbddafe24000";
	QMessageBox::about(this, "关于选课平台", message);
}

void ManagerWindow::on_actionAbout_Qt_triggered(bool checked)
{
	QMessageBox::aboutQt(this, "关于Qt");
}
