
# PongAI

This is a pong AI to test the genetic algorithm on a pong game. The game logic and UI was made with Box2D and Qt by [Lauri Himanen](https://github.com/lauri-codes), and the genetic algorithm was made with the [Armadillo](http://arma.sourceforge.net/) package, which made prototyping quite easy.

The AI with a neural network was trained in 2 steps, first learn to follow the ball, then maximize own winning score while minimizing the opponents winning score. Training step 1 hit optimal in a few hours while in training step 2, the AI became nearly undefeatable by a simple pong that just tries to follow the ball.

The relevant inputs were [xBall,yBall,xSelf,ySelf,xOppo,yOppo,vxBall,vyBall,vxSelf,vySelf,vxOppo,vyOppo], and output was the decision [up,stay,down].

Interesting Note:
At some point in training step 2, it learned to vibrate rappidly like a bee to kick the ball out of phase on the edge to bounce the ball rapidly so that the opponent couldn't follow it fast enough, which was surprising.
