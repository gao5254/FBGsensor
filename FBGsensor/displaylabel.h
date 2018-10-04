#pragma once

#include <QLabel>
#include "ui_displaylabel.h"

class displayLabel : public QLabel
{
	Q_OBJECT

public:
	displayLabel(QWidget *parent = Q_NULLPTR);
	~displayLabel();

private:
	Ui::displayLabel ui;
};
