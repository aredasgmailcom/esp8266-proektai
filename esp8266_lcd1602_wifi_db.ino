/*
 * Processor: NodeMCU (ESP8266)
 * Description: Wi-Fi network scanner. Displays discovered SSIDs and RSSI (signal strength) 
 * on LCD1602 (I2C) with a 3-second rotation.
 */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializuojame LCD ekraną (adresas 0x27, 16 stulpelių, 2 eilutės)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Pradedame I2C komunikaciją su standartiniais NodeMCU pinais
  Wire.begin(D2, D1);
  
  // Inicializuojame ekraną
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Nustatome Wi-Fi į stoties (Station) režimą ir atjungiame nuo bet kokio tinklo
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  lcd.setCursor(0, 0);
  lcd.print("Wi-Fi Skenavimas");
  lcd.setCursor(0, 1);
  lcd.print("Pradedamas...");
  delay(1500);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Skenuoju orą...");
  
  // WiFi.scanNetworks() grąžina rastų tinklų skaičių
  int tinkluSkaicius = WiFi.scanNetworks();
  
  if (tinkluSkaicius == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tinklu nerasta!");
    delay(3000);
  } else {
    // Sukamės per visus rastus tinklus
    for (int i = 0; i < tinkluSkaicius; i++) {
      lcd.clear();
      
      // 1 eilutė: Tinklo pavadinimas (SSID)
      lcd.setCursor(0, 0);
      String ssid = WiFi.SSID(i);
      // Jei pavadinimas ilgesnis nei 16 simbolių, nukertame, kad tilptų ekrane
      if (ssid.length() > 16) ssid = ssid.substring(0, 16);
      lcd.print(ssid);
      
      // 2 eilutė: Signalo stiprumas (RSSI) dBm ir kokybė procentais
      lcd.setCursor(0, 1);
      long rssi = WiFi.RSSI(i);
      
      // Konvertuojame dBm į procentus dėl aiškumo
      int procentai = 2 * (rssi + 100);
      if (procentai > 100) procentai = 100;
      else if (procentai < 0) procentai = 0;
      
      lcd.print(String(rssi) + "dBm  " + String(procentai) + "%");
      
      // Palaikome informaciją ekrane 3 sekundes, kaip ir prašėte
      delay(3000);
    }
  }
  
  // Trumpa pauzė prieš pradedant naują viso eterio skenavimą
  delay(1000);
}
