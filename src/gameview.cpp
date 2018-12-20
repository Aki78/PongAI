#include "gameview.h"
#include <QDebug>
#include <QKeyEvent>

GameView::GameView(QGraphicsScene *scene)
    : QGraphicsView(scene)
{
}

void GameView::keyPressEvent(QKeyEvent *e)
{
    // Do something
    if (e->key() == Qt::Key_Up) {
        emit upPressed();
    } else if (e->key() == Qt::Key_Down) {
        emit downPressed();
    } else if (e->key() == Qt::Key_Right) {
        emit rightPressed();
    } else if (e->key() == Qt::Key_Left) {
        emit leftPressed();
    }
}

void GameView::keyReleaseEvent(QKeyEvent *e)
{
    // Do something
    if (e->key() == Qt::Key_Up) {
        emit upReleased();
    } else if (e->key() == Qt::Key_Down) {
        emit downReleased();
    } else if (e->key() == Qt::Key_Right) {
        emit rightReleased();
    } else if (e->key() == Qt::Key_Left) {
        emit leftReleased();
    }
}
