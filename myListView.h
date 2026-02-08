#pragma once

#include <QListView>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

/* MyListView - наследник на QListView за показване на списък с намерени стихове.
   Добавя контекстно меню с възможност за изпращане на сигнал при избор на действие. */

class MyListView : public QListView
{
    Q_OBJECT

public:
    explicit MyListView(QWidget *parent = nullptr);

    // Предефинира се само за да се направи public
    QModelIndexList selectedIndexes() const override;

signals:
    void popupClicked(int i);  // Сигнал при щракване върху менюто

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onCopyList();
    void onCopyVerses();

private:
    QMenu *ppm;
    QAction *a1;
    QAction *a2;
};
