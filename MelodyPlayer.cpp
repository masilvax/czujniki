#include "MelodyPlayer.h"

struct QueueItem {
  const Melody* melody;
  AlarmType type;
};

QueueItem melodyQueue[4];
int queueSize = 0;
int currentMelodyIndex = 0;
int currentNoteIndex = 0;
unsigned long lastNoteTime = 0;
bool notePlaying = false;
bool inPause = false;
unsigned long pauseStartTime = 0;
const unsigned long pauseDuration = 0; // 2 sekundy

void updateMelodyQueue(bool tempHigh, bool humiHigh, bool gasHigh, bool waterLeak) {

  noTone(BUZZER_PIN); // przerwa przed odtworzeniem - tak z delayem, bo już nie chciało mi się kombinować
  delay(1000);

  queueSize = 0;
  if (tempHigh) melodyQueue[queueSize++] = { getMelodyForAlarm(AlarmType::TempHigh), AlarmType::TempHigh };
  if (humiHigh) melodyQueue[queueSize++] = { getMelodyForAlarm(AlarmType::HumidityHigh), AlarmType::HumidityHigh };
  if (gasHigh)  melodyQueue[queueSize++] = { getMelodyForAlarm(AlarmType::GasDanger), AlarmType::GasDanger };
  if (waterLeak) melodyQueue[queueSize++] = { getMelodyForAlarm(AlarmType::WaterLeak), AlarmType::WaterLeak };

  currentMelodyIndex = 0;
  currentNoteIndex = 0;
  notePlaying = false;
  inPause = false;
  lastNoteTime = 0;
}

void playMelodyQueue() {
  if (queueSize == 0) return;

  // Obsługa przerwy między melodiami
  if (inPause) {
    if (millis() - pauseStartTime >= pauseDuration) {
      inPause = false;
      currentNoteIndex = 0;
      notePlaying = false; // przygotuj do zagrania nowej melodii
    } else {
      return; // wciąż trwa pauza
    }
  }

  QueueItem& item = melodyQueue[currentMelodyIndex];
  const Melody* melody = item.melody;

  // Jeśli nuta nie gra — rozpocznij
  if (!notePlaying) {
    if (currentNoteIndex < melody->length) {
      //tone(BUZZER_PIN, melody->notes[currentNoteIndex]);

      if (melody->notes[currentNoteIndex] == 0) {
        noTone(BUZZER_PIN); // pauza
      } else {
        tone(BUZZER_PIN, melody->notes[currentNoteIndex]);
      }

      lastNoteTime = millis();
      notePlaying = true;
    }
  }

  // Jeśli nuta gra, sprawdzamy czy skończyła grać
  if (notePlaying && millis() - lastNoteTime >= melody->durations[currentNoteIndex]) {
    noTone(BUZZER_PIN);
    notePlaying = false;
    delay(50);
    currentNoteIndex++;

    if (currentNoteIndex >= melody->length) {
      // koniec tej melodii — przejście do następnej
      currentNoteIndex = 0;
      currentMelodyIndex = (currentMelodyIndex + 1) % queueSize;
      inPause = true;
      pauseStartTime = millis();
    }
  }
}
