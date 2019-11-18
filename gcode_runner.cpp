#include <gcode_runner.h>

int runGcode(const Gcode &gcode, double curX, double curY, Stream &stream) {
  if (gcode.num() == 0 || gcode.num() == 1) {
    drawLine(milliToSteps(curX), milliToSteps(curY), milliToSteps(gcode.x()),
             milliToSteps(gcode.y()), stream);
  }
  return 0;
}

int milliToSteps(double millimeters) {
  return (int)(millimeters * (double)200);
}

// Bresenham's algorithm
// inputs are units in steps rather than millimeters
void drawLine(int x0, int y0, int x1, int y1, Stream &stream) {
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy; /* error value e_xy */
  while (true) {     /* loop */
    if (x0 == x1 && y0 == y1)
      break;
    long e2 = 2 * err;
    if (e2 >= dy) {
      err += dy; /* e_xy+e_x > 0 */
      x0 += sx;
    }
    if (e2 <= dx) { /* e_xy+e_y < 0 */
      err += dx;
      y0 += sy;
    }
    stream.println((String) "X,Y: (" + x0 + (String) ", " + y0 + (String) ")");
  }
}
