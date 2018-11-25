#include "peakinfomodel.h"


PeakInfoModel::PeakInfoModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	peakTable[0].resize(2);
	peakTable[1].resize(2);
}

PeakInfoModel::~PeakInfoModel()
{
}

//set the number to the tabel 
void PeakInfoModel::setnum(int col, double num1, double num2)
{
	beginResetModel();
	peakTable[0][col] = num1;
	peakTable[1][col] = num2;
	endResetModel();
}

int PeakInfoModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const 
{
	return 2;
}

int PeakInfoModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const 
{
	return peakTable[0].size();
}

QVariant PeakInfoModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const 
{
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();
	return QString::number(peakTable[index.row()].at(index.column()), 'f', 3);
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
			return QString::fromLocal8Bit("Í¨µÀ") + QString::number(section + 1);
		}
	}
	return QVariant();
}
