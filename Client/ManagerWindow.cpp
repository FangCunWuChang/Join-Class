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

	connect(&timer, &QTimer::timeout, this, &ManagerWindow::on_timerTimeOut);
	timer.start(1000);
}

ManagerWindow::~ManagerWindow()
{
	timer.stop();
	this->clearClasses();
	this->clearStudents();
	this->clearUsers();
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
			if (ui->classes->item(i, 0)->text() == QString::fromStdString(clas.id())) {
				QMessageBox::warning(this, "添加课程", "课程已存在");
				return;
			}
		}

		ui->classes->setSortingEnabled(false);
		ui->classes->insertRow(currentIndex + 1);

		QTableWidgetItem* item = nullptr;

		item = new QTableWidgetItem(QString::fromStdString(clas.id()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex + 1, 0, item);//编号

		item = new QTableWidgetItem(QString::fromStdString(clas.name()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
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
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex + 1, 2, item);//类型

		item = new QTableWidgetItem(QString::number(clas.timeclass() + clas.timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex + 1, 3, item);//总学时

		item = new QTableWidgetItem(QString::number(clas.timeclass()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex + 1, 4, item);//授课学时

		item = new QTableWidgetItem(QString::number(clas.timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex + 1, 5, item);//实验学时

		item = new QTableWidgetItem(QString::number(clas.credit()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex + 1, 6, item);//学分

		item = new QTableWidgetItem(QString::fromStdString(clas.term()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
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
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex + 1, 8, item);//选课人数

		ui->classes->setSortingEnabled(true);

		for (int i = 0; i < ui->students->rowCount(); i++) {
			bool changeFlag = false;
			QString SClasses = ui->students->item(i, 4)->text();
			QStringList SCList = SClasses.split(';', Qt::SkipEmptyParts);
			if (clas.type() != JC::Class_Type::Class_Type_OPTIONAL) {
				if (!SCList.contains(QString::fromStdString(clas.id()))) {
					SCList.append(QString::fromStdString(clas.id()));
				}
				changeFlag = true;
			}
			else {
				if (SCList.contains(QString::fromStdString(clas.id()))) {
					SCList.removeAll(QString::fromStdString(clas.id()));
					changeFlag = true;
				}
			}
			ui->students->setSortingEnabled(false);
			ui->students->item(i, 4)->setText(SCList.join(';'));
			ui->students->setSortingEnabled(true);
			if (changeFlag) {
				this->resetCredit(i);
			}
		}
	}
}

void ManagerWindow::on_editClass_clicked()
{
	int currentIndex = ui->classes->currentRow();

	JC::Class clas;
	clas.set_id(ui->classes->item(currentIndex, 0)->text().toStdString());
	clas.set_name(ui->classes->item(currentIndex, 1)->text().toStdString());
	if (ui->classes->item(currentIndex, 2)->text() == "公共课") {
		clas.set_type(JC::Class_Type::Class_Type_PUBLIC);
	}
	else if (ui->classes->item(currentIndex, 2)->text() == "专业必修课") {
		clas.set_type(JC::Class_Type::Class_Type_REQUIRED);
	}
	else {
		clas.set_type(JC::Class_Type::Class_Type_OPTIONAL);
	}
	clas.set_timeclass(ui->classes->item(currentIndex, 4)->text().toDouble());
	clas.set_timeexper(ui->classes->item(currentIndex, 5)->text().toDouble());
	clas.set_credit(ui->classes->item(currentIndex, 6)->text().toDouble());
	clas.set_term(ui->classes->item(currentIndex, 7)->text().toStdString());

	EditClass ec(this);
	ec.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	ec.init(clas);
	ec.exec();

	if (ec.isAccepted()) {
		clas = ec.getClass();

		ui->classes->setSortingEnabled(false);

		ui->classes->item(currentIndex, 0)->setText(QString::fromStdString(clas.id()));//编号
		ui->classes->item(currentIndex, 1)->setText(QString::fromStdString(clas.name()));//名称
		switch (clas.type())
		{
		case JC::Class_Type::Class_Type_PUBLIC:
			ui->classes->item(currentIndex, 2)->setText("公共课");
			break;
		case JC::Class_Type::Class_Type_REQUIRED:
			ui->classes->item(currentIndex, 2)->setText("专业必修课");
			break;
		case JC::Class_Type::Class_Type_OPTIONAL:
			ui->classes->item(currentIndex, 2)->setText("选修课");
			break;
		}//类型
		ui->classes->item(currentIndex, 3)->setText(QString::number(clas.timeclass() + clas.timeexper()));//总学时
		ui->classes->item(currentIndex, 4)->setText(QString::number(clas.timeclass()));//授课学时
		ui->classes->item(currentIndex, 5)->setText(QString::number(clas.timeexper()));//实验学时
		ui->classes->item(currentIndex, 6)->setText(QString::number(clas.credit()));//学分
		ui->classes->item(currentIndex, 7)->setText(QString::fromStdString(clas.term()));//开课学期

		ui->classes->setSortingEnabled(true);

		int SCount = 0;
		for (int i = 0; i < ui->students->rowCount(); i++) {
			bool changeFlag = false;
			QString SClasses = ui->students->item(i, 4)->text();
			QStringList SCList = SClasses.split(';', Qt::SkipEmptyParts);
			if (clas.type() != JC::Class_Type::Class_Type_OPTIONAL) {
				if (!SCList.contains(QString::fromStdString(clas.id()))) {
					SCList.append(QString::fromStdString(clas.id()));
				}
				changeFlag = true;
			}
			else {
				if (SCList.contains(QString::fromStdString(clas.id()))) {
					SCount++;
					changeFlag = true;
				}
			}
			ui->students->setSortingEnabled(false);
			ui->students->item(i, 4)->setText(SCList.join(';'));
			ui->students->setSortingEnabled(true);
			if (changeFlag) {
				this->resetCredit(i);
			}
		}

		ui->classes->setSortingEnabled(false);
		currentIndex = ui->classes->currentRow();

		switch (clas.type())
		{
		case JC::Class_Type::Class_Type_PUBLIC:
			ui->classes->item(currentIndex, 8)->setText(QString::number(ui->students->rowCount()));
			break;
		case JC::Class_Type::Class_Type_REQUIRED:
			ui->classes->item(currentIndex, 8)->setText(QString::number(ui->students->rowCount()));
			break;
		case JC::Class_Type::Class_Type_OPTIONAL:
			ui->classes->item(currentIndex, 8)->setText(QString::number(SCount));
			break;
		}//选课人数

		ui->classes->setSortingEnabled(true);
	}
}

void ManagerWindow::on_removeClass_clicked()
{
	ui->classes->setSortingEnabled(false);
	int currentIndex = ui->classes->currentRow();
	QString id = ui->classes->item(currentIndex, 0)->text();

	for (int i = 0; i < ui->classes->columnCount(); i++) {
		delete ui->classes->takeItem(currentIndex, i);
	}
	ui->classes->removeRow(currentIndex);
	
	ui->classes->setSortingEnabled(true);

	for (int i = 0; i < ui->students->rowCount(); i++) {
		bool changeFlag = false;
		QString SClasses = ui->students->item(i, 4)->text();
		QStringList SCList = SClasses.split(';', Qt::SkipEmptyParts);
		if (SCList.contains(id)) {
			SCList.removeAll(id);
			changeFlag = true;
		}
		ui->students->setSortingEnabled(false);
		ui->students->item(i, 4)->setText(SCList.join(';'));
		ui->students->setSortingEnabled(true);
		if (changeFlag) {
			this->resetCredit(i);
		}
	}

	if (ui->classes->rowCount() == 0) {
		ui->editClass->setEnabled(false);
		ui->removeClass->setEnabled(false);
		ui->viewClass->setEnabled(false);
	}
}

void ManagerWindow::on_viewClass_clicked()
{
	int currentIndex = ui->classes->currentRow();

	JC::Class clas;
	clas.set_id(ui->classes->item(currentIndex, 0)->text().toStdString());
	clas.set_name(ui->classes->item(currentIndex, 1)->text().toStdString());
	if (ui->classes->item(currentIndex, 2)->text() == "公共课") {
		clas.set_type(JC::Class_Type::Class_Type_PUBLIC);
	}
	else if (ui->classes->item(currentIndex, 2)->text() == "专业必修课") {
		clas.set_type(JC::Class_Type::Class_Type_REQUIRED);
	}
	else {
		clas.set_type(JC::Class_Type::Class_Type_OPTIONAL);
	}
	clas.set_timeclass(ui->classes->item(currentIndex, 4)->text().toDouble());
	clas.set_timeexper(ui->classes->item(currentIndex, 5)->text().toDouble());
	clas.set_credit(ui->classes->item(currentIndex, 6)->text().toDouble());
	clas.set_term(ui->classes->item(currentIndex, 7)->text().toStdString());

	QStringList idL, nL;
	for (int i = 0; i < ui->students->rowCount(); i++) {
		QString SClasses = ui->students->item(i, 4)->text();
		QStringList SCList = SClasses.split(';', Qt::SkipEmptyParts);
		if (SCList.contains(ui->classes->item(currentIndex,0)->text())) {
			idL.append(ui->students->item(i, 0)->text());
			nL.append(ui->students->item(i, 1)->text());
		}
	}

	ViewClass vc(this);
	vc.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	vc.init(clas, idL, nL);
	vc.exec();
}

void ManagerWindow::on_addStudent_clicked()
{
	int currentIndex = ui->students->currentRow();
	AddStudent as(this);
	as.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	as.exec();
	if (as.isAccepted()) {
		JC::Student student = as.getStudent();
		for (int i = 0; i < ui->students->rowCount(); i++) {
			if (ui->students->item(i, 0)->text() == QString::fromStdString(student.id())) {
				QMessageBox::warning(this, "添加学生", "学生已存在");
				return;
			}
		}

		ui->students->setSortingEnabled(false);
		ui->students->insertRow(currentIndex + 1);

		QTableWidgetItem* item = nullptr;

		item = new QTableWidgetItem(QString::fromStdString(student.id()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex + 1, 0, item);//学号

		item = new QTableWidgetItem(QString::fromStdString(student.name()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex + 1, 1, item);//姓名

		ui->students->setSortingEnabled(true);

		int classCount = 0;
		double creditSum = 0;
		QStringList claList;
		for (int i = 0; i < ui->classes->rowCount(); i++) {
			if (ui->classes->item(i, 2)->text() != "选修课") {
				classCount++;
				creditSum += ui->classes->item(i, 6)->text().toDouble();
				claList.append(ui->classes->item(i, 0)->text());

				ui->classes->setSortingEnabled(false);
				ui->classes->item(i, 8)->setText(QString::number(ui->classes->item(i, 8)->text().toInt() + 1));
				ui->classes->setSortingEnabled(true);
			}
		}

		ui->students->setSortingEnabled(false);
		currentIndex = ui->students->currentRow();

		item = new QTableWidgetItem(QString::number(classCount));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex + 1, 2, item);//选课数量

		item = new QTableWidgetItem(QString::number(creditSum));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex + 1, 3, item);//学分数

		item = new QTableWidgetItem(claList.join(';'));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex + 1, 4, item);//所选课程

		ui->students->setSortingEnabled(true);
	}
}

void ManagerWindow::on_editStudent_clicked()
{
	int currentIndex = ui->students->currentRow();

	JC::Student student;
	student.set_id(ui->students->item(currentIndex, 0)->text().toStdString());
	student.set_name(ui->students->item(currentIndex, 1)->text().toStdString());

	EditStudent es(this);
	es.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	es.init(student);
	es.exec();

	if (es.isAccepted()) {
		student = es.getStudent();

		ui->students->setSortingEnabled(false);

		ui->students->item(currentIndex, 0)->setText(QString::fromStdString(student.id()));//学号
		ui->students->item(currentIndex, 1)->setText(QString::fromStdString(student.name()));//姓名

		ui->classes->setSortingEnabled(true);
	}
}

void ManagerWindow::on_removeStudent_clicked()
{
	ui->students->setSortingEnabled(false);
	int currentIndex = ui->students->currentRow();
	QStringList classList = ui->students->item(currentIndex, 4)->text().split(';', Qt::SkipEmptyParts);

	for (int i = 0; i < ui->students->columnCount(); i++) {
		delete ui->students->takeItem(currentIndex, i);
	}
	ui->students->removeRow(currentIndex);

	ui->students->setSortingEnabled(true);

	for (auto& s : classList) {
		if (!s.isEmpty()) {
			for (int i = 0; i < ui->classes->rowCount(); i++) {
				if (ui->classes->item(i, 0)->text() == s) {
					ui->classes->setSortingEnabled(false);
					ui->classes->item(i, 8)->setText(QString::number(ui->classes->item(i, 8)->text().toInt() - 1));
					ui->classes->setSortingEnabled(true);
					break;
				}
			}
		}
	}

	if (ui->students->rowCount() == 0) {
		ui->editStudent->setEnabled(false);
		ui->removeStudent->setEnabled(false);
		ui->viewStudent->setEnabled(false);
	}
}

void ManagerWindow::on_viewStudent_clicked()
{
	int currentIndex = ui->students->currentRow();

	JC::Student student;
	student.set_id(ui->students->item(currentIndex, 0)->text().toStdString());
	student.set_name(ui->students->item(currentIndex, 1)->text().toStdString());

	QVector<JC::Class> claList;
	QStringList classList = ui->students->item(currentIndex, 4)->text().split(';', Qt::SkipEmptyParts);
	for (int i = 0; i < ui->classes->rowCount(); i++) {
		if (classList.contains(ui->classes->item(i, 0)->text())) {
			JC::Class clas;
			clas.set_id(ui->classes->item(i, 0)->text().toStdString());
			clas.set_name(ui->classes->item(i, 1)->text().toStdString());
			if (ui->classes->item(i, 2)->text() == "公共课") {
				clas.set_type(JC::Class_Type::Class_Type_PUBLIC);
			}
			else if (ui->classes->item(i, 2)->text() == "专业必修课") {
				clas.set_type(JC::Class_Type::Class_Type_REQUIRED);
			}
			else {
				clas.set_type(JC::Class_Type::Class_Type_OPTIONAL);
			}
			clas.set_timeclass(ui->classes->item(i, 4)->text().toDouble());
			clas.set_timeexper(ui->classes->item(i, 5)->text().toDouble());
			clas.set_credit(ui->classes->item(i, 6)->text().toDouble());
			clas.set_term(ui->classes->item(i, 7)->text().toStdString());
			claList.append(clas);
		}
	}

	ViewStudent vs(this);
	vs.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	vs.init(student, claList);
	vs.exec();
}

void ManagerWindow::on_refreshUser_clicked()
{
	this->refreshUserList();
}

void ManagerWindow::on_addUser_clicked()
{
	bool idOk = false;
	QString userId = QInputDialog::getText(
		this,
		"添加用户",
		"输入新用户名",
		QLineEdit::Normal,
		QString(),
		&idOk,
		Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint
	);
	if (!idOk) {
		return;
	}

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETUSERLIST);
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

	QStringList userList;
	for (int i = 0; i < answer.userlist_size(); i++) {
		userList.append(QString::fromStdString(answer.userlist(i).id()));
	}

	if (userId.isEmpty()) {
		QMessageBox::warning(this, "出错", "用户名不得为空");
		return;
	}
	for (auto& c : userId) {
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			QMessageBox::warning(this, "出错", "用户名需由数字或英文大小写字母构成");
			return;
		}
	}
	if (userList.contains(userId)) {
		QMessageBox::warning(this, "出错", "用户已存在");
		return;
	}

	bool typeOk = false;
	QString userType = QInputDialog::getItem(
		this,
		"添加用户",
		"选择用户身份",
		{ "用户","管理员" },
		0,
		false,
		&typeOk,
		Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint
	);
	if (!typeOk) {
		return;
	}

	JC::User newUser;
	newUser.set_id(userId.toStdString());
	newUser.set_type((userType == "管理员") ? JC::User_Type::User_Type_MANAGER : JC::User_Type::User_Type_USER);

	QByteArray passdata = QString("12345678").toUtf8();
	QByteArray passhash;
	RSASignature::hash512(passdata, passhash);
	newUser.set_password(QString::fromUtf8(passhash).toStdString());

	request.Clear();
	request.set_type(JC::Net::Request_Type::Request_Type_ADDUSER);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	request.mutable_newuser()->CopyFrom(newUser);

	data.clear();
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "出错", "无法与相应服务器通信");
		return;
	}

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

	this->refreshUserList();
}

