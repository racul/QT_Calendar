#include "eventeditpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>

EventEditPage::EventEditPage(CalendarManager *manager, QWidget *parent)
    : QWidget(parent), calendarManager(manager)
{
    // 메인 레이아웃 설정
    QVBoxLayout *editLayout = new QVBoxLayout(this);
    editLayout->setSpacing(20);
    editLayout->setContentsMargins(20, 20, 20, 20);

    // 제목 레이블 설정
    eventTitleLabel = new QLabel("일정 수정", this);
    eventTitleLabel->setAlignment(Qt::AlignCenter);
    eventTitleLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 24px;"
        "   color: #333333;"
        "   padding: 20px;"
        "   font-weight: bold;"
        "}"
        );

    // 입력 필드 그룹 박스 생성
    QGroupBox *inputGroup = new QGroupBox(this);
    inputGroup->setStyleSheet(
        "QGroupBox {"
        "   background-color: #ffffff;"
        "   border: 1px solid #dddddd;"
        "   border-radius: 8px;"
        "   margin: 10px;"
        "   padding: 15px;"
        "}"
        );

    QVBoxLayout *groupLayout = new QVBoxLayout(inputGroup);
    groupLayout->setSpacing(15);

    // 입력 필드 스타일 설정
    QString inputStyle =
        "QLineEdit, QDateEdit, QTimeEdit {"
        "   padding: 8px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 4px;"
        "   background-color: #f8f9fa;"
        "   min-height: 25px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus {"
        "   border-color: #87CEEB;"
        "   background-color: white;"
        "}";

    // 라벨 스타일 설정
    QString labelStyle =
        "QLabel {"
        "   color: #666666;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "}";

    // 입력 필드 생성 및 설정
    QLabel *nameLabel = new QLabel("일정 이름", inputGroup);
    eventNameLineEdit = new QLineEdit(inputGroup);

    QLabel *dateLabel = new QLabel("날짜", inputGroup);
    dateEdit = new QDateEdit(inputGroup);
    dateEdit->setCalendarPopup(true);

    QLabel *startLabel = new QLabel("시작 시간", inputGroup);
    startTimeEdit = new QTimeEdit(inputGroup);

    QLabel *endLabel = new QLabel("종료 시간", inputGroup);
    endTimeEdit = new QTimeEdit(inputGroup);

    // 스타일 적용
    nameLabel->setStyleSheet(labelStyle);
    dateLabel->setStyleSheet(labelStyle);
    startLabel->setStyleSheet(labelStyle);
    endLabel->setStyleSheet(labelStyle);

    eventNameLineEdit->setStyleSheet(inputStyle);
    dateEdit->setStyleSheet(inputStyle);
    startTimeEdit->setStyleSheet(inputStyle);
    endTimeEdit->setStyleSheet(inputStyle);

    // 그룹 레이아웃에 위젯 추가
    groupLayout->addWidget(nameLabel);
    groupLayout->addWidget(eventNameLineEdit);
    groupLayout->addWidget(dateLabel);
    groupLayout->addWidget(dateEdit);
    groupLayout->addWidget(startLabel);
    groupLayout->addWidget(startTimeEdit);
    groupLayout->addWidget(endLabel);
    groupLayout->addWidget(endTimeEdit);

    // 버튼 컨테이너 생성
    QWidget *buttonContainer = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setSpacing(10);

    // 버튼 스타일 설정
    QString buttonStyle =
        "QPushButton {"
        "   padding: 8px 20px;"
        "   border-radius: 4px;"
        "   font-weight: bold;"
        "   min-width: 100px;"
        "}"
        "QPushButton#saveButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "}"
        "QPushButton#saveButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton#cancelButton, QPushButton#deleteButton {"
        "   background-color: #f8f9fa;"
        "   border: 1px solid #ddd;"
        "   color: #666;"
        "}"
        "QPushButton#cancelButton:hover, QPushButton#deleteButton:hover {"
        "   background-color: #e9ecef;"
        "}"
        "QPushButton#deleteButton {"
        "   color: #dc3545;"
        "}";

    // 버튼 생성
    saveButton = new QPushButton("저장", buttonContainer);
    cancelButton = new QPushButton("취소", buttonContainer);
    deleteButton = new QPushButton("삭제", buttonContainer);

    saveButton->setObjectName("saveButton");
    cancelButton->setObjectName("cancelButton");
    deleteButton->setObjectName("deleteButton");

    saveButton->setStyleSheet(buttonStyle);
    cancelButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);

    // 버튼 이벤트 연결
    connect(saveButton, &QPushButton::clicked, this, &EventEditPage::onSaveButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, [this]() { emit cancelClicked(); });
    connect(deleteButton, &QPushButton::clicked, this, &EventEditPage::onDeleteButtonClicked);

    // 버튼 레이아웃 설정
    buttonLayout->addStretch();
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    // 메인 레이아웃에 위젯 추가
    editLayout->addWidget(eventTitleLabel);
    editLayout->addWidget(inputGroup);
    editLayout->addWidget(buttonContainer);
    editLayout->addStretch();

    // 초기 상태 설정
    deleteButton->setVisible(false);  // 새 일정 추가시에는 삭제 버튼 숨김
}

