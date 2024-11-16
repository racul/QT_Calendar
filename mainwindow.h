/*
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
    void onDeleteButtonClicked();
    void onEventDoubleClicked(const Event &event);


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
    QDate lastClickedDate;

    // 일정 추가/편집 화면
    QLabel *eventTitleLabel;
    QLineEdit *eventNameLineEdit;
    QDateEdit *dateEdit;
    QTimeEdit *startTimeEdit;
    QTimeEdit *endTimeEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *deleteButton;
    CalendarManager *calendarManager;

    Event currentEditingEvent;  // 현재 수정 중인 이벤트를 저장

    // 위젯
    QWidget *calendarPage;
    QWidget *editEventPage;
    DayView *dayView;


};

#endif // MAINWINDOW_H
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include "calendarmanager.h"

class QStackedWidget;
class CalendarPage;
class EventEditPage;
class DayView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // 일정 추가/수정 관련 슬롯
    void onAddEventRequested();
    void onEventSaved();
    void onEventCancelled();
    void onEventDeleted();

    // 날짜 선택 관련 슬롯
    void onDateDoubleClicked(const QDate &date);

    // 이벤트 더블클릭 슬롯
    void onEventDoubleClicked(const Event &event);

private:
    // 초기화 함수들
    void initializePages();
    void connectSignals();

    // 위젯들
    QStackedWidget *stackedWidget;
    CalendarPage *calendarPage;
    EventEditPage *editEventPage;
    DayView *dayView;

    // 데이터 관리
    CalendarManager *calendarManager;
};

#endif // MAINWINDOW_H
