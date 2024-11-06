#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QDate>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onCellClicked(const QModelIndex &index);
    void onPrevMonthClicked();
    void onNextMonthClicked();
    void onAddEventButtonClicked();

private:
    void updateMonthLabel();
    void updateCalendarView();

    QTableView *calendarView;
    QPushButton *addEventButton;
    QPushButton *prevMonthButton;
    QPushButton *nextMonthButton;
    QLabel *monthLabel;
    QDate currentMonth;
};

#endif // MAINWINDOW_H
