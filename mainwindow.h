#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "calendarmanager.h"
#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QStackedWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onCellClicked(const QModelIndex &index);
    void onPrevMonthClicked();
    void onNextMonthClicked();
    void onAddEventButtonClicked();
    void onSaveButtonClicked();
    void onCancelButtonClicked();

private:
    void updateMonthLabel();
    void updateCalendarView();

    QStackedWidget *stackedWidget;

    // 달력 화면
    QTableView *calendarView;
    QPushButton *addEventButton;
    QPushButton *prevMonthButton;
    QPushButton *nextMonthButton;
    QLabel *monthLabel;
    QDate currentMonth;

    // 일정 추가/편집 화면
    QLabel *eventTitleLabel;
    QLineEdit *eventNameLineEdit;
    QDateEdit *dateEdit;
    QTimeEdit *startTimeEdit;
    QTimeEdit *endTimeEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    CalendarManager *calendarManager;

};

#endif // MAINWINDOW_H