void ManagerWindow::on_removeUser_clicked()
{
	QString userId = ui->users->item(ui->users->currentRow(), 0)->text();

	JC::User newUser;
	newUser.set_id(userId.toStdString());

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_DELUSER);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	request.mutable_newuser()->CopyFrom(newUser);

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

	this->refreshUserList();
}

void ManagerWindow::on_resetUser_clicked()
{
	QString userId = ui->users->item(ui->users->currentRow(), 0)->text();
	QString userType = ui->users->item(ui->users->currentRow(), 1)->text();

	JC::User newUser;
	newUser.set_id(userId.toStdString());
	newUser.set_type((userType == "管理员") ? JC::User_Type::User_Type_MANAGER : JC::User_Type::User_Type_USER);

	QByteArray passdata = QString("12345678").toUtf8();
	QByteArray passhash;
	RSASignature::hash512(passdata, passhash);
	newUser.set_password(QString::fromUtf8(passhash).toStdString());

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_CHANGEUSER);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	request.mutable_newuser()->CopyFrom(newUser);

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

	this->refreshUserList();

	QMessageBox::information(this, "重置密码", "已重置用户 " + userId + " 的密码为默认密码 12345678");
}

void ManagerWindow::on_typeUser_clicked()
{
	QString userId = ui->users->item(ui->users->currentRow(), 0)->text();
	QString userType = ui->users->item(ui->users->currentRow(), 1)->text();
	bool typeOk = false;
	QString newUserType = QInputDialog::getItem(
		this,
		"添加用户",
		"选择用户身份",
		{ "用户","管理员" },
		(userType == "用户") ? 0 : 1,
		false,
		&typeOk,
		Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint
	);
	if (!typeOk) {
		return;
	}
	if (newUserType == userType) {
		QMessageBox::information(this, "修改用户权限", "用户 " + userId + " 权限已经是" + userType);
		return;
	}

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETUSERLIST);
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

	QStringList userList;
	for (int i = 0; i < answer.userlist_size(); i++) {
		userList.append(QString::fromStdString(answer.userlist(i).id()));
	}

	if (!userList.contains(userId)) {
		QMessageBox::warning(this, "出错", "用户 " + userId + " 不存在");
		this->refreshUserList();
		return;
	}
	QString passNow = QString::fromStdString(answer.userlist(userList.indexOf(userId)).password());

	JC::User newUser;
	newUser.set_id(userId.toStdString());
	newUser.set_password(passNow.toStdString());
	newUser.set_type((newUserType == "用户") ? JC::User_Type::User_Type_USER : JC::User_Type::User_Type_MANAGER);

	request.Clear();
	request.set_type(JC::Net::Request_Type::Request_Type_CHANGEUSER);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	request.mutable_newuser()->CopyFrom(newUser);

	data.clear();
	LPPack::packRequestWithPubilc(request, data);

	if (!lpc.send(data)) {
		QMessageBox::warning(this, "出错", "无法与相应服务器通信");
		return;
	}

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

	this->refreshUserList();

	QMessageBox::information(this, "修改用户权限", "已设置用户 " + userId + " 身份为" + ((newUser.type() == JC::User_Type::User_Type_MANAGER) ? QString("管理员") : QString("用户")));
}

