#include "myListView.h"
#include "showMessage.h"

MyListView::MyListView(QWidget *parent)
    : QListView(parent)
{
    // Създаване на изскачащо меню
    ppm = new QMenu(this);
    a1 = new QAction(tr("Copy list"), this);
    a2 = new QAction(tr("Copy verses"), this);
    ppm->addAction(a1);
    ppm->addAction(a2);

    connect(a1, SIGNAL(triggered()), this, SLOT(onCopyList()));
    connect(a2, SIGNAL(triggered()), this, SLOT(onCopyVerses()));

}

QModelIndexList MyListView::selectedIndexes() const {
    return QListView::selectedIndexes();
}

void MyListView::keyPressEvent(QKeyEvent *event) {
    QListView::keyPressEvent(event);
    if (!selectedIndexes().isEmpty() &&
        (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)) {
        emit clicked(selectedIndexes().at(0));
    }
}

void MyListView::contextMenuEvent(QContextMenuEvent *event) {
    QListView::contextMenuEvent(event);
    ppm->exec(QCursor::pos());
}

void MyListView::onCopyList() {
    emit popupClicked(0);
}

void MyListView::onCopyVerses() {
    emit popupClicked(1);
}
