#ifndef MYMDIAREA_H
#define MYMDIAREA_H

#include <QMdiArea>

class MyMdiArea : public QMdiArea

{

    Q_OBJECT

public:

    explicit MyMdiArea(QWidget *parent = nullptr);

protected:

    void paintEvent(QPaintEvent *event) override;

};

#endif // MYMDIAREA_H
