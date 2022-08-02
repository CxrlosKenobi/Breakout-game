//* Paddle controller *//
void movePaddle(Paddle *paddle, int up, int down, int left, int right, int WINDOW_WIDTH, int WINDOW_HEIGHT, int SPEED) {
  paddle -> speed.x = 0;
  if (left == 1 && right == 0)
    paddle -> speed.x = -SPEED/2;
  if (right == 1 && left == 0)
    paddle -> speed.x = SPEED/2;
  
  paddle -> xPos += paddle -> speed.x / 60;
  paddle -> yPos = 400;

  paddle -> rect.x = (int)paddle -> xPos;
  paddle -> rect.y = (int)paddle -> yPos;

  // Keep player sprite in the bounds of the window
  if (paddle -> xPos <= 0)
    paddle -> xPos = 0;
  if (paddle -> yPos <= 0)
    paddle -> yPos = 0;
  if (paddle -> xPos >= (WINDOW_WIDTH - paddle -> rect.w))
    paddle -> xPos = WINDOW_WIDTH - paddle -> rect.w;
  if (paddle -> yPos >= (WINDOW_HEIGHT - paddle -> rect.h))
    paddle -> yPos = WINDOW_HEIGHT - paddle -> rect.h;
}


void centerPaddle(Paddle *paddle, int WINDOW_WIDTH, int WINDOW_HEIGHT) {
  paddle -> xPos = (WINDOW_WIDTH / 2) - (paddle -> rect.w / 2);
  paddle -> yPos = (WINDOW_HEIGHT / 2) - (paddle -> rect.h / 2);
  paddle -> rect.x = paddle -> xPos;
}
