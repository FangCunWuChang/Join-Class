#include "Login.h"
#include "ui_Login.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

Login::Login(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::Login();
	ui->setupUi(this);

	ui->message->hide();
}

Login::~Login()
{
	delete ui;
}

void Login::init(neb::CJsonObject& config)
{
	this->config = config;
}

void Login::on_config_clicked()
{
	emit this->initConfig(this->config);
	emit this->wannaConfig();
	this->close();
}

void Login::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);
	ui->message->setText("���״���");
	ui->message->hide();
	if (GetUser::check()) {
		neb::CJsonObject user = GetUser::getUser();

		ui->user->setText(QString::fromStdString(user("name")));
		ui->password->setPlaceholderText("�ѱ��������");
		ui->password->clear();
		this->passSave = true;
		this->passSaved = QString::fromStdString(user("password"));
	}
	else {
		ui->password->setPlaceholderText("����������");
		this->passSave = false;
		this->passSaved = "";
	}
}

void Login::on_login_clicked()
{
	QString user = ui->user->text();
	if (user.isEmpty()) {
		ui->message->setText("�û�������Ϊ��");
		ui->message->show();
		return;
	}
	QString password;
	if (!ui->password->text().isEmpty()) {
		QByteArray passtemp = ui->password->text().toUtf8();

		QByteArray passhash;
		RSASignature::hash512(passtemp, passhash);

		password = QString::fromUtf8(passhash);
	}
	else {
		if (this->passSave) {
			password = this->passSaved;
		}
		else {
			ui->message->setText("���벻��Ϊ��");
			ui->message->show();
			return;
		}
	}//��ȡ�û�������

	LinkPortClient linkport;
	linkport.init(this->config);//��ʼ��LinkPort

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_LOGIN);
	request.set_user(user.toStdString());
	request.set_password(password.toStdString());//׼�������

	QByteArray requestPack;
	if (!LPPack::packRequestWithPubilc(request, requestPack)) {
		ui->message->setText("������ʧ��");
		ui->message->show();
		return;
	}//������

	QByteArray answerPack;
	if (linkport.send(requestPack)) {
		answerPack = linkport.getResult();
	}
	else {
		ui->message->setText("�޷��������ͨ��");
		ui->message->show();
		return;
	}//�������ͨ��

	JC::Net::Answer answer;
	if (!LPPack::unpackAnswerWithPubilc(answerPack, answer)) {
		ui->message->setText("Ӧ����ʧ��");
		ui->message->show();
		return;
	}//Ӧ����

	if (answer.type() != JC::Net::Answer_Type::Answer_Type_DONE) {
		ui->message->setText(QString::fromStdString(answer.mesage()));
		ui->message->show();
		return;
	}//��ȡ��¼״̬

	if (ui->save->isChecked()) {
		neb::CJsonObject userData;
		userData.Add("name", user.toStdString());
		userData.Add("password", password.toStdString());
		if (!GetUser::setUser(userData)) {
			ui->message->setText("�޷�д���û�����");
			ui->message->show();
			return;
		}
	}//��������

	emit this->initWindow(this->config, user, password);
	if (answer.usertype() == JC::User_Type::User_Type_MANAGER) {
		emit this->showManager();
	}
	else {
		emit this->showUser();
	}//��ʾ��Ӧ����

	ui->message->setText("���״���");
	ui->message->hide();
	this->close();
	return;
}