void ManagerWindow::on_regUser_clicked()
{
	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETUSERLIST);
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

	QStringList userList;
	for (int i = 0; i < answer.userlist_size(); i++) {
		userList.append(QString::fromStdString(answer.userlist(i).id()));
	}

	QStringList newList;
	for (int i = 0; i < ui->students->rowCount(); i++) {
		if (!userList.contains(ui->students->item(i, 0)->text())) {
			newList.append(ui->students->item(i, 0)->text());
		}
	}

	if (newList.isEmpty()) {
		QMessageBox::information(this, "批量注册", "无需任何操作");
		return;
	}

	RegUsers ru(this);
	ru.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
	ru.init(this->config, this->userName, this->userPassword);
	ru.setList(newList);
	ru.exec();

	this->refreshUserList();

	QMessageBox::information(this, "批量注册", "完成注册");
}


void ManagerWindow::resetCredit(int row)
{
	QString SClasses = ui->students->item(row, 4)->text();
	QStringList SCList = SClasses.split(';', Qt::SkipEmptyParts);

	auto getCredit = [&](QString name)->double {
		for (int i = 0; i < ui->classes->rowCount(); i++) {
			if (ui->classes->item(i, 0)->text() == name) {
				return ui->classes->item(i, 6)->text().toDouble();
			}
		}
		return 0;
	};

	int SCCount = 0;
	double CDSum = 0;
	for (auto& s : SCList) {
		if (!s.isEmpty()) {
			SCCount++;
			CDSum += getCredit(s);
		}
	}

	ui->students->setSortingEnabled(false);
	ui->students->item(row, 2)->setText(QString::number(SCCount));
	ui->students->item(row, 3)->setText(QString::number(CDSum));
	ui->students->setSortingEnabled(true);
}