void EventEditPage::setEvent(const Event &event)
{
    currentEvent = event;
    eventTitleLabel->setText("일정 수정");
    eventNameLineEdit->setText(event.name);
    dateEdit->setDate(event.date);
    startTimeEdit->setTime(event.startTime);
    endTimeEdit->setTime(event.endTime);
    deleteButton->setVisible(true);
}

void EventEditPage::clearFields()
{
    currentEvent = Event();
    eventTitleLabel->setText("새 일정");
    eventNameLineEdit->clear();
    dateEdit->setDate(QDate::currentDate());
    startTimeEdit->setTime(QTime(13,0));
    endTimeEdit->setTime(QTime(14,0));
    deleteButton->setVisible(false);
}

void EventEditPage::setDate(const QDate &date)
{
    dateEdit->setDate(date);
}

void EventEditPage::onSaveButtonClicked()
{
    QString eventName = eventNameLineEdit->text();
    QDate eventDate = dateEdit->date();
    QTime startTime = startTimeEdit->time();
    QTime endTime = endTimeEdit->time();

    if (eventName.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "일정 이름을 입력하세요.");
        return;
    }

    // 시작 시간과 종료 시간 사이의 간격 검증
    int timeDifference = startTime.secsTo(endTime);
    if (timeDifference < 1800) { // 1800초 = 30분
        QMessageBox::warning(this, "시간 설정 오류", "종료 시간은 시작 시간보다 최소 30분 이상 뒤여야 합니다.");
        return;
    }

    bool success;
    if (currentEvent.name.isEmpty()) {
        success = calendarManager->saveEvent(eventName, eventDate, startTime, endTime);
    } else {
        success = calendarManager->updateEvent(currentEvent, eventName, eventDate, startTime, endTime);
    }

    if (success) {
        QMessageBox::information(this, "저장 완료", "일정이 저장되었습니다.");
        emit saveClicked();
    } else {
        QMessageBox::critical(this, "저장 실패", "일정을 저장하는 데 실패했습니다.");
    }
}

void EventEditPage::onDeleteButtonClicked()
{
    if (currentEvent.name.isEmpty()) {
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "일정 삭제",
                                  "이 일정을 삭제하시겠습니까?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (calendarManager->deleteEvent(currentEvent)) {
            QMessageBox::information(this, "삭제 완료", "일정이 삭제되었습니다.");
            emit deleteClicked();
        } else {
            QMessageBox::critical(this, "삭제 실패", "일정을 삭제하는 데 실패했습니다.");
        }
    }
}
