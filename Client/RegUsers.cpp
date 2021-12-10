#include "RegUsers.h"
#include "ui_RegUsers.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

SendObject::SendObject(QObject* parent)
	: QObject(parent)
{

}

SendObject::~SendObject()
{

}

void SendObject::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
}

void SendObject::setList(QStringList list)
{
	this->idList = list;
}

void SendObject::start()
{
	LinkPortClient lpc;

	lpc.init(this->config);

	for (int i = 0; i < this->idList.size(); i++) {
		emit this->percents(i);

		QString userId = this->idList.at(i);
		emit this->warning("用户名:" + userId);

		JC::User newUser;
		newUser.set_id(userId.toStdString());
		newUser.set_type(JC::User_Type::User_Type_USER);

		QByteArray passdata = QString("12345678").toUtf8();
		QByteArray passhash;
		RSASignature::hash512(passdata, passhash);
		newUser.set_password(QString::fromUtf8(passhash).toStdString());

		JC::Net::Request request;
		request.set_type(JC::Net::Request_Type::Request_Type_ADDUSER);
		request.set_user(this->userName.toStdString());
		request.set_password(this->userPassword.toStdString());
		request.mutable_newuser()->CopyFrom(newUser);

		QByteArray data;
		LPPack::packRequestWithPubilc(request, data);

		if (!lpc.send(data)) {
			emit this->warning("无法与相应服务器通信");
			continue;
		}

		QByteArray answerData = lpc.getResult();
		JC::Net::Answer answer;
		LPPack::unpackAnswerWithPubilc(answerData, answer);

		if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
			emit this->warning("权限不足:" + QString::fromStdString(answer.mesage()));
			continue;
		}
		if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
			emit this->warning("出错:" + QString::fromStdString(answer.mesage()));
			continue;
		}

		emit this->warning("已注册:" + userId);
	}

	emit this->finished();
}

//

RegUsers::RegUsers(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::RegUsers();
	ui->setupUi(this);

	so = new SendObject(this);
	so->moveToThread(&r_thread);
	r_thread.start();

	connect(so, &SendObject::finished, this, &RegUsers::on_finished, Qt::ConnectionType::QueuedConnection);
	connect(so, &SendObject::percents, this, &RegUsers::on_percents, Qt::ConnectionType::QueuedConnection);
	connect(so, &SendObject::warning, this, &RegUsers::on_warning, Qt::ConnectionType::QueuedConnection);
	connect(this, &RegUsers::sigStart, so, &SendObject::start, Qt::ConnectionType::QueuedConnection);
}

RegUsers::~RegUsers()
{
	if (r_thread.isRunning()) {
		r_thread.terminate();
		r_thread.wait();
	}
	so->deleteLater();
	delete ui;
}

void RegUsers::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
	this->so->init(config, userName, userPassword);
}

void RegUsers::setList(QStringList list)
{
	this->idList = list;
	this->so->setList(list);
	ui->total->setText(QString::number(list.size()));
	ui->finished->setText("0");
	ui->bar->setValue(0);
	ui->output->clear();
}

void RegUsers::on_percents(int percent)
{
	ui->finished->setText(QString::number(percent));
	ui->bar->setValue(100 * ((double)percent / (double)this->idList.size()));
}

void RegUsers::on_warning(QString message)
{
	ui->output->appendPlainText(message);
}

void RegUsers::on_finished()
{
	this->close();
}

void RegUsers::on_start_clicked()
{
	emit this->sigStart();
}