void ManagerWindow::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	this->clearClasses();
	this->clearStudents();
	this->clearUsers();
	ui->tabWidget->setCurrentIndex(0);
}

void ManagerWindow::on_tabWidget_currentChanged(int index)
{
	if (index == 2) {
		this->refreshUserList();
	}
}

void ManagerWindow::clearClasses()
{
	ui->classes->setSortingEnabled(false);
	while (ui->classes->rowCount() > 0) {
		for (int i = 0; i < ui->classes->columnCount(); i++) {
			delete ui->classes->takeItem(0, i);
		}
		ui->classes->removeRow(0);
	}
	ui->editClass->setEnabled(false);
	ui->removeClass->setEnabled(false);
	ui->viewClass->setEnabled(false);
	ui->classes->setSortingEnabled(true);
}

void ManagerWindow::clearStudents()
{
	ui->students->setSortingEnabled(false);
	while (ui->students->rowCount() > 0) {
		for (int i = 0; i < ui->students->columnCount(); i++) {
			delete ui->students->takeItem(0, i);
		}
		ui->students->removeRow(0);
	}
	ui->editStudent->setEnabled(false);
	ui->removeStudent->setEnabled(false);
	ui->viewStudent->setEnabled(false);
	ui->students->setSortingEnabled(true);
}

