#include "DialogUtils.h"

#include <QScreen>
#include <QGuiApplication>

bool ensureDialogFitsScreen(QDialog *dlg, bool szh )
{
    bool rez = false;
    if (!dlg) return rez;
    dlg->ensurePolished();
    QSize dlgSize;
    if(szh){
        dlg->adjustSize();
        dlgSize = dlg->sizeHint();
    }
    else dlgSize = dlg->size();
    QScreen *screen = dlg->screen();
    if (!screen) screen = QGuiApplication::primaryScreen();
    if (!screen) return rez;
    QRect screenRect = screen->availableGeometry();
    if (dlgSize.width() > screenRect.width() || dlgSize.height() > screenRect.height())
    {
        dlg->setWindowState(Qt::WindowFullScreen);
        return true;
    }
    int x = screenRect.x() + (screenRect.width() - dlgSize.width()) / 2;
    int y = screenRect.y() + (screenRect.height() - dlgSize.height()) / 2;
    dlg->move(x, y);
    return rez;
}