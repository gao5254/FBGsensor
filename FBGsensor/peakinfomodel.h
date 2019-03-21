#pragma once

#include <QAbstractTableModel>


//the model to show the peak wavelength infomation in the main window

class PeakInfoModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	PeakInfoModel(QObject *parent = nullptr);
	~PeakInfoModel();
	void setnum(const QVector<double> &table);
	void setUnitList(const QStringList &uList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
	QVector<double> peakTable;
	QStringList Horiheader = QStringList() << QString::fromLocal8Bit("��ֵ1") << QString::fromLocal8Bit("����ֵ1")
		<< QString::fromLocal8Bit("��ֵ2") << QString::fromLocal8Bit("����ֵ2")
		<< QString::fromLocal8Bit("��ֵ3") << QString::fromLocal8Bit("����ֵ3");
	QStringList unitList;
};
