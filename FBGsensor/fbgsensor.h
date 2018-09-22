#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_fbgsensor.h"

class FBGsensor : public QMainWindow
{
	Q_OBJECT

public:
	FBGsensor(QWidget *parent = Q_NULLPTR);

private:
	Ui::FBGsensorClass ui;
};
