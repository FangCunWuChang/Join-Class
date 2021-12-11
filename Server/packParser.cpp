#include "packParser.h"

PackParser::PackParser(QObject* parent) : QObject(parent)
{

}

void PackParser::dataIn(QString peeraddress, const QByteArray& data)
{
	JC::Net::Request request;
	if (LPPack::unpackRequestWithPrivate(const_cast<QByteArray&>(data), request)) {
		this->parse(peeraddress, request);
	}
}

void PackParser::send(QString peeraddress, const JC::Net::Answer& answer)
{
	QByteArray data;
	if (LPPack::packAnswerWithPrivate(const_cast<JC::Net::Answer&>(answer), data)) {
		emit this->dataOut(peeraddress, data);
	}
}

void PackParser::parse(QString peeraddress, const JC::Net::Request& request)
{
	Logger::log(QString::asprintf("Parse request from %s", qPrintable(peeraddress)));
	JC::Net::Answer answer;
	bool shutdownFlag = false;
	bool restartFlag = false;

	switch (request.type())
	{
	case JC::Net::Request_Type::Request_Type_TEST:
	{
		Logger::log("Package Test");
		
		answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
		answer.set_mesage(QString::asprintf("Join Classes Server Service %.1f", ::_JC_Version).toStdString());

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_LOGIN:
	{
		Logger::log("Package Login");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
				answer.set_mesage(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
				answer.set_usertype(Users::getType(QString::fromStdString(request.user())));
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_CHANGEUSER:
	{
		Logger::log("Package Change User");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					if (Users::find(QString::fromStdString(request.newuser().id()))) {
						if (Users::setUser(request.newuser())) {
							Logger::log(QString::asprintf("Writen user data:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
							answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
							answer.set_mesage(QString::asprintf("Writen user data").toStdString());
						}
						else {
							Logger::log(QString::asprintf("Can't write user data:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
							answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
							answer.set_mesage(QString::asprintf("Can't write user data").toStdString());
						}
					}
					else {
						Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
						answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
						answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.newuser().id()))).toStdString());
					}
				}
				else {
					if (request.user() == request.newuser().id()) {
						if (Users::setPass(request.newuser())) {
							Logger::log(QString::asprintf("Writen password:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
							answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
							answer.set_mesage(QString::asprintf("Writen password").toStdString());
						}
						else {
							Logger::log(QString::asprintf("Can't write password:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
							answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
							answer.set_mesage(QString::asprintf("Can't write password").toStdString());
						}
					}
					else {
						Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
						answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
						answer.set_mesage(QString::asprintf("Permission denied").toStdString());
					}
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_GETUSERLIST:
	{
		Logger::log("Package Get Users");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					QVector<JC::User> users = Users::getUsers();
					Logger::log("Got user list");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Got user list");
					for (int i = 0; i < users.size(); i++) {
						answer.add_userlist()->CopyFrom(users.at(i));
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_ADDUSER:
	{
		Logger::log("Package Add User");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					if (!Users::find(QString::fromStdString(request.newuser().id()))) {
						if (Users::setUser(request.newuser())) {
							Logger::log(QString::asprintf("Writen user data:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
							answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
							answer.set_mesage(QString::asprintf("Writen user data").toStdString());
						}
						else {
							Logger::log(QString::asprintf("Can't write user data:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
							answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
							answer.set_mesage(QString::asprintf("Can't write user data").toStdString());
						}
					}
					else {
						Logger::log(QString::asprintf("User is already exists:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
						answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
						answer.set_mesage(QString::asprintf("User is already exists:%s", qPrintable(QString::fromStdString(request.newuser().id()))).toStdString());
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_DELUSER:
	{
		Logger::log("Package Remove User");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					if (Users::find(QString::fromStdString(request.newuser().id()))) {
						if (request.user() != request.newuser().id()) {
							if (Users::removeUser(QString::fromStdString(request.newuser().id()))) {
								Logger::log(QString::asprintf("Removed user data:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
								answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
								answer.set_mesage(QString::asprintf("Removed user data").toStdString());
							}
							else {
								Logger::log(QString::asprintf("Can't remove user data:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
								answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
								answer.set_mesage(QString::asprintf("Can't remove user data").toStdString());
							}
						}
						else {
							Logger::log(QString::asprintf("You can't remove yourself:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
							answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
							answer.set_mesage(QString::asprintf("You can't remove yourself:%s", qPrintable(QString::fromStdString(request.newuser().id()))).toStdString());
						}
					}
					else {
						Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.newuser().id()))));
						answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
						answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.newuser().id()))).toStdString());
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_GETCONFIG:
	{
		Logger::log("Package Get Config");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					QString config = QString::fromStdString(GetConfig::config().ToString());

					Logger::log("Get Config");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Get Config");
					answer.set_config(config.toStdString());
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_SETCONFIG:
	{
		Logger::log("Package Set Config");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					QString config = QString::fromStdString(request.config());
					neb::CJsonObject jsonObject;
					jsonObject.Parse(config.toStdString());
					if (GetConfig::setConfig(jsonObject)) {
						Logger::log("Writen config data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
						answer.set_mesage("Writen config data");
					}
					else {
						Logger::log("Can't write config data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
						answer.set_mesage("Can't write config data");
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_GETTASK:
	{
		Logger::log("Package Get Task");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					JC::Task task = Tasks::getTask();
					Logger::log("Got task");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Got task");
					answer.mutable_task()->CopyFrom(task);
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_SETTASK:
	{
		Logger::log("Package Set Task");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					if (Tasks::setTask(request.task())) {
						Logger::log("Writen task data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
						answer.set_mesage("Writen task data");
					}
					else {
						Logger::log("Can't write task data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
						answer.set_mesage("Can't write task data");
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_GETSTUDENT:
	{
		Logger::log("Package Get Student Data");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_USER) {
					JC::Student student = Tasks::getStudent(QString::fromStdString(request.user()));
					Logger::log("Got student");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Got student");
					answer.mutable_student()->CopyFrom(student);
					answer.mutable_classes()->CopyFrom(Tasks::getClasses());
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_JOINCLASS:
	{
		Logger::log("Package Join Class");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_USER) {
					QStringList classes;
					for (int i = 0; i < request.classes_size(); i++) {
						classes.append(QString::fromStdString(request.classes(i)));
					}
					if (Tasks::setClasses(QString::fromStdString(request.user()), classes)) {
						Logger::log("Writen class data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
						answer.set_mesage("Writen class data");
					}
					else {
						Logger::log("Can't write class data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
						answer.set_mesage("Can't write class data");
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_GETLOGLIST:
	{
		Logger::log("Package Get Log List");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					QStringList logList = Logger::getList();
					Logger::log("Got Log List");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Got Log List");
					for (auto& str : logList) {
						*(answer.add_loglist()) = str.toStdString();
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_GETLOGDATA:
	{
		Logger::log("Package Get Log Data");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					QString data = Logger::getData(QString::fromStdString(request.logname()));
					Logger::log("Got Log Data");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Got Log Data");
					answer.set_logdata(data.toStdString());
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_CLEARLOGS:
	{
		Logger::log("Package Clear Log Data");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					if (Logger::clearLogs()) {
						Logger::log("Cleared log data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
						answer.set_mesage("Cleared log data");
					}
					else {
						Logger::log("Can't clear log data");
						answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
						answer.set_mesage("Can't clear log data");
					}
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		this->send(peeraddress, answer);
		break;
	}
	case JC::Net::Request_Type::Request_Type_SHUTDOWN:
	{
		Logger::log("Package Shutdown");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					Logger::log("Shutdown");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Shutdown");
					shutdownFlag = true;
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		//this->send(peeraddress, answer);
		QByteArray data;
		if (LPPack::packAnswerWithPrivate(const_cast<JC::Net::Answer&>(answer), data)) {
			LinkPortServer::getClass().socketOut(peeraddress, data);
			LinkPortServer::getClass().wait();
		}
		break;
	}
	case JC::Net::Request_Type::Request_Type_RESTART:
	{
		Logger::log("Package Restart");

		if (Users::find(QString::fromStdString(request.user()))) {
			if (Users::login(QString::fromStdString(request.user()), QString::fromStdString(request.password()).toUtf8())) {
				Logger::log(QString::asprintf("Login as user:%s", qPrintable(QString::fromStdString(request.user()))));
				if (Users::getType(QString::fromStdString(request.user())) == JC::User_Type::User_Type_MANAGER) {
					Logger::log("Restart");
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DONE);
					answer.set_mesage("Restart");
					restartFlag = true;
				}
				else {
					Logger::log(QString::asprintf("Permission denied:%s", qPrintable(QString::fromStdString(request.password()))));
					answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
					answer.set_mesage(QString::asprintf("Permission denied").toStdString());
				}
			}
			else {
				Logger::log(QString::asprintf("Password error:%s", qPrintable(QString::fromStdString(request.password()))));
				answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
				answer.set_mesage(QString::asprintf("Password error").toStdString());
			}
		}
		else {
			Logger::log(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))));
			answer.set_type(JC::Net::Answer_Type::Answer_Type_DENIED);
			answer.set_mesage(QString::asprintf("User does not exist:%s", qPrintable(QString::fromStdString(request.user()))).toStdString());
		}

		//this->send(peeraddress, answer);
		QByteArray data;
		if (LPPack::packAnswerWithPrivate(const_cast<JC::Net::Answer&>(answer), data)) {
			LinkPortServer::getClass().socketOut(peeraddress, data);
			LinkPortServer::getClass().wait();
		}
		break;
	}
	default:
	{
		Logger::log("Package Undefined");
		answer.set_type(JC::Net::Answer_Type::Answer_Type_ERROR);
		answer.set_mesage("Undefined package type");
		this->send(peeraddress, answer);
		break;
	}
	}
	if (shutdownFlag) {
		this->shutdown();
	}
	else if (restartFlag) {
		this->restart();
	}
}

void PackParser::shutdown()
{
	LinkPortServer::getClass().stoplisten();
	QCoreApplication::exit(0);
}

void PackParser::restart()
{
	LinkPortServer::getClass().stoplisten();

	QString appPath = QCoreApplication::applicationFilePath();
	QString appDir = QCoreApplication::applicationDirPath();

	SingleManager::getClass().lock();
	SingleManager::getClass().exit();
	SingleManager::getClass().unlock();

	QStringList argList = { appPath };
	QProcess process(this);
	process.setProgram(appPath);
	process.setWorkingDirectory(appDir);
	process.setArguments(argList);
	process.startDetached();

	QCoreApplication::exit(0);
}
