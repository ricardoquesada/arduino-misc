#include <WEMOS_Matrix_LED.h>

MLED mled(0); //intensity=0

void setup() {

  for (int i = 0; i < 8; i++)
  {
    mled.disBuffer[i] = 0xff; //full screen
  }
}
void loop() {
  for (int y = 0; y < 8; y++)
  {
    mled.intensity = y; //change intensity
    for (int x = 0; x < 8; x++)
    {
      mled.dot(x, y); // draw dot
      mled.display();
      delay(200);
      mled.dot(x, y, 0); //clear dot
      mled.display();
      delay(200);
    }
  }
}
