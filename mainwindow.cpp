#include "mainwindow.h"
#include "calendarpage.h"
#include "eventeditpage.h"
#include "dayview.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 기본 위젯 설정
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    // 스택 위젯 생성
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    // CalendarManager 초기화
    calendarManager = new CalendarManager("events.csv");
    calendarManager->loadEvents();

    // 각 페이지 초기화
    initializePages();

    // 시그널-슬롯 연결
    connectSignals();

    // 창 크기 설정
    resize(800, 500);
}

void MainWindow::initializePages()
{
    // 캘린더 페이지 생성
    calendarPage = new CalendarPage(calendarManager, this);

    // 이벤트 편집 페이지 생성
    editEventPage = new EventEditPage(calendarManager, this);

    // 일별 보기 페이지 생성
    dayView = new DayView(this);

    // 스택 위젯에 페이지 추가
    stackedWidget->addWidget(calendarPage);    // 인덱스 0
    stackedWidget->addWidget(editEventPage);   // 인덱스 1
    stackedWidget->addWidget(dayView);         // 인덱스 2
}

void MainWindow::connectSignals()
{
    // CalendarPage 시그널 연결
    connect(calendarPage, &CalendarPage::addEventClicked,
            this, &MainWindow::onAddEventRequested);
    connect(calendarPage, &CalendarPage::dateDoubleClicked,
            this, &MainWindow::onDateDoubleClicked);

    // EventEditPage 시그널 연결
    connect(editEventPage, &EventEditPage::saveClicked,
            this, &MainWindow::onEventSaved);
    connect(editEventPage, &EventEditPage::cancelClicked,
            this, &MainWindow::onEventCancelled);
    connect(editEventPage, &EventEditPage::deleteClicked,
            this, &MainWindow::onEventDeleted);

    // DayView 시그널 연결
    connect(dayView, &DayView::backButtonClicked,
            this, [this]() { stackedWidget->setCurrentWidget(calendarPage); });
    connect(dayView, &DayView::eventDoubleClicked,
            this, &MainWindow::onEventDoubleClicked);
}

void MainWindow::onAddEventRequested()
{
    editEventPage->clearFields();
    editEventPage->setDate(calendarPage->getSelectedDate());
    stackedWidget->setCurrentWidget(editEventPage);
}

void MainWindow::onDateDoubleClicked(const QDate &date)
{
    QList<Event> events = calendarManager->getEventsForDate(date);
    dayView->displaySchedule(date, events);
    stackedWidget->setCurrentWidget(dayView);
}

void MainWindow::onEventDoubleClicked(const Event &event)
{
    editEventPage->setEvent(event);
    stackedWidget->setCurrentWidget(editEventPage);
}

void MainWindow::onEventSaved()
{
    calendarManager->loadEvents();
    calendarPage->updateCalendarView();
    stackedWidget->setCurrentWidget(calendarPage);
}

void MainWindow::onEventCancelled()
{
    stackedWidget->setCurrentWidget(calendarPage);
}

void MainWindow::onEventDeleted()
{
    calendarManager->loadEvents();
    calendarPage->updateCalendarView();
    stackedWidget->setCurrentWidget(calendarPage);
}
