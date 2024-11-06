#include "mainwindow.h"
#include "calendarmodel.h"
#include "createeventdialog.h"
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



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentMonth(QDate::currentDate())
{
    // QTableView 생성
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
    CalendarModel *model = new CalendarModel(currentMonth, this);
    calendarView->setModel(model);

    // 일정 추가 버튼 생성
    QPushButton *addEventButton = new QPushButton("+", this);
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
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(prevMonthButton);
    headerLayout->addWidget(monthLabel);
    headerLayout->addWidget(nextMonthButton);

    layout->addLayout(headerLayout);
    layout->addWidget(calendarView);

    // 우측 하단 고정 버튼 레이아웃
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addStretch();  // 위쪽 여백 채우기
    buttonLayout->addWidget(addEventButton, 0, Qt::AlignRight | Qt::AlignBottom);

    // 메인 레이아웃에 하단 버튼 레이아웃 추가
    layout->addLayout(buttonLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // 창 크기 설정
    this->resize(800, 600); // 창의 기본 크기를 설정
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
    CalendarModel *newModel = new CalendarModel(currentMonth, this);
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
    // 일정 추가 화면 (예: 다이얼로그) 표시
    CreateEventDialog *createEventDialog = new CreateEventDialog(this);
    //createEventDialog->exec();
}

