#include "peakinfomodel.h"


PeakInfoModel::PeakInfoModel(int size, QObject *parent)
	: QAbstractTableModel(parent)
{
	peakTable.resize(size * 4);
	for (int i = 0; i < size; ++i)
	{
		unitList << "" << "" << "" << "";
		Horiheader << QString::fromLocal8Bit("波长值") + QString::number(i + 1) << QString::fromLocal8Bit("测量值") + QString::number(i + 1);
	}
}

PeakInfoModel::~PeakInfoModel()
{
}

//set the calculation tabel to the tabel 
void PeakInfoModel::setnum(const QVector<double> &table)
{
	Q_ASSERT(peakTable.size() == table.size());
	beginResetModel();
// 	if (peakTable.size() != table.size())
// 	{
// 		unitList = QStringList();
// 		Horiheader = QStringList();
// 		for (int i = 0; i < table.size(); ++i)
// 		{
// 			Horiheader << QString::fromLocal8Bit("波长值") + QString::number(i + 1) << QString::fromLocal8Bit("测量值") + QString::number(i + 1);
// 		}
// 	}
	peakTable = table;
	endResetModel();
}

//set the unit list
void PeakInfoModel::setUnitList(const QStringList &uList)
{
	Q_ASSERT(uList.size() == unitList.size());
	beginResetModel();
	unitList = uList;
	endResetModel();
}

int PeakInfoModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const 
{
	return 2;
}

int PeakInfoModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const 
{
	return peakTable.size() / 2;
}

QVariant PeakInfoModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const 
{
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();
	if (unitList.at(index.row() * peakTable.size() / 2 + index.column()) == "")
	{
		return QString::fromLocal8Bit("未连接");
	}
	return QString::number(peakTable.at(index.row() * peakTable.size() / 2 + index.column()), 'f', 3) + unitList.at(index.row() * peakTable.size() / 2 + index.column());
}

QVariant PeakInfoModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const 
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			return Horiheader.at(section);
		} 
		else if (orientation == Qt:: Vertical)
		{
			return QString::fromLocal8Bit("通道") + QString::number(section + 1);
		}
	}
	return QVariant();
}
