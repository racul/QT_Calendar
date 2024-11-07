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
    // 파일에서 일정을 불러오는 기능 추가.
    return true;
}
