#include "dayview.h"
#include "calendarmanager.h"
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>

DayView::DayView(QWidget *parent)
    : QWidget(parent) {
    // 메인 레이아웃 설정
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QScrollArea *scrollArea = new QScrollArea(this);

    // 스크롤 영역 내 콘텐츠 위젯
    QWidget *contentWidget = new QWidget(scrollArea);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentWidget->setObjectName("contentWidget");

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(contentWidget);

    // 우측 일정 표시 영역 레이아웃
    scheduleLayout = new QVBoxLayout();
    scheduleLayout->setObjectName("scheduleLayout");

    QWidget *scheduleWidget = new QWidget();
    scheduleWidget->setLayout(scheduleLayout);

    // 메인 콘텐츠 레이아웃
    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    mainContentLayout->addWidget(scheduleWidget, 3);  // 우측 일정

    // 뒤로가기 버튼 추가
    QPushButton *backButton = new QPushButton("뒤로가기", this);
    backButton->setFixedSize(100, 40);
    backButton->setStyleSheet(
        "background-color: #87CEEB;"
        "border: 1px solid #4682B4;"
        "border-radius: 5px;"
        "font-size: 14px;"
        "padding: 5px;"
        );

    // 레이아웃 내 버튼을 하단 고정
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();              // 버튼을 우측으로 밀기
    buttonLayout->addWidget(backButton);     // 버튼 추가


    // 뒤로가기 버튼 클릭 이벤트 연결
    connect(backButton, &QPushButton::clicked, this, &DayView::onBackButtonClicked);

    // 콘텐츠 레이아웃 연결
    contentLayout->addLayout(mainContentLayout);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(buttonLayout);
}

void DayView::displaySchedule(const QDate &date, const QList<Event> &events) {
    qDebug() << "Displaying schedule for date:" << date;

    // 기존 일정 제거
    QLayoutItem *item;
    while ((item = scheduleLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // 새로운 Grid Layout 생성
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // 시간 슬롯과 선 추가
    for (int row = 0; row < 24 * 2; ++row) {
        // 시간 표시 레이블
        QWidget* timeContainer = new QWidget();
        QVBoxLayout* timeLayout = new QVBoxLayout(timeContainer);
        timeLayout->setContentsMargins(0, 0, 0, 0);
        timeLayout->setSpacing(0);

        QLabel *timeLabel = new QLabel(
            QString("%1:%2")
                .arg(row / 2, 2, 10, QChar('0'))
                .arg((row % 2) * 30, 2, 10, QChar('0'))
            );
        timeLabel->setStyleSheet(
            "color: #666;"
            "font-size: 12px;"
            "padding-right: 5px;"
            "background: transparent;"
            );
        timeLabel->setFixedWidth(45);
        timeLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        timeLayout->addWidget(timeLabel);
        timeLayout->addStretch();

        // 가로선
        QFrame *line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Plain);
        line->setStyleSheet("background-color: #dddddd;");
        line->setFixedHeight(1);

        // 레이아웃에 추가
        gridLayout->addWidget(timeContainer, row, 0);
        gridLayout->addWidget(line, row, 1, 1, -1);
        gridLayout->setRowMinimumHeight(row, 30);
    }

    // 일정 추가
    for (const Event &event : events) {
        int startRow = event.startTime.hour() * 2 + (event.startTime.minute() / 30);
        int endRow = event.endTime.hour() * 2 + (event.endTime.minute() / 30);
        int rowSpan = endRow - startRow;

        QLabel *eventLabel = new QLabel(
            QString("%1\n%2 - %3")
                .arg(event.name)
                .arg(event.startTime.toString("hh:mm"))
                .arg(event.endTime.toString("hh:mm"))
            );
        eventLabel->setStyleSheet(
            "QLabel {"
            "   background-color: #ffcccc;"
            "   border: 1px solid #666666;"
            "   border-radius: 3px;"
            "   padding: 5px;"
            "   margin: 1px;"
            "}"
            );
        eventLabel->setWordWrap(true);
        eventLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        // 일정 배치 (겹치기 고려)
        int column = 1;
        while (true) {
            bool canPlace = true;
            for (int row = startRow; row < endRow; ++row) {
                if (gridLayout->itemAtPosition(row, column) != nullptr) {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace) {
                gridLayout->addWidget(eventLabel, startRow, column, rowSpan, 1);
                break;
            }
            ++column;
        }
    }

    // 새로운 레이아웃 설정
    QWidget *scheduleWidget = new QWidget();
    scheduleWidget->setLayout(gridLayout);
    QVBoxLayout *outerLayout = new QVBoxLayout();
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scheduleWidget);
    scheduleLayout->addLayout(outerLayout);
}


void DayView::onBackButtonClicked() {
    // 뒤로가기 버튼 클릭 시 동작 구현
    qDebug() << "Back button clicked!";
    emit backButtonClicked();  // 신호를 방출하여 부모 또는 다른 위젯에서 처리
}
