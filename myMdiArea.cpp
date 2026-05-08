#include "mymdiarea.h"

#include <QPainter>

MyMdiArea::MyMdiArea(QWidget *parent)
    : QMdiArea(parent)
{
}

void MyMdiArea::paintEvent(QPaintEvent *event)
{
    QMdiArea::paintEvent(event);

    if (subWindowList().isEmpty()) {

        QPainter p(viewport());

        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 18));

        p.drawText(viewport()->rect(),
                   Qt::AlignCenter,
                   tr("Select a Bible translation\nf"
                      "rom the Bible menu.\n"
                      "If it is not available, confirm\n"
                      "that you want to download it from the Internet."));
    }
}