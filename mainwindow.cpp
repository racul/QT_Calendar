#include "mainwindow.h"
#include "calendarmodel.h"
#include "calendardelegate.h"
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
    QWidget *calendarPage = new QWidget(this);

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

    // 우측 하단 고정 버튼 레이아웃
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    // buttonLayout->addStretch();  // 위쪽 여백 채우기
    buttonLayout->addWidget(addEventButton, 0, Qt::AlignRight | Qt::AlignBottom);

    // 메인 레이아웃에 하단 버튼 레이아웃 추가
    calendarLayout->addLayout(buttonLayout);

    // 메인 페이지에 레이아웃 설정
    calendarPage->setLayout(calendarLayout);

    // 2. 두 번째 페이지: 일정 추가/편집 화면 ***************
    QWidget *editEventPage = new QWidget(this);
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
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::onCancelButtonClicked);

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
    editEventPage->setLayout(editLayout);


    // 페이지를 QStackedWidget에 추가 **************
    stackedWidget->addWidget(calendarPage);  // 페이지 0: 달력 화면
    stackedWidget->addWidget(editEventPage); // 페이지 1: 일정 추가/편집 화면

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
    QDate clickedDate = model->getDateForIndex(index);
    qDebug() << "클릭된 날짜:" << clickedDate;
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
    // 일정 추가/편집 화면으로 전환
    stackedWidget->setCurrentIndex(1);
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
    if (calendarManager->saveEvent(eventName, eventDate, startTime, endTime)) {
        QMessageBox::information(this, "저장 완료", "일정이 저장되었습니다.");
        calendarManager->loadEvents();
    } else {
        QMessageBox::critical(this, "저장 실패", "일정을 저장하는 데 실패했습니다.");
    }

    // 캘린더 화면으로 돌아가기
    stackedWidget->setCurrentIndex(0);
}


void MainWindow::onCancelButtonClicked()
{
    // 캘린더 화면으로 돌아가기
    stackedWidget->setCurrentIndex(0);
}
