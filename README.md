# esp8266-proektai
# Mano ESP8266 Projektas

Šis kodas skirtas valdyti LED per WiFi.

### Svarbios pastabos:
* Naudoti **115200** baud rate.
* Jungti LED prie **D1** (GPIO5) pino.

### Pagrindinis kodo fragmentas:
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}
