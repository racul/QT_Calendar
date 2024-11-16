#ifndef EVENTEDITPAGE_H
#define EVENTEDITPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QPushButton>
#include <QLabel>
#include "calendarmanager.h"

class EventEditPage : public QWidget {
    Q_OBJECT

public:
    explicit EventEditPage(CalendarManager *manager, QWidget *parent = nullptr);
    void setEvent(const Event &event);
    void clearFields();
    void setDate(const QDate &date);

signals:
    void saveClicked();
    void cancelClicked();
    void deleteClicked();

private slots:
    void onSaveButtonClicked();
    void onDeleteButtonClicked();

private:
    QLabel *eventTitleLabel;
    QLineEdit *eventNameLineEdit;
    QDateEdit *dateEdit;
    QTimeEdit *startTimeEdit;
    QTimeEdit *endTimeEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *deleteButton;
    CalendarManager *calendarManager;
    Event currentEvent;
};

#endif
