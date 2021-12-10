#pragma once

#include "AddClass.h"

class EditClass final : public AddClass
{
	Q_OBJECT

public:
	EditClass(QWidget *parent = Q_NULLPTR);
	void init(JC::Class& clas);
};
