#ifndef _GCODE_PARSER_H_
#define _GCODE_PARSER_H_

#include <Stream.h>
#include <gcode.h>

int parseGcodeLine(const String &line, Gcode &o_gcode);
int nextSpace(const String &line, int offset, int &end);
int nextSpaceOrEnd(const String &line, int offset);
int readGNum(const String &line, Gcode &o_gcode, int &end);
int readCoords(const String &line, int offset, Gcode &o_gcode);

#endif // _GCODE_PARSER_H_
