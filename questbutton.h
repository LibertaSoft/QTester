#ifndef PIANOBUTTON_H
#define PIANOBUTTON_H

#include <QToolButton>

class QuestButton : public QToolButton
{
    Q_OBJECT
private:
    int _num;
public:
    explicit QuestButton(int num, QWidget *parent);

protected:
    QSize sizeHint();

signals:
    void activated(int);

private slots:
    void active();
};

#endif // PIANOBUTTON_H
