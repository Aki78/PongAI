#ifndef BALLCONTACTLISTENER_H
#define BALLCONTACTLISTENER_H

class Game;
#include <Box2D/Box2D.h>

class BallContactListener : public b2ContactListener {
public:
    BallContactListener(Game* game);
    Game* mGame;
    void EndContact(b2Contact* contact);

};
#endif // BALLCONTACTLISTENER_H
