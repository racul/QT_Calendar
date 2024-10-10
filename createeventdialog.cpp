#include "createeventdialog.h"
#include "ui_createeventdialog.h"

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
