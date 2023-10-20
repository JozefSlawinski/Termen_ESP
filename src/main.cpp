#include <Wire.h>
#include <U8g2lib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BluetoothSerial.h>

// Ustawienia dla DS18B20
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Ustawienia dla OLED
U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 5, /* dc=*/ 17, /* reset=*/ 16);

// Inicjalizacja BluetoothSerial
BluetoothSerial SerialBT;

void setup() {
  // Rozpocznij komunikację z sensorami temperatury
  sensors.begin();

  // Rozpocznij komunikację z wyświetlaczem OLED
  u8g2.begin();

  // Inicjalizacja Bluetooth
  SerialBT.begin("ESP32-WROOM-32");  // Nazwa Twojego urządzenia Bluetooth
}

void loop() {
  // Pobierz aktualne odczyty temperatury
  sensors.requestTemperatures(); 
  float temperature1 = sensors.getTempCByIndex(0); // pierwszy sensor
  float temperature2 = sensors.getTempCByIndex(1); // drugi sensor

  // Format danych do wysłania
  String dataToSend = String("Temp1:") + String(temperature1) + String(",Temp2:") + String(temperature2);
  SerialBT.println(dataToSend);  // Wyślij dane do urządzenia Android

  // Przygotuj wiadomości do wyświetlenia
  char temperatureDisplay1[20];
  char temperatureDisplay2[20];
  sprintf(temperatureDisplay1, "Temp1: %3.1f *C", temperature1);
  sprintf(temperatureDisplay2, "Temp2: %3.1f *C", temperature2);
  
  // Wyświetl wiadomości na wyświetlaczu OLED
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr); // wybierz czcionkę
    u8g2.drawStr(0,20,temperatureDisplay1);  // napisz tekst dla pierwszego sensora
    u8g2.drawStr(0,50,temperatureDisplay2);  // napisz tekst dla drugiego sensora
  } while ( u8g2.nextPage() );

  // Odczekaj chwilę przed kolejnym odczytem
  delay(1000);
}
