#include "UserWindow.h"
#include "ui_UserWindow.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

UserWindow::UserWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::UserWindow();
	ui->setupUi(this);

	connect(&timer, &QTimer::timeout, this, &UserWindow::on_timerTimeOut);
	timer.start(1000);
}

UserWindow::~UserWindow()
{
	timer.stop();
	this->clearList();
	delete ui;
}

void UserWindow::init(neb::CJsonObject& config, QString userName, QString userPassword)
{
	this->config = config;
	this->userName = userName;
	this->userPassword = userPassword;
	this->refreshTask();
}

void UserWindow::on_actionRefresh_Classes_triggered(bool checked)
{
	Q_UNUSED(checked);
	if (this->refreshTask()) {
		QMessageBox::information(this, "ˢ��", "��ˢ���б�");
	}
}

void UserWindow::on_actionExport_triggered(bool checked)
{
	Q_UNUSED(checked);
	QString fileName = QFileDialog::getSaveFileName(this, "ѡ�񱣴�λ��", QDir::currentPath(), "csv����ļ�(*.csv)");
	if (fileName.isEmpty()) {
		return;
	}

	QFileInfo fi(fileName);
	QDir::setCurrent(fi.absolutePath());

	QString data;

	data += QString("\"�γ̱��\",\"�γ�����\",\"����\",\"��ѧʱ\",\"�ڿ�ѧʱ\",\"ʵ��ѧʱ\",\"ѧ��\",\"����ѧ��\"\r\n");

	for (int i = 0; i < ui->list->rowCount(); i++) {
		if (qobject_cast<QCheckBox*>(ui->list->cellWidget(i, 0))->isChecked()) {
			data += QString(ui->list->item(i, 1)->text()).prepend('\"').append('\"');
			for (int j = 2; j <= 8; j++) {
				data += ",";
				data += QString(ui->list->item(i, j)->text()).prepend('\"').append('\"');
			}
			data += "\r\n";
		}
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		QMessageBox::warning(this, "����", "�޷����ļ�");
		return;
	}
	file.write(data.toLocal8Bit());
	file.close();
	QMessageBox::information(this, "�����α�", "�ѵ����α���" + fileName);
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

void UserWindow::clearList()
{
	ui->list->setSortingEnabled(false);
	while (ui->list->rowCount() > 0) {
		for (int i = 1; i < ui->list->columnCount(); i++) {
			delete ui->list->takeItem(0, i);
		}
		ui->list->removeRow(0);
	}
	for (auto ck : this->checkList) {
		ck->deleteLater();
	}
	this->checkList.clear();
	ui->list->setSortingEnabled(true);
}

bool UserWindow::parseTask(JC::Student& student, QVector<JC::Class>& classes)
{
	this->clearList();
	ui->id->setText(QString::fromStdString(student.id()));
	ui->name->setText(QString::fromStdString(student.name()));

	QStringList SCList;
	for (int i = 0; i < student.classes_size(); i++) {
		SCList.append(QString::fromStdString(student.classes(i)));
	}

	ui->list->setSortingEnabled(false);
	int SCCount = 0;
	double CreditSum = 0;
	for (int i = 0; i < classes.size(); i++) {
		ui->list->insertRow(i);
		QCheckBox* cb = new QCheckBox(this);
		cb->setText("ѡ��");
		if (SCList.contains(QString::fromStdString(classes.at(i).id()))) {
			cb->setChecked(true);
			cb->setText("��ѡ��");
			SCCount++;
			CreditSum += classes.at(i).credit();
		}
		cb->setEnabled(classes.at(i).type() == JC::Class_Type::Class_Type_OPTIONAL);
		ui->list->setCellWidget(i, 0, cb);
		connect(cb, &QCheckBox::toggled, this, &UserWindow::on_checkBoxToggled);
		this->checkList.append(cb);

		QTableWidgetItem* item = nullptr;

		item = new QTableWidgetItem(QString::fromStdString(classes.at(i).id()));
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 1, item);//���

		item = new QTableWidgetItem(QString::fromStdString(classes.at(i).name()));
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 2, item);//����

		switch (classes.at(i).type())
		{
		case JC::Class_Type::Class_Type_PUBLIC:
			item = new QTableWidgetItem("������");
			break;
		case JC::Class_Type::Class_Type_REQUIRED:
			item = new QTableWidgetItem("רҵ���޿�");
			break;
		case JC::Class_Type::Class_Type_OPTIONAL:
			item = new QTableWidgetItem("ѡ�޿�");
			break;
		}
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 3, item);//����

		item = new QTableWidgetItem(QString::number(classes.at(i).timeclass() + classes.at(i).timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 4, item);//��ѧʱ

		item = new QTableWidgetItem(QString::number(classes.at(i).timeclass()));
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 5, item);//�ڿ�ѧʱ

		item = new QTableWidgetItem(QString::number(classes.at(i).timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 6, item);//ʵ��ѧʱ

		item = new QTableWidgetItem(QString::number(classes.at(i).credit()));
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 7, item);//ѧ��

		item = new QTableWidgetItem(QString::fromStdString(classes.at(i).term()));
		if (item == nullptr) {
			QMessageBox::critical(this, "��������", "�ڴ����ʧ��");
			ui->list->setSortingEnabled(true);
			return false;
		}
		ui->list->setItem(i, 8, item);//����ѧ��
	}
	ui->list->setSortingEnabled(true);

	ui->count->setText(QString::number(SCCount));
	ui->credit->setText(QString::number(CreditSum));
	ui->submit->setText("���ύ");
	ui->submit->setEnabled(false);
	ui->refreshTime->setText("ˢ��ʱ�� " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	ui->refreshTime->show();
	ui->actionExport->setEnabled(true);

	return true;
}

