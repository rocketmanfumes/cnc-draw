#include <gcode_runner.h>
#include <math.h>

int runGcode(const Gcode &gcode, DrawState &dstate, Stream &stream) {
  if (gcode.num() == 0 || gcode.num() == 1) { // just a line
    dstate.drawLine(gcode.x(), gcode.y(), stream);
  } else if (gcode.num() == 2) {
    dstate.drawArc(gcode.x(), gcode.y(), gcode.i(), gcode.j(), ARC_CW, stream);
  } else if (gcode.num() == 3) {
    dstate.drawArc(gcode.x(), gcode.y(), gcode.i(), gcode.j(), ARC_CCW, stream);
  }
  return 0;
}

int milliToSteps(float millimeters) {
  return (int)(millimeters * STEPS_PER_MM);
}
float stepsToMillis(int steps) { return ((float)steps) / (float)STEPS_PER_MM; }

// Bresenham's algorithm
// inputs are units in steps rather than millimeters
void DrawState::drawLine(float xf1, float yf1, Stream &stream) {
  int x1 = milliToSteps(xf1);
  int y1 = milliToSteps(yf1);
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
    stream.println((x0 + (String) "," + y0));
  }
}

static float atan3(float dy, float dx) {
  float a = atan2(dy, dx);
  if (a < 0)
    a = (M_PI * 2.0) + a;
  return a;
}

// Mostly taken from
// https://www.marginallyclever.com/2014/03/how-to-improve-the-2-axis-cnc-gcode-interpreter-to-understand-arcs/
// This method assumes the limits have already been checked.
// This method assumes the start and end radius match (both points are same
// distance from center)
// cx/cy - center of circle
// x/y - end position
// dir - ARC_CW or ARC_CCW to control direction of arc
// (posx,posy,posz) is the starting position
void DrawState::drawArc(float x, float y, float gi, float gj, float dir,
                        Stream &stream) {
  float posx = stepsToMillis(x0);
  float posy = stepsToMillis(y0);
  // get circle centre
  float cx = posx + gi;
  float cy = posy + gj;

  // get radius
  float sx = posx - cx;
  float sy = posy - cy;
  float dx = x - cx;
  float dy = y - cy;
  float radius = sqrt(dx * dx + dy * dy);

  // find the sweep of the arc
  float angle1 = atan3(sy, sx);
  float angle2 = atan3(dy, dx);
  float sweep = angle2 - angle1;

  sweep = angle2 - angle1;

  if (dir > 0 && sweep < 0) {
    sweep = 2 * M_PI + sweep;
  }

  // get length of arc
  // float circumference=PI*2.0*radius;
  // float len=sweep*circumference/(PI*2.0);
  // simplifies to
  float len = (float)fabs(sweep) * (float)radius;

  int num_segments = floor(len / MM_PER_SEGMENT);

  // declare variables outside of loops because compilers can be really dumb
  // and inefficient some times.
  float nx, ny, nz, angle3, fraction;

  for (int i = 0; i < num_segments; ++i) {
    // interpolate around the arc
    fraction = ((float)i) / ((float)num_segments);
    angle3 = (sweep * fraction) + angle1;

    // find the intermediate position
    nx = cx + cos(angle3) * radius;
    ny = cy + sin(angle3) * radius;
    // make a line to that intermediate position
    drawLine(nx, ny, stream);
  }

  // one last line hit the end
  drawLine(x, y, stream);
}
