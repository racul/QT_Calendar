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

    const int ROW_HEIGHT = 30; // 각 행의 높이를 상수로 정의
    const int MIN_COLUMNS = 3; // 최소 column 수를 3으로 설정

    // 시간 슬롯과 선 추가
    for (int row = 0; row < 24 * 2; ++row) {
        // 시간 표시 레이블
        QLabel *timeLabel = new QLabel(
            QString("%1:%2")
                .arg(row / 2, 2, 10, QChar('0'))
                .arg((row % 2) * 30, 2, 10, QChar('0'))
            );
        timeLabel->setStyleSheet(
            "color: #666;"
            "font-size: 12px;"
            "padding-right: 5px;"
            );
        timeLabel->setFixedWidth(45);
        timeLabel->setFixedHeight(ROW_HEIGHT);
        timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // 가로선
        QFrame *line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Plain);
        line->setStyleSheet("background-color: #dddddd;");
        line->setFixedHeight(1);

        // 레이아웃에 추가
        gridLayout->addWidget(timeLabel, row, 0);
        gridLayout->addWidget(line, row, 1, 1, -1);
        gridLayout->setRowMinimumHeight(row, ROW_HEIGHT);
    }

    // 최소 3개의 column 확보
    for (int col = 1; col <= MIN_COLUMNS; ++col) {
        gridLayout->setColumnStretch(col, 1);
    }

    // 일정 추가
    for (const Event &event : events) {
        int startRow = event.startTime.hour() * 2 + (event.startTime.minute() / 30);
        int endRow = event.endTime.hour() * 2 + (event.endTime.minute() / 30);
        int rowSpan = endRow - startRow;
        if(rowSpan==0) {
            endRow += 1;
            rowSpan = 1;
        }

        QWidget* eventWidget = new QWidget();
        QVBoxLayout* eventLayout = new QVBoxLayout(eventWidget);
        eventLayout->setContentsMargins(2, 0, 2, 0);
        eventLayout->setSpacing(0);

        QLabel* eventLabel = new QLabel(
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
            "   margin: 0px;"
            "}"
            );
        eventLabel->setWordWrap(true);
        eventLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        eventLayout->addWidget(eventLabel);

        // 이벤트 위젯의 전체 높이 설정 (한 셀 더 추가)
        int totalHeight = (rowSpan + 1) * ROW_HEIGHT;
        eventWidget->setFixedHeight(totalHeight);

        // 컨테이너 위젯 생성
        QWidget* containerWidget = new QWidget();
        containerWidget->setObjectName("EventContainer"); // 객체 이름 설정

        QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);
        containerLayout->setContentsMargins(0, ROW_HEIGHT/2, 0, 0);
        containerLayout->setSpacing(0);
        containerLayout->addWidget(eventWidget);

        // 일정 배치 (겹치기 고려)
        int column = 1;
        bool placed = false;

        // 겹치지 않는 column을 찾을 때까지 반복
        while (!placed) {
            // 해당 column에 EventContainer가 있는지 확인
            if (!checkEventContainer(gridLayout, startRow, endRow, column)) {
                // EventContainer가 없으면 해당 column에 배치
                containerWidget->setObjectName("EventContainer");
                gridLayout->addWidget(containerWidget, startRow, column, rowSpan + 1, 1);
                placed = true;
            } else {
                // EventContainer가 있으면 다음 column 검사
                column++;
            }
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

bool DayView::checkEventContainer(QGridLayout* gridLayout, int startRow, int endRow, int column) {
    // 모든 아이템을 순회
    for (int i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem* item = gridLayout->itemAt(i);
        if (item && item->widget()) {
            // 아이템의 위치 정보 가져오기
            int itemRow, itemCol, rowSpan, colSpan;
            gridLayout->getItemPosition(i, &itemRow, &itemCol, &rowSpan, &colSpan);

            // 검사하려는 column과 row 범위에 있는지 확인
            if (itemCol == column &&
                item->widget()->objectName() == "EventContainer") {
                // 아이템의 row 범위가 검사 범위와 겹치는지 확인
                if ((itemRow <= endRow + 1 && itemRow + rowSpan > startRow)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}


void DayView::onBackButtonClicked() {
    // 뒤로가기 버튼 클릭 시 동작 구현
    qDebug() << "Back button clicked!";
    emit backButtonClicked();  // 신호를 방출하여 부모 또는 다른 위젯에서 처리
}