void ManagerWindow::clearUsers()
{
	ui->users->setSortingEnabled(false);
	while (ui->users->rowCount() > 0) {
		for (int i = 0; i < ui->users->columnCount(); i++) {
			delete ui->users->takeItem(0, i);
		}
		ui->users->removeRow(0);
	}
	ui->removeUser->setEnabled(false);
	ui->resetUser->setEnabled(false);
	ui->typeUser->setEnabled(false);
	ui->users->setSortingEnabled(true);
}

void ManagerWindow::refreshUserList()
{
	this->clearUsers();

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETUSERLIST);
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

	ui->users->setSortingEnabled(false);
	for (int i = 0; i < answer.userlist_size(); i++) {
		ui->users->insertRow(i);
		QTableWidgetItem* item = nullptr;

		item = new QTableWidgetItem(QString::fromStdString(answer.userlist(i).id()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->users->setSortingEnabled(true);
			return;
		}
		ui->users->setItem(i, 0, item);//用户名

		item = new QTableWidgetItem((answer.userlist(i).type() == JC::User_Type::User_Type_MANAGER) ? QString("管理员") : QString("用户"));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->users->setSortingEnabled(true);
			return;
		}
		ui->users->setItem(i, 1, item);//用户身份
	}
	ui->users->setSortingEnabled(true);
}

