#ifndef DAYVIEW_H
#define DAYVIEW_H

#include <QMainWindow>

namespace Ui {
class DayView;
}

class DayView : public QMainWindow
{
    Q_OBJECT

public:
    explicit DayView(QWidget *parent = nullptr);
    ~DayView();

private:
    Ui::DayView *ui;
};

#endif // DAYVIEW_H
