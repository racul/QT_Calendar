#ifndef CALENDARMODEL_H
#define CALENDARMODEL_H

#include <QAbstractTableModel>
#include <QDate>

class CalendarModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CalendarModel(const QDate &month, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QDate getDateForIndex(const QModelIndex &index) const;

private:
    QDate firstDayOfMonth;
    int daysInMonth;
};

#endif // CALENDARMODEL_H