void ManagerWindow::on_actionChange_Password_triggered(bool checked)
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

void ManagerWindow::on_actionLogout_triggered(bool checked)
{
	Q_UNUSED(checked);
	emit this->initLogin(this->config);
	emit this->showLogin();
	this->close();
}

void ManagerWindow::on_actionAbout_triggered(bool checked)
{
	Q_UNUSED(checked);
	QMessageBox::about(this, "关于选课平台", ::_JC_Message);
}

void ManagerWindow::on_actionAbout_Qt_triggered(bool checked)
{
	Q_UNUSED(checked);
	QMessageBox::aboutQt(this, "关于Qt");
}

void ManagerWindow::on_actionGet_Task_triggered(bool checked)
{
	Q_UNUSED(checked);
	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_GETTASK);
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

	this->clearClasses();
	this->clearStudents();
	JC::Task task = answer.task();

	this->parseTask(task);
	QMessageBox::information(this, "拉取任务", "已从服务器拉取任务");
}

void ManagerWindow::on_actionSet_Task_triggered(bool checked)
{
	Q_UNUSED(checked);
	QMessageBox::StandardButton result = QMessageBox::warning(this, "推送任务", "确定将本地任务推送至服务端？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (result != QMessageBox::Yes) {
		return;
	}

	JC::Task task;

	for (int i = 0; i < ui->classes->rowCount(); i++) {
		JC::Class* clas = task.add_classes();

		clas->set_id(ui->classes->item(i, 0)->text().toStdString());
		clas->set_name(ui->classes->item(i, 1)->text().toStdString());
		if (ui->classes->item(i, 2)->text() == "公共课") {
			clas->set_type(JC::Class_Type::Class_Type_PUBLIC);
		}
		else if (ui->classes->item(i, 2)->text() == "专业必修课") {
			clas->set_type(JC::Class_Type::Class_Type_REQUIRED);
		}
		else {
			clas->set_type(JC::Class_Type::Class_Type_OPTIONAL);
		}
		clas->set_timeclass(ui->classes->item(i, 4)->text().toDouble());
		clas->set_timeexper(ui->classes->item(i, 5)->text().toDouble());
		clas->set_credit(ui->classes->item(i, 6)->text().toDouble());
		clas->set_term(ui->classes->item(i, 7)->text().toStdString());
	}
	for (int i = 0; i < ui->students->rowCount(); i++) {
		JC::Student* student = task.add_students();

		student->set_id(ui->students->item(i, 0)->text().toStdString());
		student->set_name(ui->students->item(i, 1)->text().toStdString());

		QStringList claList = ui->students->item(i, 4)->text().split(';', Qt::SkipEmptyParts);
		for (auto& s : claList) {
			student->mutable_classes()->Add(s.toStdString());
		}
	}

	LinkPortClient lpc;

	lpc.init(this->config);

	JC::Net::Request request;
	request.set_type(JC::Net::Request_Type::Request_Type_SETTASK);
	request.set_user(this->userName.toStdString());
	request.set_password(this->userPassword.toStdString());
	request.mutable_task()->CopyFrom(task);

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

	QMessageBox::information(this, "推送任务", "已推送任务至服务器");
}

