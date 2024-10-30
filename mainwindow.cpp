#include "mainwindow.h"
#include "calendarmodel.h"
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



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentMonth(QDate::currentDate())
{
    // QTableView 생성
    calendarView = new QTableView(this);

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

    // 셀 클릭 시 시그널-슬롯 연결
    connect(calendarView, &QTableView::clicked, this, &MainWindow::onCellClicked);

    // 월 변경 버튼 연결
    connect(prevMonthButton, &QPushButton::clicked, this, &MainWindow::onPrevMonthClicked);
    connect(nextMonthButton, &QPushButton::clicked, this, &MainWindow::onNextMonthClicked);

    // 레이아웃 설정
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(prevMonthButton);
    headerLayout->addWidget(monthLabel);
    headerLayout->addWidget(nextMonthButton);

    layout->addLayout(headerLayout);
    layout->addWidget(calendarView);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
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


