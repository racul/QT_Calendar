#include "createeventdialog.h"
#include "ui_createeventdialog.h"

// 새로운 일정 생성 화면 클래스 정의

CreateEventDialog::CreateEventDialog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CreateEventDialog)
{
    ui->setupUi(this);
}

CreateEventDialog::~CreateEventDialog()
{
    delete ui;
}
