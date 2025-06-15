#include "AlarmMelodies.h"

// C - 262 - 524
// D - 294 - 588
// E - 330 - 660
// F - 349 - 698
// G - 392 - 784
// A - 440 - 880
// Ais-466 - 932
// B - 494 - 988

const int melodyTemp[] =   { 440, 524, 588,   0, 524, 440, 524,   0, 440, 392,   0, 440, 524,   0, 524,   0, 440, 524, 588,   0 };        // fragment „Night Fever”
const int durationTemp[] = { 200, 200, 200, 200, 200, 200, 200, 200, 200, 700, 450,  75,  75, 200,  75,  75,  75, 325,  75, 3075 };
const int melodyHumi[] = {392, 440, 494};
const int durationHumi[] = {200, 200, 200};
const int melodyGas[] =   { 294,   0, 294, 294, 330,   0, 392, 440, 494,   0, 330, 392, 440, 494, 440, 392, 330, 392, 440, 494, 440, 392,   0 }; // we didn't start the fire
const int durationGas[] = { 325,  75, 200, 200, 325,  75, 200, 450, 200, 950, 200, 200, 450, 200, 450, 200, 200, 200, 450, 200, 450, 200, 450 };
const int melodyWater[] =   { 524,   0, 392,   0, 330,   0, 440,  0, 494,  0, 466, 440,  0, 392,  0, 660, 784,  0, 880,  0, 698, 784,  0, 660,  0, 524, 588, 494,   0 }; // super mario
const int durationWater[] = {  75, 200,  75, 200,  75, 200,  75, 75,  75, 75,  75,  75, 75,  75, 13,  75,  75, 13,  75, 75,  75,  75, 75,  75, 75,  75,  75,  75, 200 };

const Melody tempMelody = {melodyTemp, durationTemp, sizeof(melodyTemp) / sizeof(melodyTemp[0])};
const Melody humiMelody = {melodyHumi, durationHumi, sizeof(melodyHumi) / sizeof(melodyHumi[0])};
const Melody gasMelody = {melodyGas, durationGas, sizeof(melodyGas) / sizeof(melodyGas[0])};
const Melody waterMelody = {melodyWater, durationWater, sizeof(melodyWater) / sizeof(melodyWater[0])};

const Melody* getMelodyForAlarm(AlarmType type) {
  switch (type) {
    case AlarmType::TempHigh: return &tempMelody;
    case AlarmType::HumidityHigh: return &humiMelody;
    case AlarmType::GasDanger: return &gasMelody;
    case AlarmType::WaterLeak: return &waterMelody;
    default: return nullptr;
  }
}