void ManagerWindow::on_actionNew_Task_triggered(bool checked)
{
	Q_UNUSED(checked);
	this->clearClasses();
	this->clearStudents();
}

void ManagerWindow::on_actionSave_Task_triggered(bool checked)
{
	Q_UNUSED(checked);
	JC::Task task;

	for (int i = 0; i < ui->classes->rowCount(); i++) {
		JC::Class* clas = task.add_classes();

		clas->set_id(ui->classes->item(i, 0)->text().toStdString());
		clas->set_name(ui->classes->item(i, 1)->text().toStdString());
		if (ui->classes->item(i, 2)->text() == "公共课") {
			clas->set_type(JC::Class_Type::Class_Type_PUBLIC);
		}
		else if (ui->classes->item(i, 2)->text() == "专业必修课") {
			clas->set_type(JC::Class_Type::Class_Type_REQUIRED);
		}
		else {
			clas->set_type(JC::Class_Type::Class_Type_OPTIONAL);
		}
		clas->set_timeclass(ui->classes->item(i, 4)->text().toDouble());
		clas->set_timeexper(ui->classes->item(i, 5)->text().toDouble());
		clas->set_credit(ui->classes->item(i, 6)->text().toDouble());
		clas->set_term(ui->classes->item(i, 7)->text().toStdString());
	}
	for (int i = 0; i < ui->students->rowCount(); i++) {
		JC::Student* student = task.add_students();

		student->set_id(ui->students->item(i, 0)->text().toStdString());
		student->set_name(ui->students->item(i, 1)->text().toStdString());

		QStringList claList = ui->students->item(i, 4)->text().split(';', Qt::SkipEmptyParts);
		for (auto& s : claList) {
			student->mutable_classes()->Add(s.toStdString());
		}
	}

	QString filename = QFileDialog::getSaveFileName(
		this,
		"保存任务",
		QDir::currentPath(),
		"jctaskdata文件(*.jctaskdata)"
	);
	if (filename.isEmpty()) {
		return;
	}
	QFileInfo filei(filename);
	QDir::setCurrent(filei.absolutePath());

	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		QMessageBox::warning(this, "出错", "无法保存文件:" + filename);
		return;
	}
	
	QByteArray data(task.ByteSize(), 0);
	task.SerializeToArray(data.data(), data.size());
	file.write(data);
	file.close();

	QMessageBox::information(this, "保存任务", "已保存文件:" + filename);
}

