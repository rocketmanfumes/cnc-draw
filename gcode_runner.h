#ifndef _GCODE_RUNNER_H_
#define _GCODE_RUNNER_H_

#include <gcode.h>

int runGcode(const Gcode &gcode, double curX, double curY, Stream &stream);
void drawLine(int x0, int y0, int x1, int y1, Stream &stream);
int milliToSteps(double millis);

#endif // _GCODE_RUNNER_H_
