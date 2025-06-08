#ifndef CONFIG_H
#define CONFIG_H

// Ustawienia czujników
// gaz i ciecz fizycznie nie obok siebie w nadziei, że jedno nie będzie wpływać na drugie, 
// ale i tak jak gaz jest na maxa to wpływa na ciecz.
#define DHTPIN 27           // Pin DHT22
#define DHTTYPE DHT22
#define GAS_SENSOR_PIN 35   // Pin czujnika gazu (MQ-5) fizycznie
#define WATER_SENSOR_PIN 36 // VP (ADC1_CHANNEL_0) - Pin czujnika zalania

#endif