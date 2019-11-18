#include <gcode_parser.h>

int parseGcodeLine(const String &line, Gcode &o_gcode) {
  int endIdx;

  if (!readGNum(line, o_gcode, endIdx)) {
    return ERROR;
  }

  if (!readCoords(line, endIdx + 1, o_gcode)) {
    return ERROR;
  }

  return SUCCESS;
}

int readGNum(const String &line, Gcode &o_gcode, int &end) {
  if (line.length() < 2) {
    return ERROR;
  }

  if (line[0] != 'G') {
    return ERROR;
  }

  int endIdx = line.indexOf(' ', 1);
  if (endIdx == -1) {
    return ERROR;
  }

  // This toInt will return a 0 if it can't convert
  // Basically can't tell if this errors so it defaults
  // to 0
  int gcodeNum = line.substring(1, endIdx).toInt();
  o_gcode = Gcode(gcodeNum);

  end = endIdx;
  return SUCCESS;
}

int readCoords(const String &line, int offset, Gcode &o_gcode) {
  char direction;
  int dirIdx = offset;
  if (dirIdx < line.length()) {
    direction = line[dirIdx];
  } else {
    return ERROR;
  }
  while (direction == 'X' || direction == 'Y' || direction == 'Z' ||
         direction == 'I' || direction == 'J') {
    // Check that there is something after the direction
    int numOffset = dirIdx + 1;
    if (numOffset >= line.length()) {
      // These coords should always have a number attached
      return ERROR;
    }
    int endIdx = nextSpaceOrEnd(line, numOffset);
    double value = line.substring(numOffset, endIdx).toDouble();
    switch (direction) {
    case 'X': {
      o_gcode.setX(value);
      break;
    }
    case 'Y': {
      o_gcode.setY(value);
      break;
    }
    case 'Z': {
      o_gcode.setZ(value);
      break;
    }
    case 'I': {
      o_gcode.setI(value);
      break;
    }
    case 'J': {
      o_gcode.setJ(value);
      break;
    }
    default:
      break;
    }
    // A single space separates them
    int nextDirIdx = endIdx + 1;
    if (nextDirIdx < line.length()) {
      direction = line[nextDirIdx];
      dirIdx = nextDirIdx;
    } else {
      break;
    }
  }
  return SUCCESS;
}

int nextSpaceOrEnd(const String &line, int offset) {
  int idx = line.indexOf(' ', offset);
  if (idx == -1) {
    return line.length();
  }
  return idx;
}