bool UserWindow::refreshTask()
{
	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETSTUDENT);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());

	QByteArray data;
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "����", "�޷�����Ӧ������ͨ��");
		return false;
	}

	QByteArray answerData = lpc.getResult();
	JC::Net::Answer answer;
	LPPack::unpackAnswerWithPubilc(answerData, answer);

	if (answer.type() == JC::Net::Answer_Type::Answer_Type_DENIED) {
		QMessageBox::warning(this, "Ȩ�޲���", QString::fromStdString(answer.mesage()));
		return false;
	}
	if (answer.type() == JC::Net::Answer_Type::Answer_Type_ERROR) {
		QMessageBox::warning(this, "����", QString::fromStdString(answer.mesage()));
		return false;
	}

	JC::Student student = answer.student();
	QVector<JC::Class> classList;
	for (int i = 0; i < answer.classes_size(); i++) {
		classList.append(answer.classes(i));
	}

	
	return this->parseTask(student, classList);
}

void UserWindow::on_checkBoxToggled(bool checked)
{
	Q_UNUSED(checked);
	int SCCount = 0;
	double CreditSum = 0;
	for (int i = 0; i < ui->list->rowCount(); i++) {
		if (qobject_cast<QCheckBox*>(ui->list->cellWidget(i, 0))->isChecked()) {
			qobject_cast<QCheckBox*>(ui->list->cellWidget(i, 0))->setText("��ѡ��");
			SCCount++;
			CreditSum += ui->list->item(i, 7)->text().toDouble();
		}
		else {
			qobject_cast<QCheckBox*>(ui->list->cellWidget(i, 0))->setText("ѡ��");
		}
	}
	ui->count->setText(QString::number(SCCount));
	ui->credit->setText(QString::number(CreditSum));
	ui->submit->setText("�ύ����");
	ui->submit->setEnabled(true);
	ui->refreshTime->setText("ˢ��ʱ�� 0000-00-00 00:00:00");
	ui->refreshTime->hide();
	ui->actionExport->setEnabled(false);
}

void UserWindow::on_submit_clicked()
{
	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_JOINCLASS);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	
	for (int i = 0; i < ui->list->rowCount(); i++) {
		if (qobject_cast<QCheckBox*>(ui->list->cellWidget(i, 0))->isChecked()) {
			request.mutable_classes()->Add(ui->list->item(i, 1)->text().toStdString());
		}
	}

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

	QMessageBox::information(this, "�ύ", "���ύѡ��");
	this->refreshTask();
}

void UserWindow::on_timerTimeOut()
{
	ui->time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	this->update();
}

void UserWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPalette pal = this->palette();
	QBrush brush = pal.window();
	QColor col = brush.color();
	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setColor(QColor(col.red() + 2, col.green() + 2, col.blue() + 2));
	painter.setPen(pen);
	QFont font;
	font.setPixelSize(height() / 50);
	painter.setFont(font);
	QString times = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	for (int i = 0; i < 50; i++) {
		if (i % 2) {
			painter.drawText(QPointF(0.05 * width(), i * (double)((double)height() / (double)50)), "name:" + this->userName);
		}
		else {
			painter.drawText(QPointF(0.05 * width(), i * (double)((double)height() / (double)50)), "time:" + times);
		}
	}
	for (int i = 0; i < 50; i++) {
		if (i % 2) {
			painter.drawText(QPointF(0.55 * width(), i * (double)((double)height() / (double)50)), "name:" + this->userName);
		}
		else {
			painter.drawText(QPointF(0.55 * width(), i * (double)((double)height() / (double)50)), "time:" + times);
		}
	}
	int gx = ui->groupBox->geometry().x();
	int xPos = gx + ui->groupBox->width() * 0.1;
	for (int i = 0; i < 50; i++) {
		if (i % 2) {
			painter.drawText(QPointF(xPos, i * (double)((double)height() / (double)50)), "name:" + this->userName);
		}
		else {
			painter.drawText(QPointF(xPos, i * (double)((double)height() / (double)50)), "time:" + times);
		}
	}
	QWidget::paintEvent(event);
}