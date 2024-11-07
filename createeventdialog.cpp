#include "createeventdialog.h"
#include "ui_createeventdialog.h"

CreateEventDialog::CreateEventDialog(QWidget *parent) :
    QMainWindow(parent),  // QMainWindow로 변경
    ui(new Ui::CreateEventDialog)
{
    ui->setupUi(this);

    // 버튼 시그널-슬롯 연결
    connect(ui->saveButton, &QPushButton::clicked, this, &CreateEventDialog::onSaveButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &CreateEventDialog::onCancelButtonClicked);
}

CreateEventDialog::~CreateEventDialog()
{
    delete ui;
}

void CreateEventDialog::onSaveButtonClicked()
{
    // 일정 저장 로직
    this->close();  // 창 닫기
}

void CreateEventDialog::onCancelButtonClicked()
{
    this->close();  // 창 닫기
}
