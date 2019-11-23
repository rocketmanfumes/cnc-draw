#ifndef _GCODE_RUNNER_H_
#define _GCODE_RUNNER_H_

#include <gcode.h>

class DrawState;

int runGcode(const Gcode &gcode, DrawState &dstate, Stream &stream);
void drawLine(float xf1, float yf1, Stream &stream);
int milliToSteps(float millis);
float stepsToMillis(int steps);
static float atan3(float dy, float dx);

#define ARC_CW -1
#define ARC_CCW 1
#define MM_PER_SEGMENT 1
#define STEPS_PER_MM 1
class DrawState {
private:
  int x0;
  int y0;

public:
  DrawState(int xf, int yf) {
    x0 = milliToSteps(xf);
    y0 = milliToSteps(yf);
  }
  void drawLine(float xf1, float yf1, Stream &stream);
  void drawArc(float cx, float cy, float x, float y, int dir, Stream &stream);
};

#endif // _GCODE_RUNNER_H_
