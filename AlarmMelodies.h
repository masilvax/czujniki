#ifndef ALARM_MELODIES_H
#define ALARM_MELODIES_H

#include "AlarmTypes.h"

struct Melody {
  const int* notes;
  const int* durations;
  int length;
};

const Melody* getMelodyForAlarm(AlarmType type);

#endif