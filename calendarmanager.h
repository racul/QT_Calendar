#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QList>

struct Event {
    QString name;       // 일정 이름
    QDate date;         // 일정 날짜
    QTime startTime;    // 시작 시간
    QTime endTime;      // 종료 시간
};

class CalendarManager
{
public:
    explicit CalendarManager(const QString &filename);

    bool saveEvent(const QString &name, const QDate &date, const QTime &startTime, const QTime &endTime);
    bool loadEvents();
    QList<Event> getEventsForDate(const QDate &date) const;  // 특정 날짜의 일정 가져오기

private:
    QString filename;   // CSV 파일 경로
    QList<Event> events; // 일정 목록
};

#endif // CALENDARMANAGER_H
