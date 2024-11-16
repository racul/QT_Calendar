#ifndef DAYVIEW_H
#define DAYVIEW_H

#include "calendarmanager.h"
#include <QWidget>
#include <QDate>
#include <QList>

class QLabel;
class QVBoxLayout;
class QGridLayout;
class QScrollArea;

class DayView : public QWidget {
    Q_OBJECT

public:
    explicit DayView(QWidget *parent = nullptr);
    void displaySchedule(const QDate &date, const QList<Event> &events);

signals:
    void backButtonClicked(); // 뒤로가기 버튼 신호

private slots:
    void onBackButtonClicked(); // 뒤로가기 슬롯

private:
    QLabel *dateLabel;
    QVBoxLayout *scheduleLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
};

#endif // DAYVIEW_H
