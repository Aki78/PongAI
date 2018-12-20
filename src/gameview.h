#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>

class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    GameView(QGraphicsScene *scene);

signals:
    void upPressed();
    void downPressed();
    void upReleased();
    void downReleased();
    void rightPressed();
    void leftPressed();
    void rightReleased();
    void leftReleased();

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};

#endif // GAMEVIEW_H
