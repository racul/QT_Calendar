#include "calendardelegate.h"
#include <QApplication>
#include <QDate>

CalendarDelegate::CalendarDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void CalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QDate currentDate = QDate::currentDate();
    QDate cellDate = index.data(Qt::UserRole).toDate();  // 셀의 날짜 정보

    // 셀 배경 색상 설정
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor("#E0E0E0"));  // 연한 회색 배경 (선택된 경우)
    } else if (cellDate == currentDate) {
        painter->fillRect(option.rect, QColor(255, 255, 224));  // 연한 노란색 배경 (현재 날짜)
    } else {
        painter->fillRect(option.rect, option.palette.base());
    }

    // 날짜 및 일정 정보 가져오기
    QString dayText = index.data(Qt::DisplayRole).toString();  // 날짜 정보
    QString eventText = index.data(Qt::UserRole + 1).toString();   // 일정 정보

    // 텍스트 위치 설정
    QRect dayRect = option.rect.adjusted(5, 5, -5, -option.rect.height() / 2);
    QRect eventRect = option.rect.adjusted(5, option.rect.height() / 2, -5, -5);

    // 날짜 텍스트 (가운데 정렬)
    painter->setPen(Qt::black);
    painter->drawText(dayRect, Qt::AlignCenter, dayText);

    // 일정 텍스트 (왼쪽 정렬 및 회색 배경)
    if (!eventText.isEmpty()) {
        painter->setBrush(QColor("#D3D3D3"));  // 회색 배경
        painter->setPen(Qt::black);
        painter->drawRect(eventRect);          // 일정 배경
        painter->drawText(eventRect.adjusted(3, 0, -3, 0), Qt::AlignLeft | Qt::AlignVCenter, eventText);
    }

    painter->restore();
}

QSize CalendarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    return QSize(size.width(), size.height() + 20);  // 일정이 들어갈 공간 추가
}
