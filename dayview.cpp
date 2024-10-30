#include "dayview.h"
#include "ui_dayview.h"

// 하루 일정 화면 클래스

DayView::DayView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DayView)
{
    ui->setupUi(this);
}

DayView::~DayView()
{
    delete ui;
}
