#include "calendarmodel.h"

CalendarModel::CalendarModel(const QDate &month, QObject *parent)
    : QAbstractTableModel(parent)
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
    if (role == Qt::DisplayRole) {
        // 달력의 첫 주에 시작되는 요일을 기준으로 날짜를 계산
        int dayOfWeek = firstDayOfMonth.dayOfWeek(); // 첫 날의 요일(1 = 월요일)
        int cellNumber = index.row() * 7 + index.column(); // 셀 번호 계산
        int dayNumber = cellNumber - (dayOfWeek - 1) + 1;  // 실제 날짜 계산

        if (dayNumber > 0 && dayNumber <= daysInMonth) {
            return QString::number(dayNumber); // 유효한 날짜면 숫자 표시
        }
    }
    // TextAlignmentRole: 텍스트 가운데 정렬 설정
    else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;  // 가운데 정렬 설정
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

