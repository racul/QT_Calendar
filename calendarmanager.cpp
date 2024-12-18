#include "calendarmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>

/*
CalendarManager::CalendarManager(const QString &filename)
    : filename(filename) {}
*/

CalendarManager::CalendarManager(const QString &filename)
{
    // 프로그램 실행 경로에 data 폴더 생성
    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists("data")) {
        dir.mkdir("data"); // data 폴더가 없으면 생성
    }

    // data 폴더 내의 파일 경로 설정
    this->filename = QCoreApplication::applicationDirPath() + "/data/" + filename;
}


bool CalendarManager::saveEvent(const QString &name, const QDate &date, const QTime &startTime, const QTime &endTime)
{
    Event event = { name, date, startTime, endTime };
    events.append(event);

    QFile file(filename);

    // 파일 디렉토리 확인 및 생성
    QFileInfo fileInfo(filename);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create directory:" << dir.path();
            return false;
        }
    }

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    out << name << ","
        << date.toString("yyyy-MM-dd") << ","
        << startTime.toString("HH:mm") << ","
        << endTime.toString("HH:mm") << "\n";

    file.close();
    return true;
}

bool CalendarManager::loadEvents()
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return false;
    }

    events.clear();  // 기존 일정 데이터를 초기화

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() != 4) {
            qWarning() << "Invalid line format in file:" << line;
            continue;
        }

        Event event;
        event.name = fields[0];
        event.date = QDate::fromString(fields[1], "yyyy-MM-dd");
        event.startTime = QTime::fromString(fields[2], "hh:mm");
        event.endTime = QTime::fromString(fields[3], "hh:mm");

        if (event.date.isValid() && event.startTime.isValid() && event.endTime.isValid()) {
            events.append(event);
        } else {
            qWarning() << "Invalid event data:" << line;
        }
    }

    file.close();
    return true;
}

QList<Event> CalendarManager::getEventsForDate(const QDate &date) const
{
    QList<Event> eventsForDate;
    for (const Event &event : events) {
        if (event.date == date) {
            eventsForDate.append(event);
        }
    }
    return eventsForDate;
}

bool CalendarManager::updateEvent(const Event &oldEvent, const QString &newName, const QDate &newDate,
                                  const QTime &newStartTime, const QTime &newEndTime)
{
    // 기존 이벤트를 찾아 수정
    for (int i = 0; i < events.size(); ++i) {
        if (events[i].name == oldEvent.name &&
            events[i].date == oldEvent.date &&
            events[i].startTime == oldEvent.startTime &&
            events[i].endTime == oldEvent.endTime) {

            // 이벤트 정보 업데이트
            events[i].name = newName;
            events[i].date = newDate;
            events[i].startTime = newStartTime;
            events[i].endTime = newEndTime;

            // 파일에 모든 이벤트를 다시 쓰기
            QFile file(filename);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug() << "Failed to open file for writing:" << file.errorString();
                return false;
            }

            QTextStream out(&file);
            for (const Event &event : events) {
                out << event.name << ","
                    << event.date.toString("yyyy-MM-dd") << ","
                    << event.startTime.toString("HH:mm") << ","
                    << event.endTime.toString("HH:mm") << "\n";
            }

            file.close();
            return true;
        }
    }

    // 일치하는 이벤트를 찾지 못한 경우
    qDebug() << "Event not found for update";
    return false;
}


bool CalendarManager::deleteEvent(const Event &event)
{
    // 이벤트 목록에서 해당 이벤트 제거
    for (int i = 0; i < events.size(); ++i) {
        if (events[i].name == event.name &&
            events[i].date == event.date &&
            events[i].startTime == event.startTime &&
            events[i].endTime == event.endTime) {
            events.removeAt(i);
            break;
        }
    }

    // 파일에 업데이트된 이벤트 목록 쓰기
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    for (const Event &e : events) {
        out << e.name << ","
            << e.date.toString("yyyy-MM-dd") << ","
            << e.startTime.toString("HH:mm") << ","
            << e.endTime.toString("HH:mm") << "\n";
    }

    file.close();
    return true;
}
