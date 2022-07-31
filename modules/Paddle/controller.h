//* Paddle controller *//
void centerPaddle(Paddle *paddle, int WINDOW_WIDTH, int WINDOW_HEIGHT) {
  paddle -> xPos = (WINDOW_WIDTH / 2) - (paddle -> rect.w / 2);
  paddle -> yPos = (WINDOW_HEIGHT / 2) - (paddle -> rect.h / 2);
  paddle -> rect.x = paddle -> xPos;
}
