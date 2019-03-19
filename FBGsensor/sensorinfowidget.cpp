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
			(*ssInfo)[i].ssName = settings.value("ssname").toString();
			(*ssInfo)[i].chl = settings.value("chl").toInt();
			(*ssInfo)[i].type = (sensorType)(settings.value("type").toInt());
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

		}
	} 
}
