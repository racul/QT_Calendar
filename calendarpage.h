#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include "calendarmanager.h"
#include "calendarmodel.h"

class CalendarPage : public QWidget {
    Q_OBJECT

public:
    explicit CalendarPage(CalendarManager *manager, QWidget *parent = nullptr);
    void updateCalendarView();
    void updateMonthLabel();
    QDate getSelectedDate() const;

signals:
    void addEventClicked();
    void dateDoubleClicked(const QDate &date);
    void dateClicked(const QDate &date);

private slots:
    void onCellClicked(const QModelIndex &index);
    void onCellDoubleClicked(const QModelIndex &index);
    void onPrevMonthClicked();
    void onNextMonthClicked();

private:
    QTableView *calendarView;
    QPushButton *prevMonthButton;
    QPushButton *nextMonthButton;
    QPushButton *addEventButton;
    QLabel *monthLabel;
    QLabel *selectedDateLabel;
    CalendarManager *calendarManager;
    QDate currentMonth;
    QDate lastClickedDate;

};

#endif
