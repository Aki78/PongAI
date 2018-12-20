#include "ballcontactlistener.h"
#include "game.h"
#include <QDebug>

BallContactListener::BallContactListener(Game* game)
    : mGame(game)
{
}

void BallContactListener::EndContact(b2Contact* contact) {
    void *ptr = contact->GetFixtureA()->GetUserData();
    if (ptr) {
        if (ptr != mGame->mPrevContact) {
            mGame->mPrevContact = ptr;
            b2Vec2 oldVelocity = mGame->mBall.mBody->GetLinearVelocity();
            b2Vec2 addition = oldVelocity;
            addition.Normalize();
            mGame->mBall.setVelocity(oldVelocity + addition);
        }
    }
}
