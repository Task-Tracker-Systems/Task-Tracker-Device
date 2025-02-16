#include <Arduino.h>
#include <FFat.h>

static void testFatFS() {
  File file = FFat.open("/test.txt", "w");
  if (!file) {
    Serial.println("Fehler beim Öffnen der Datei!");
    return;
  }
  file.println("Hallo FatFS!");
  file.close();

  file = FFat.open("/test.txt", "r");
  if (file) {
    Serial.println("Inhalt der Datei:");
    while (file.available()) {
      Serial.write(file.read());
    }
    Serial.println();
    file.close();
  } else {
    Serial.println("Konnte Datei nicht lesen.");
  }
}

void setup() {
  Serial.begin(115200);

  if (!FFat.begin(
          true)) { // `true` = Formatieren falls kein Dateisystem vorhanden
    Serial.println("FatFS konnte nicht gestartet werden!");
    return;
  }

  Serial.println("FatFS erfolgreich gemountet.");
}

void loop() {
  testFatFS();
  delay(10000);
}
