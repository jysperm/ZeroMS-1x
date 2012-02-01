#ifndef ONETWOKERDEBUGER_H
#define ONETWOKERDEBUGER_H

#include <QWidget>

namespace Ui {
class ONetwokerDebuger;
}

class ONetwokerDebuger : public QWidget
{
    Q_OBJECT
    
public:
    explicit ONetwokerDebuger(QWidget *parent = 0);
    ~ONetwokerDebuger();
    
private:
    Ui::ONetwokerDebuger *ui;
};

#endif // ONETWOKERDEBUGER_H
