#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "calendarmanager.h"
#include "dayview.h"
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
    void onCellDoubleClicked(const QModelIndex &index);
    void onPrevMonthClicked();
    void onNextMonthClicked();
    void onAddEventButtonClicked();
    void onSaveButtonClicked();
    void onCancelButtonClicked();

private:
    void updateMonthLabel();
    void updateCalendarView();

    QStackedWidget *stackedWidget;
    QLabel *selectedDateLabel;  // 클릭된 날짜를 표시할 레이블

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

    // 위젯
    QWidget *calendarPage;
    QWidget *editEventPage;
    DayView *dayView;


};

#endif // MAINWINDOW_H
