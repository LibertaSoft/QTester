#include "questbutton.h"

QuestButton::QuestButton(int num, QWidget *parent = 0)
    : QToolButton(parent), _num(num-1)
{
    QToolButton::setText( QString::number(_num) );
    connect( this, SIGNAL( clicked() ), this, SLOT( active() ) );
}

QSize QuestButton::sizeHint()
{
    return QSize(32, 32);
}

void QuestButton::active()
{
    emit activated(_num);
}
