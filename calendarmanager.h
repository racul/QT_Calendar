#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QList>

class CalendarManager
{
public:
    explicit CalendarManager(const QString &filename);

    bool saveEvent(const QString &name, const QDate &date, const QTime &startTime, const QTime &endTime);
    bool loadEvents();

private:
    QString filename;
};

#endif // CALENDARMANAGER_H
