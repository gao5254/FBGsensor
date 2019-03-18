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
			(*ssInfo)[i].chl = static_cast<sensorType>(settings.value("chl").toInt());

		}
	} 
	else
	{
	}

}

sensorInfoWidget::~sensorInfoWidget()
{
}
