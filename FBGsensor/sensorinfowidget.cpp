#include <QSettings>
#include <QDebug>
#include "sensorinfowidget.h"

sensorInfoWidget::sensorInfoWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// read the config file
	QSettings settings("FBGconfig.ini", QSettings::IniFormat);
	// test whether there has sensor
	if (settings.contains("sensor"))
	{
		// load data to vector
		int ssnum = settings.beginReadArray("sensor");
		ssInfo = new QVector<sensorInfo>(ssnum);
		for (int i = 0; i < ssnum; i++)
		{
			settings.setArrayIndex(i);
			(*ssInfo)[i].type = (sensorType)(settings.value("type").toInt());
			(*ssInfo)[i].chl = settings.value("chl").toInt();
			(*ssInfo)[i].wavRangeStart = settings.value("wavrangestart").toInt();
			(*ssInfo)[i].wavRangeEnd = settings.value("wavrangeend").toInt();
			(*ssInfo)[i].k = settings.value("k").toDouble();
			(*ssInfo)[i].b = settings.value("b").toDouble();
		}
		settings.endArray();
	} 
	else
	{
		// a null vector
		ssInfo = new QVector<sensorInfo>();
	}
	//delete later
	ssInfo->resize(1);
	(*ssInfo)[0].type = sensorType::Temperature;
	(*ssInfo)[0].chl = 0;
	(*ssInfo)[0].wavRangeStart = 1545000;
	(*ssInfo)[0].wavRangeEnd = 1555000;
	(*ssInfo)[0].k = 50;
	(*ssInfo)[0].b = 100;
}

sensorInfoWidget::~sensorInfoWidget()
{
	//write the ini file
	QSettings settings("FBGconfig.ini", QSettings::IniFormat);
	settings.remove("sensor");
	if (ssInfo->size() > 0)
	{
		settings.beginWriteArray("sensor");
		for (int i = 0; i < ssInfo->size(); ++i)
		{
			settings.setArrayIndex(i);
			settings.setValue("type", (int)(ssInfo->at(i).type));
			settings.setValue("chl", ssInfo->at(i).chl);
			settings.setValue("wavrangestart", ssInfo->at(i).wavRangeStart);
			settings.setValue("wavrangeend", ssInfo->at(i).wavRangeEnd);
			settings.setValue("k", ssInfo->at(i).k);
			settings.setValue("b", ssInfo->at(i).b);

		}
		settings.endArray();

	} 
}
