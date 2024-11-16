/*
#include "mainwindow.h"
#include "calendarmodel.h"
#include "calendardelegate.h"
#include "dayview.h"
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QDebug>
#include <QDialog>
#include <QHeaderView>
#include <QStackedWidget>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentMonth(QDate::currentDate())
{
    // QStackedWidget 생성
    stackedWidget = new QStackedWidget(this);

    // CalendarManager 초기화 및 이벤트 로드
    calendarManager = new CalendarManager("events.csv");
    calendarManager->loadEvents();

    // 1. 첫 번째 페이지: 달력 화면 **********
    calendarPage = new QWidget(this);

    // 달력 뷰 (QTableView 생성 및 설정)
    calendarView = new QTableView(this);
    calendarView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 창 크기에 맞춰 확장

    // 열과 행의 크기를 창 크기에 맞춰 조정
    calendarView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    calendarView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 스크롤바 제거
    calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 이전, 다음 버튼 생성
    prevMonthButton = new QPushButton("<", this);
    nextMonthButton = new QPushButton(">", this);

    // 현재 월을 표시하는 레이블 생성
    monthLabel = new QLabel(this);
    monthLabel->setAlignment(Qt::AlignCenter);
    updateMonthLabel();

    // CalendarModel을 사용하여 QTableView에 설정
    CalendarModel *model = new CalendarModel(currentMonth, calendarManager, this);
    calendarView->setModel(model);

    // CalendarDelegate 설정
    CalendarDelegate *delegate = new CalendarDelegate(this);
    calendarView->setItemDelegate(delegate);

    // 일정 추가 버튼 생성
    addEventButton = new QPushButton("+", this);
    addEventButton->setFixedSize(50, 50);  // 버튼 크기를 고정
    addEventButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"  // 버튼 색상
        "    color: white;"
        "    font-size: 24px;"
        "    border-radius: 25px;"         // 버튼을 원형으로
        "    width: 50px;"
        "    height: 50px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"   // 호버 시 색상
        "}"
        );

    // 셀 클릭 시 시그널-슬롯 연결
    connect(calendarView, &QTableView::clicked, this, &MainWindow::onCellClicked);

    // 달력 뷰 더블 클릭 시 DayView로 이동
    connect(calendarView, &QTableView::doubleClicked, this, &MainWindow::onCellDoubleClicked);

    // 월 변경 버튼 연결
    connect(prevMonthButton, &QPushButton::clicked, this, &MainWindow::onPrevMonthClicked);
    connect(nextMonthButton, &QPushButton::clicked, this, &MainWindow::onNextMonthClicked);

    // 일정 추가 버튼 클릭 시 슬롯 연결
    connect(addEventButton, &QPushButton::clicked, this, &MainWindow::onAddEventButtonClicked);

    // 레이아웃 설정
    QVBoxLayout *calendarLayout = new QVBoxLayout(calendarPage);
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(prevMonthButton);
    headerLayout->addWidget(monthLabel);
    headerLayout->addWidget(nextMonthButton);

    calendarLayout->addLayout(headerLayout);
    calendarLayout->addWidget(calendarView);

    // 선택된 날짜 레이블 초기화
    selectedDateLabel = new QLabel(this);
    selectedDateLabel->setAlignment(Qt::AlignCenter);
    selectedDateLabel->setStyleSheet("font-size: 16px; color: #333;");
    selectedDateLabel->setText("날짜를 선택하세요.");

    // 달력 화면에 레이블 추가
    calendarLayout->addWidget(selectedDateLabel);

    // 우측 하단 고정 버튼 레이아웃
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    // buttonLayout->addStretch();  // 위쪽 여백 채우기
    buttonLayout->addWidget(addEventButton, 0, Qt::AlignRight | Qt::AlignBottom);

    // 메인 레이아웃에 하단 버튼 레이아웃 추가
    calendarLayout->addLayout(buttonLayout);

    // 메인 페이지에 레이아웃 설정
    calendarPage->setLayout(calendarLayout);

    // 2. 두 번째 페이지: 일정 추가/편집 화면 ***************
    editEventPage = new QWidget(this);
    QVBoxLayout *editLayout = new QVBoxLayout(editEventPage);

    eventTitleLabel = new QLabel("일정 추가/편집", editEventPage);
    eventTitleLabel->setAlignment(Qt::AlignCenter);
    eventNameLineEdit = new QLineEdit(editEventPage);
    dateEdit = new QDateEdit(editEventPage);
    startTimeEdit = new QTimeEdit(editEventPage);
    endTimeEdit = new QTimeEdit(editEventPage);

    // 저장 및 취소 버튼
    saveButton = new QPushButton("저장", editEventPage);
    cancelButton = new QPushButton("취소", editEventPage);
    deleteButton = new QPushButton("삭제", editEventPage); // 수정시 삭제 버튼
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::onCancelButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);


    editLayout->addWidget(eventTitleLabel);
    editLayout->addWidget(new QLabel("일정 이름"));
    editLayout->addWidget(eventNameLineEdit);
    editLayout->addWidget(new QLabel("날짜"));
    editLayout->addWidget(dateEdit);
    editLayout->addWidget(new QLabel("시작 시간"));
    editLayout->addWidget(startTimeEdit);
    editLayout->addWidget(new QLabel("종료 시간"));
    editLayout->addWidget(endTimeEdit);
    editLayout->addWidget(saveButton);
    editLayout->addWidget(cancelButton);
    editLayout->addWidget(deleteButton);
    editEventPage->setLayout(editLayout);

    // 3. 세 번째 페이지: 상세 일정 화면
    dayView = new DayView(this);

    // 뒤로가기 버튼 클릭 시 캘린더 화면으로 돌아가는 슬롯 연결
    connect(dayView, &DayView::backButtonClicked, this, [this]() {
        stackedWidget->setCurrentWidget(calendarPage);
        qDebug() << "Returned to main calendar view.";
    });

    connect(dayView, &DayView::eventDoubleClicked, this, &MainWindow::onEventDoubleClicked);

    // 페이지를 QStackedWidget에 추가 **************
    stackedWidget->addWidget(calendarPage);  // 페이지 0: 달력 화면
    stackedWidget->addWidget(editEventPage); // 페이지 1: 일정 추가/편집 화면
    stackedWidget->addWidget(dayView);       // 페이지 2: 상세 일정 화면

    // 메인 레이아웃 설정
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(stackedWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


    // 창 크기 설정
    this->resize(800, 500); // 창의 기본 크기를 설정
}

void MainWindow::onCellClicked(const QModelIndex &index)
{
    CalendarModel *model = static_cast<CalendarModel *>(calendarView->model());
    lastClickedDate = model->getDateForIndex(index);

    // 선택된 날짜를 레이블에 표시
    selectedDateLabel->setText("선택된 날짜: " + lastClickedDate.toString("yyyy-MM-dd"));
}

void MainWindow::onCellDoubleClicked(const QModelIndex &index)
{
    CalendarModel *model = static_cast<CalendarModel *>(calendarView->model());
    QDate clickedDate = model->getDateForIndex(index);

    // 해당 날짜의 일정 가져오기
    QList<Event> events = calendarManager->getEventsForDate(clickedDate);

    // 일정 상세 페이지로 전환
    dayView->displaySchedule(clickedDate, events);
    stackedWidget->setCurrentWidget(dayView);
}

void MainWindow::updateMonthLabel()
{
    monthLabel->setText(currentMonth.toString("yyyy년 MM월"));
}

void MainWindow::updateCalendarView()
{
    CalendarModel *newModel = new CalendarModel(currentMonth, calendarManager, this);
    calendarView->setModel(newModel);
    updateMonthLabel();
}

void MainWindow::onPrevMonthClicked()
{
    currentMonth = currentMonth.addMonths(-1);
    updateCalendarView();
}

void MainWindow::onNextMonthClicked()
{
    currentMonth = currentMonth.addMonths(1);
    updateCalendarView();
}

void MainWindow::onAddEventButtonClicked()
{
    currentEditingEvent = Event();

    // 일정 수정 화면의 필드들을 날짜 정보로 채우기
    eventTitleLabel->setText("일정 추가");
    eventNameLineEdit->clear();

    // 마지막으로 클릭한 날짜로 설정, 없으면 현재 날짜로 설정
    if (lastClickedDate.isValid()) {
        dateEdit->setDate(lastClickedDate);
    } else {
        dateEdit->setDate(QDate::currentDate());
    }

    // 시간은 현재 시간으로 설정
    startTimeEdit->setTime(QTime::currentTime());
    endTimeEdit->setTime(QTime::currentTime().addSecs(3600));
    deleteButton->setEnabled(false);

    // 일정 추가/편집 화면으로 전환
    stackedWidget->setCurrentWidget(editEventPage);
}

void MainWindow::onSaveButtonClicked()
{
    // 일정 데이터를 입력 필드에서 가져옴
    QString eventName = eventNameLineEdit->text();
    QDate eventDate = dateEdit->date();
    QTime startTime = startTimeEdit->time();
    QTime endTime = endTimeEdit->time();

    // 필수 입력 검증
    if (eventName.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "일정 이름을 입력하세요.");
        return;
    }

    // 이벤트 데이터를 CSV 파일에 저장
    bool success = false;
    if (currentEditingEvent.name.isEmpty()) {
        // 새 일정 추가
        success = calendarManager->saveEvent(eventName, eventDate, startTime, endTime);
    } else {
        // 기존 일정 수정
        success = calendarManager->updateEvent(currentEditingEvent, eventName, eventDate, startTime, endTime);
    }

    if (success) {
        QMessageBox::information(this, "저장 완료", "일정이 저장되었습니다.");
        calendarManager->loadEvents();
        updateCalendarView();  // 캘린더 뷰 업데이트
    } else {
        QMessageBox::critical(this, "저장 실패", "일정을 저장하는 데 실패했습니다.");
    }

    // 현재 수정 중인 이벤트 초기화
    currentEditingEvent = Event();

    // 캘린더 화면으로 돌아가기
    stackedWidget->setCurrentWidget(calendarPage);
}


void MainWindow::onCancelButtonClicked()
{
    // 현재 수정 중인 이벤트 초기화
    currentEditingEvent = Event();

    // 캘린더 화면으로 돌아가기
    stackedWidget->setCurrentWidget(calendarPage);
}

void MainWindow::onDeleteButtonClicked()
{
    if (currentEditingEvent.name.isEmpty()) {
        QMessageBox::warning(this, "삭제 오류", "삭제할 이벤트가 선택되지 않았습니다.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "이벤트 삭제", "정말로 이 이벤트를 삭제하시겠습니까?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (calendarManager->deleteEvent(currentEditingEvent)) {
            QMessageBox::information(this, "삭제 완료", "이벤트가 삭제되었습니다.");
            calendarManager->loadEvents();
            updateCalendarView();
            stackedWidget->setCurrentWidget(calendarPage);
        } else {
            QMessageBox::critical(this, "삭제 실패", "이벤트 삭제에 실패했습니다.");
        }
    }
}

void MainWindow::onEventDoubleClicked(const Event &event) {
    // 현재 수정 중인 이벤트 저장
    currentEditingEvent = event;

    // 일정 수정 화면의 필드들을 현재 이벤트 정보로 채우기
    eventTitleLabel->setText("일정 수정");
    eventNameLineEdit->setText(event.name);
    dateEdit->setDate(event.date);
    startTimeEdit->setTime(event.startTime);
    endTimeEdit->setTime(event.endTime);
    deleteButton->setEnabled(true);

    // 일정 수정 화면으로 전환
    stackedWidget->setCurrentWidget(editEventPage);
}
*/

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
