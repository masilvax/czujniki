#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include <Arduino.h>
#include "AlarmTypes.h"
#include "AlarmMelodies.h"

#define BUZZER_PIN 13

void updateMelodyQueue(bool tempHigh, bool humiHigh, bool gasHigh, bool waterLeak);
void playMelodyQueue();

#endif