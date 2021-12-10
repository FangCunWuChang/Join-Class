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
			"�޸�����",
			"������������",
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
		QMessageBox::warning(this, "����", "���벻��Ϊ��");
		return;
	}
	for (auto& c : password) {
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			QMessageBox::warning(this, "����", "�����������ֻ�Ӣ�Ĵ�Сд��ĸ����");
			return;
		}
	}
	QString passwordRepeat =
		QInputDialog::getText(
			this,
			"�޸�����",
			"���ٴ�����������",
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
		QMessageBox::warning(this, "����", "������������벻ͬ");
		return;
	}

	QByteArray passwordData = password.toUtf8();
	QByteArray passwordHash;
	RSASignature::hash512(passwordData, passwordHash);

	if (passwordHash == this->userPassword.toUtf8()) {
		QMessageBox::warning(this, "����", "�����뵱ǰ������ͬ");
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
		QMessageBox::warning(this, "����", "�޷�����Ӧ������ͨ��");
		return;
	}

	QByteArray answerData = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerData, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "Ȩ�޲���", QString::fromStdString(answer.mesage()));
		return;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "����", QString::fromStdString(answer.mesage()));
		return;
	}

	QMessageBox::information(this, "�޸�����", "�����Ѹ��ģ������µ�¼");
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
	QMessageBox::about(this, "����ѡ��ƽ̨", ::_JC_Message);
}

void UserWindow::on_actionAbout_Qt_triggered(bool checked)
{
	Q_UNUSED(checked);
	QMessageBox::aboutQt(this, "����Qt");
}
