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
    bool checkEventContainer(QGridLayout* gridLayout, int startRow, int endRow, int column);
    void displaySchedule(const QDate &date, const QList<Event> &events);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void backButtonClicked(); // 뒤로가기 버튼 신호
    void eventDoubleClicked(const Event &event); // 이벤트 더블클릭 신호 추가

private slots:
    void onBackButtonClicked(); // 뒤로가기 슬롯
    void onEventDoubleClicked(const Event &event); // 이벤트 더블클릭 슬롯 추가


private:
    QLabel *dateLabel;
    QVBoxLayout *scheduleLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
};

#endif // DAYVIEW_H
