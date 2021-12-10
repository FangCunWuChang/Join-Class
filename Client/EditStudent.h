#pragma once

#include "AddStudent.h"

class EditStudent final : public AddStudent
{
	Q_OBJECT

public:
	EditStudent(QWidget *parent = Q_NULLPTR);
	void init(JC::Student& student);
};
