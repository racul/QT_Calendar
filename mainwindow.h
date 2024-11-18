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
