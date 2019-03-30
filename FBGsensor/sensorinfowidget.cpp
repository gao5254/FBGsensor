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
	if (settings.contains("sensor/1/type"))
	{
		// load data to vector
		int ssnum = settings.beginReadArray("sensor");
		ssInfo = new QVector<sensorInfo>(ssnum);
		for (int i = 0; i < ssnum; i++)
		{
			settings.setArrayIndex(i);
			(*ssInfo)[i].type = (sensorType)(settings.value("type").toInt());
// 			(*ssInfo)[i].chl = settings.value("chl").toInt();
			(*ssInfo)[i].wavRangeStart = settings.value("wavrangestart").toInt();
			(*ssInfo)[i].wavRangeEnd = settings.value("wavrangeend").toInt();
			(*ssInfo)[i].mu[0] = settings.value("mu0").toDouble();
			(*ssInfo)[i].mu[1] = settings.value("mu1").toDouble();
			(*ssInfo)[i].a[0] = settings.value("a0").toDouble();
			(*ssInfo)[i].a[1] = settings.value("a1").toDouble();
			(*ssInfo)[i].a[2] = settings.value("a2").toDouble();
			(*ssInfo)[i].a[3] = settings.value("a3").toDouble();
			(*ssInfo)[i].chl = -1;
			(*ssInfo)[i].isconnected = false;
		}
		settings.endArray();
	} 
	else
	{
		// a null vector
		ssInfo = new QVector<sensorInfo>();
	}
	//delete later
// 	ssInfo->resize(1);
// 	(*ssInfo)[0].type = sensorType::Temperature;
// 	(*ssInfo)[0].chl = 0;
// 	(*ssInfo)[0].wavRangeStart = 1545000;
// 	(*ssInfo)[0].wavRangeEnd = 1555000;
// 	(*ssInfo)[0].k = 0.0680;
// 	(*ssInfo)[0].b = -105437.635;


	//init model
	ssmodel = new sensorInfoModel((*ssInfo), this); 
	ui.sensorInfoView->setModel(ssmodel);

	//init headerview
	headerView = ui.sensorInfoView->horizontalHeader();
	headerView->setSectionResizeMode(QHeaderView::Interactive);
// 	qDebug() << headerView->count();
	headerView->resizeSection(1, 120);
	headerView->resizeSection(2, 150);
	headerView->resizeSection(3, 180);

	connect(this, &sensorInfoWidget::sensorInfoChanged, ssmodel, &sensorInfoModel::setInfo);
}

sensorInfoWidget::~sensorInfoWidget()
{
	//write the ini file
// 	QSettings settings("FBGconfig.ini", QSettings::IniFormat);
// 	settings.remove("sensor");
// 	if (ssInfo->size() > 0)
// 	{
// 		settings.beginWriteArray("sensor");
// 		for (int i = 0; i < ssInfo->size(); ++i)
// 		{
// 			settings.setArrayIndex(i);
// 			settings.setValue("type", (int)(ssInfo->at(i).type));
// // 			settings.setValue("chl", ssInfo->at(i).chl);
// 			settings.setValue("wavrangestart", ssInfo->at(i).wavRangeStart);
// 			settings.setValue("wavrangeend", ssInfo->at(i).wavRangeEnd);
// 			settings.setValue("k", ssInfo->at(i).k);
// 			settings.setValue("b", ssInfo->at(i).b);
// 
// 		}
// 		settings.endArray();
// 
// 	} 
}

//get the isDetected
bool sensorInfoWidget::getDetectStatus() const
{
	return isdetected;
}

//set the channel number corresponding to each sensor after detecting sensors
void sensorInfoWidget::setChannelNum(const QVector<int> &chnl)
{
	Q_ASSERT(chnl.size() == ssInfo->size());
	for (int i = 0; i < ssInfo->size(); ++i)
	{
		if (chnl.at(i) == -1)
		{
			(*ssInfo)[i].isconnected = false;
			(*ssInfo)[i].chl = -1;
		} 
		else
		{
			(*ssInfo)[i].isconnected = true;
			(*ssInfo)[i].chl = chnl.at(i);
		}
	}
	isdetected = true;
	emit sensorInfoChanged(ssInfo);
}

void sensorInfoWidget::on_sensorDetectBtn_clicked()
{
	isdetected = false;
	for (int i = 0; i < ssInfo->size(); ++i)
	{
		(*ssInfo)[i].isconnected = false;
		(*ssInfo)[i].chl = -1; 
	}
	emit sensorInfoChanged(ssInfo);
}

//sensorinfo model,to show sensor info
sensorInfoModel::sensorInfoModel(const QVector<sensorInfo> &ssInfo, QObject *parent /*= nullptr*/)
{
	infoTable = ssInfo;
}

sensorInfoModel::~sensorInfoModel()
{

}

//set the info vector
void sensorInfoModel::setInfo(const QVector<sensorInfo> *ssInfo)
{
	beginResetModel();
	infoTable = *ssInfo;
	endResetModel();
}

int sensorInfoModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const 
{
	return infoTable.size();
}

int sensorInfoModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const 
{
	return Horiheader.size();
}

QVariant sensorInfoModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const 
{
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();
	switch (index.column())
	{
	case 0:
		return typeList.at(index.row());
		break;
	case 1:
		return QString("%1-%2").arg(infoTable.at(index.row()).wavRangeStart).arg(infoTable.at(index.row()).wavRangeEnd);
		break;
// 	case 2:
// 		return QString::number(infoTable.at(index.row()).wavRangeEnd);
// 		break;
	case 2:
		return QString("%1, %2").arg(infoTable.at(index.row()).mu[0], 0, 'f', 3).arg(infoTable.at(index.row()).mu[1], 0, 'f', 3);
		break;
	case 3:
		return QString("%1, %2, %3, %4").arg(infoTable.at(index.row()).a[0], 0, 'f', 3).arg(infoTable.at(index.row()).a[1], 0, 'f', 3)
			.arg(infoTable.at(index.row()).a[2], 0, 'f', 3).arg(infoTable.at(index.row()).a[3], 0, 'f', 3);
		break;
	case 4:
		if (infoTable.at(index.row()).chl == -1)
		{
			return QString::fromLocal8Bit("未连接");
		} 
		else
		{
			return QString::fromLocal8Bit("通道") + QString::number(infoTable.at(index.row()).chl + 1);
		}
		break;
	case 5:
		if (infoTable.at(index.row()).isconnected)
		{
			return QString::fromLocal8Bit("是");
		}
		else
		{
			return QString::fromLocal8Bit("否");
		}
		break;
	default:
		return QVariant();
		break;
	}
}

QVariant sensorInfoModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			return Horiheader.at(section);
		}
		else if (orientation == Qt::Vertical)
		{
			return QString::number(section + 1);
		}
	}
	return QVariant();

}