void ManagerWindow::on_actionLoad_Task_triggered(bool checked)
{
	Q_UNUSED(checked);
	QString filename = QFileDialog::getOpenFileName(
		this,
		"打开任务",
		QDir::currentPath(),
		"jctaskdata文件(*.jctaskdata)"
	);
	if (filename.isEmpty()) {
		return;
	}
	QFileInfo filei(filename);
	QDir::setCurrent(filei.absolutePath());

	QFile file(filename);
	if (!file.open(QIODevice::ReadWrite)) {
		QMessageBox::warning(this, "出错", "无法打开文件:" + filename);
		return;
	}

	QByteArray data = file.readAll();
	file.close();

	JC::Task task;
	task.ParseFromArray(data.constData(), data.size());

	this->clearClasses();
	this->clearStudents();
	this->parseTask(task);

	QMessageBox::information(this, "打开任务", "已载入文件:" + filename);
}

void ManagerWindow::parseTask(JC::Task& task)
{
	ui->classes->setSortingEnabled(false);
	for (int i = task.classes_size() - 1; i >= 0; i--) {
		JC::Class clas = task.classes(i);

		int currentIndex = ui->classes->currentRow() + 1;
		ui->classes->insertRow(currentIndex);

		QTableWidgetItem* item = nullptr;

		item = new QTableWidgetItem(QString::fromStdString(clas.id()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 0, item);//编号

		item = new QTableWidgetItem(QString::fromStdString(clas.name()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 1, item);//名称

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
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 2, item);//类型

		item = new QTableWidgetItem(QString::number(clas.timeclass() + clas.timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 3, item);//总学时

		item = new QTableWidgetItem(QString::number(clas.timeclass()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 4, item);//授课学时

		item = new QTableWidgetItem(QString::number(clas.timeexper()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 5, item);//实验学时

		item = new QTableWidgetItem(QString::number(clas.credit()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 6, item);//学分

		item = new QTableWidgetItem(QString::fromStdString(clas.term()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 7, item);//开课学期

		int stuCount = 0;
		for (int j = 0; j < task.students_size(); j++) {
			QStringList classList;
			for (auto& s : task.students(j).classes()) {
				classList.append(QString::fromStdString(s));
			}
			if (classList.contains(QString::fromStdString(clas.id()))) {
				stuCount++;
			}
		}
		item = new QTableWidgetItem(QString::number(stuCount));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->classes->setSortingEnabled(true);
			return;
		}
		ui->classes->setItem(currentIndex, 8, item);//选课人数
		
	}
	ui->classes->setSortingEnabled(true);

	ui->students->setSortingEnabled(false);
	for (int i = task.students_size() - 1; i >= 0; i--) {
		JC::Student student = task.students(i);

		int currentIndex = ui->students->currentRow() + 1;
		ui->students->insertRow(currentIndex);

		QTableWidgetItem* item = nullptr;

		item = new QTableWidgetItem(QString::fromStdString(student.id()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex, 0, item);//学号

		item = new QTableWidgetItem(QString::fromStdString(student.name()));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex, 1, item);//姓名

		QStringList SCList;
		for (auto& s : student.classes()) {
			SCList.append(QString::fromStdString(s));
		}

		auto getCredit = [&](QString name)->double {
			for (int i = 0; i < ui->classes->rowCount(); i++) {
				if (ui->classes->item(i, 0)->text() == name) {
					return ui->classes->item(i, 6)->text().toDouble();
				}
			}
			return 0;
		};

		int SCCount = 0;
		double CDSum = 0;
		for (auto& s : SCList) {
			if (!s.isEmpty()) {
				SCCount++;
				CDSum += getCredit(s);
			}
		}

		item = new QTableWidgetItem(QString::number(SCCount));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex, 2, item);//选课数量

		item = new QTableWidgetItem(QString::number(CDSum));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex, 3, item);//学分数

		item = new QTableWidgetItem(SCList.join(';'));
		if (item == nullptr) {
			QMessageBox::critical(this, "致命错误", "内存分配失败");
			ui->students->setSortingEnabled(true);
			return;
		}
		ui->students->setItem(currentIndex, 4, item);//所选课程
	}
	ui->students->setSortingEnabled(true);
}

void ManagerWindow::on_timerTimeOut()
{
	this->update();
}

void ManagerWindow::paintEvent(QPaintEvent* event)
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
	QWidget::paintEvent(event);
}