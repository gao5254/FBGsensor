#include "peakinfomodel.h"


PeakInfoModel::PeakInfoModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	peakTable.resize(12);
	for (int i = 0; i < 6; ++i)
	{
		unitList << "" << "";
	}
}

PeakInfoModel::~PeakInfoModel()
{
}

//set the calculation tabel to the tabel 
void PeakInfoModel::setnum(const QVector<double> &table)
{
	beginResetModel();
	peakTable = table;
	endResetModel();
}

//set the unit list
void PeakInfoModel::setUnitList(const QStringList &uList)
{
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
	return 6;
}

QVariant PeakInfoModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const 
{
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();
	if (unitList.at(index.row() * 6 + index.column()) == "")
	{
		return QString::fromLocal8Bit("未连接");
	}
	return QString::number(peakTable.at(index.row() * 6 + index.column()), 'f', 3) + unitList.at(index.row() * 6 + index.column());
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
