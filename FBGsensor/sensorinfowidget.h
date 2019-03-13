#pragma once

#include <QWidget>
#include "ui_sensorinfowidget.h"

class sensorInfoWidget : public QWidget
{
	Q_OBJECT

public:
	sensorInfoWidget(QWidget *parent = Q_NULLPTR);
	~sensorInfoWidget();

private:
	Ui::sensorInfoWidget ui;
};
