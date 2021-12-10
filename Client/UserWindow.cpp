#include "UserWindow.h"
#include "ui_UserWindow.h"

UserWindow::UserWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::UserWindow();
	ui->setupUi(this);
}

UserWindow::~UserWindow()
{
	delete ui;
}

void UserWindow::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
}

void UserWindow::on_actionRefresh_Classes_triggered(bool checked)
{
	Q_UNUSED(checked);
}

void UserWindow::on_actionChange_Password_triggered(bool checked)
{
	Q_UNUSED(checked);
	bool passOk = false;
	QString password =
		QInputDialog::getText(
			this,
			"修改密码",
			"请输入新密码",
			QLineEdit::EchoMode::Password,
			QString(),
			&passOk,
			Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint,
			Qt::InputMethodHint::ImhNone
		);
	if (!passOk) {
		return;
	}
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
			&passOk,
			Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint,
			Qt::InputMethodHint::ImhNone
		);
	if (!passOk) {
		return;
	}
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

void UserWindow::on_actionLogout_triggered(bool checked)
{
	Q_UNUSED(checked);
	emit this->initLogin(this->config);
	emit this->showLogin();
	this->close();
}

void UserWindow::on_actionAbout_triggered(bool checked)
{
	Q_UNUSED(checked);
	QMessageBox::about(this, "关于选课平台", ::_JC_Message);
}

void UserWindow::on_actionAbout_Qt_triggered(bool checked)
{
	Q_UNUSED(checked);
	QMessageBox::aboutQt(this, "关于Qt");
}
