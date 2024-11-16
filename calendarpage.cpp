#include "calendarpage.h"
#include "calendarmodel.h"
#include "calendardelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>

CalendarPage::CalendarPage(CalendarManager *manager, QWidget *parent)
    : QWidget(parent), calendarManager(manager), currentMonth(QDate::currentDate()), lastClickedDate(QDate::currentDate())
{
    // 메인 레이아웃 설정
    QVBoxLayout *calendarLayout = new QVBoxLayout(this);
    calendarLayout->setSpacing(10);
    calendarLayout->setContentsMargins(20, 20, 20, 20);

    // 헤더 부분 (월 표시 및 이전/다음 버튼)
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // 이전, 다음 버튼 생성 및 스타일 설정
    prevMonthButton = new QPushButton("<", this);
    nextMonthButton = new QPushButton(">", this);
    QString navigationButtonStyle =
        "QPushButton {"
        "   background-color: #f8f9fa;"
        "   border: 1px solid #dee2e6;"
        "   border-radius: 15px;"
        "   padding: 5px 15px;"
        "   font-size: 16px;"
        "   min-width: 30px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e9ecef;"
        "}";
    prevMonthButton->setStyleSheet(navigationButtonStyle);
    nextMonthButton->setStyleSheet(navigationButtonStyle);

    // 월 표시 레이블
    monthLabel = new QLabel(this);
    monthLabel->setAlignment(Qt::AlignCenter);
    monthLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   color: #495057;"
        "}"
        );
    updateMonthLabel();

    // 헤더 레이아웃 구성
    headerLayout->addWidget(prevMonthButton);
    headerLayout->addWidget(monthLabel, 1);
    headerLayout->addWidget(nextMonthButton);

    // 달력 뷰 설정
    calendarView = new QTableView(this);
    calendarView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    calendarView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    calendarView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    calendarView->setStyleSheet(
        "QTableView {"
        "   background-color: white;"
        "   border: 1px solid #dee2e6;"
        "   border-radius: 8px;"
        "}"
        "QTableView::item {"
        "   padding: 5px;"
        "}"
        );

    // CalendarModel 및 Delegate 설정
    CalendarModel *model = new CalendarModel(currentMonth, calendarManager, this);
    calendarView->setModel(model);
    CalendarDelegate *delegate = new CalendarDelegate(this);
    calendarView->setItemDelegate(delegate);

    // 선택된 날짜 레이블
    selectedDateLabel = new QLabel(this);
    selectedDateLabel->setAlignment(Qt::AlignCenter);
    selectedDateLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 16px;"
        "   color: #495057;"
        "   padding: 10px;"
        "}"
        );
    selectedDateLabel->setText("날짜를 선택하세요.");

    // 일정 추가 버튼
    addEventButton = new QPushButton("+", this);
    addEventButton->setFixedSize(50, 50);
    addEventButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   font-size: 24px;"
        "   border-radius: 25px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        );

    // 버튼 레이아웃
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(addEventButton);

    // 시그널-슬롯 연결
    connect(prevMonthButton, &QPushButton::clicked, this, &CalendarPage::onPrevMonthClicked);
    connect(nextMonthButton, &QPushButton::clicked, this, &CalendarPage::onNextMonthClicked);
    connect(calendarView, &QTableView::clicked, this, &CalendarPage::onCellClicked);
    connect(calendarView, &QTableView::doubleClicked, this, &CalendarPage::onCellDoubleClicked);
    connect(addEventButton, &QPushButton::clicked, this, [this]() {
        emit addEventClicked();
    });

    // 메인 레이아웃에 위젯 추가
    calendarLayout->addLayout(headerLayout);
    calendarLayout->addWidget(calendarView);
    calendarLayout->addWidget(selectedDateLabel);
    calendarLayout->addLayout(buttonLayout);
}

void CalendarPage::updateMonthLabel()
{
    monthLabel->setText(currentMonth.toString("yyyy년 MM월"));
}

void CalendarPage::updateCalendarView()
{
    CalendarModel *newModel = new CalendarModel(currentMonth, calendarManager, this);
    calendarView->setModel(newModel);
    updateMonthLabel();
}

void CalendarPage::onPrevMonthClicked()
{
    currentMonth = currentMonth.addMonths(-1);
    updateCalendarView();
}

void CalendarPage::onNextMonthClicked()
{
    currentMonth = currentMonth.addMonths(1);
    updateCalendarView();
}

void CalendarPage::onCellClicked(const QModelIndex &index)
{
    CalendarModel *model = static_cast<CalendarModel *>(calendarView->model());
    lastClickedDate = model->getDateForIndex(index);
    selectedDateLabel->setText("선택된 날짜: " + lastClickedDate.toString("yyyy-MM-dd"));
    emit dateClicked(lastClickedDate);
}

void CalendarPage::onCellDoubleClicked(const QModelIndex &index)
{
    CalendarModel *model = static_cast<CalendarModel *>(calendarView->model());
    QDate clickedDate = model->getDateForIndex(index);
    emit dateDoubleClicked(clickedDate);
}

QDate CalendarPage::getSelectedDate() const
{
    return lastClickedDate;
}
