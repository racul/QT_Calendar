#include "calendarmodel.h"

CalendarModel::CalendarModel(const QDate &month, CalendarManager *manager, QObject *parent)
    : QAbstractTableModel(parent), calendarManager(manager)
{
    // 초기화
    // fisrtDayOfMonth : 해당 달의 1일을 QDate로 저장
    // daysInMonth : 해당 달이 28/29/30/31 중 몇 일까지 있는지 저장
    firstDayOfMonth = QDate(month.year(), month.month(), 1);
    daysInMonth = firstDayOfMonth.daysInMonth();
}

int CalendarModel::rowCount(const QModelIndex & /* parent */) const
{
    // 한 달을 표시할 때 필요한 행 수 (6행을 기본적으로 사용)
    return 6;
}

int CalendarModel::columnCount(const QModelIndex & /* parent */) const
{
    // 한 주는 7일이므로 7열 사용
    return 7;
}

QVariant CalendarModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int dayOfWeek = firstDayOfMonth.dayOfWeek();
    int cellNumber = index.row() * 7 + index.column();
    int dayNumber = cellNumber - (dayOfWeek - 1) + 1;

    if (dayNumber > 0 && dayNumber <= daysInMonth) {
        QDate date = QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), dayNumber);

        if (role == Qt::DisplayRole) {
            // 날짜를 반환
            return QString::number(dayNumber);
        }
        else if (role == Qt::UserRole) {
            // 첫 번째 일정 이름 반환 (있을 경우)
            QList<Event> events = calendarManager->getEventsForDate(date);
            if (!events.isEmpty()) {
                return events[0].name;
            }
        }
        else if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;  // 날짜는 상단 가운데 정렬
        }
    }
    return QVariant();
}


QVariant CalendarModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        // 열 헤더일 때 (수직 방향)
        if (orientation == Qt::Horizontal) {
            QStringList daysOfWeek = {"월", "화", "수", "목", "금", "토", "일"};
            if (section >= 0 && section < daysOfWeek.size()) {
                return daysOfWeek[section];
            }
        }
        // 행 헤더: "1주", "2주", "3주" 형식으로 설정
        else if (orientation == Qt::Vertical) {
            return QString("%1주").arg(section + 1);  // section은 0부터 시작하므로 1을 더함
        }
    }
    return QVariant();
}


QDate CalendarModel::getDateForIndex(const QModelIndex &index) const
{
    int dayOfWeek = firstDayOfMonth.dayOfWeek();
    int cellNumber = index.row() * 7 + index.column();
    int dayNumber = cellNumber - (dayOfWeek - 1) + 1;

    if (dayNumber > 0 && dayNumber <= daysInMonth) {
        return QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), dayNumber);
    }
    return QDate(); // 유효하지 않은 날짜일 경우
}

