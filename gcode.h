#ifndef _GCODE_H_
#define _GCODE_H_

#include <Stream.h>

class Gcode {
private:
  int m_num;
  double m_x;
  double m_y;
  double m_z;
  double m_j;
  double m_i;
  int m_has;

public:
  Gcode(int gNum) : m_num(gNum), m_has(0){};
  bool hasX() const { return (m_has & 0x01); }
  bool hasY() const { return (m_has & 0x02); }
  bool hasZ() const { return (m_has & 0x04); }
  bool hasI() const { return (m_has & 0x08); }
  bool hasJ() const { return (m_has & 0x010); }

  void setX(double x) {
    m_x = x;
    m_has |= 0x01; // first bit
  }
  void setY(double y) {
    m_y = y;
    m_has |= 0x02; // second bit
  }
  void setZ(double z) {
    m_z = z;
    m_has |= 0x04; // third bit
  }
  void setI(double i) {
    m_i = i;
    m_has |= 0x08; // fourth bit
  }
  void setJ(double j) {
    m_j = j;
    m_has |= 0x010; // fifth bit
  }

  double x() const { return m_x; }
  double y() const { return m_y; }
  double z() const { return m_z; }
  double i() const { return m_i; }
  double j() const { return m_j; }
  int num() const { return m_num; }
};

#define ERROR 0
#define SUCCESS 1

#endif
