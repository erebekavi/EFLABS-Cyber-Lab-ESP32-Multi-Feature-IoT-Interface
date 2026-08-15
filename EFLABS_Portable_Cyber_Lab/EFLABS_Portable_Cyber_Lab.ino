/*
  EFLABS PORTABLE CYBER LAB
  ESP32 DevKit V1 - No nRF24L01

  Buttons (INPUT_PULLUP):
    UP     GPIO26 -> GND
    DOWN   GPIO33 -> GND
    SELECT GPIO32 -> GND
    BACK   GPIO25 -> GND
    EXTRA  GPIO27 -> GND

  OLED SSD1306 128x64 I2C:
    SDA GPIO21
    SCL GPIO22
    VCC 3.3V
    GND GND

  Optional NeoPixel:
    DIN GPIO14

  Libraries:
    U8g2
    Adafruit NeoPixel

  Features:
    WiFi Scanner
    2.4 GHz Channel Analyzer
    BLE Scanner
    WiFi Info
    Signal Monitor
    Device Info
    Settings
    About
*/

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>

#define BTN_UP       26
#define BTN_DOWN     33
#define BTN_SELECT   32
#define BTN_BACK     25
#define BTN_EXTRA    27

#define OLED_SDA     21
#define OLED_SCL     22
#define LED_PIN      14

U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(
  U8G2_R0, U8X8_PIN_NONE
);

Adafruit_NeoPixel pixel(
  1, LED_PIN, NEO_GRB + NEO_KHZ800
);

const char* menuItems[] = {
  "WiFi Scanner",
  "Channel Analyzer",
  "BLE Scanner",
  "WiFi Info",
  "Signal Monitor",
  "Device Info",
  "Settings",
  "About"
};

const uint8_t MENU_COUNT =
  sizeof(menuItems) / sizeof(menuItems[0]);

int menuIndex = 0;

bool buttonPressed(uint8_t pin) {
  static bool previous[40] = {};
  static unsigned long lastPress[40] = {};

  bool state = digitalRead(pin) == LOW;
  unsigned long now = millis();

  if (state && !previous[pin] &&
      now - lastPress[pin] > 180) {
    previous[pin] = true;
    lastPress[pin] = now;
    return true;
  }

  if (!state)
    previous[pin] = false;

  return false;
}

void header(const char* title) {
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 9, title);
  oled.drawHLine(0, 11, 128);
}

void footer(const char* text) {
  oled.setFont(u8g2_font_5x7_tf);
  oled.drawStr(0, 63, text);
  oled.sendBuffer();
}

void waitBack() {
  while (!buttonPressed(BTN_BACK) &&
         !buttonPressed(BTN_SELECT)) {
    delay(10);
  }
}

void drawMenu() {
  oled.clearBuffer();

  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 9, "EFLABS CYBER LAB");
  oled.drawHLine(0, 11, 128);

  int first = menuIndex;
  if (first > MENU_COUNT - 5)
    first = MENU_COUNT - 5;
  if (first < 0)
    first = 0;

  for (int i = 0;
       i < 5 && first + i < MENU_COUNT;
       i++) {
    int y = 21 + i * 9;

    if (first + i == menuIndex) {
      oled.drawBox(0, y - 7, 128, 9);
      oled.setDrawColor(0);
      oled.drawStr(3, y, menuItems[first + i]);
      oled.setDrawColor(1);
    } else {
      oled.drawStr(3, y, menuItems[first + i]);
    }
  }

  oled.setFont(u8g2_font_5x7_tf);
  oled.drawStr(0, 63, "UP DN SEL BACK EXTRA");
  oled.sendBuffer();
}

void wifiScanner() {
  header("WIFI SCANNER");
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 27, "Scanning...");
  oled.sendBuffer();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks(false, true);

  if (n <= 0) {
    header("WIFI SCANNER");
    oled.drawStr(0, 27, "No networks found");
    footer("BACK = RETURN");
    waitBack();
    WiFi.scanDelete();
    return;
  }

  const int perPage = 4;
  int page = 0;
  int pages = (n + perPage - 1) / perPage;

  while (true) {
    header("WIFI SCANNER");
    oled.setFont(u8g2_font_5x7_tf);

    int start = page * perPage;

    for (int i = 0;
         i < perPage && start + i < n;
         i++) {
      int idx = start + i;
      String ssid = WiFi.SSID(idx);

      if (ssid.length() == 0)
        ssid = "<hidden>";
      if (ssid.length() > 10)
        ssid = ssid.substring(0, 10);

      char line[40];
      snprintf(line, sizeof(line),
               "%s %ddBm C%d",
               ssid.c_str(),
               WiFi.RSSI(idx),
               WiFi.channel(idx));

      oled.drawStr(0, 21 + i * 9, line);
    }

    char f[32];
    snprintf(f, sizeof(f),
             "%d/%d EXTRA=REFRESH",
             page + 1, pages);

    oled.drawStr(0, 62, f);
    oled.sendBuffer();

    if (buttonPressed(BTN_UP) && page > 0)
      page--;

    if (buttonPressed(BTN_DOWN) && page < pages - 1)
      page++;

    if (buttonPressed(BTN_EXTRA)) {
      WiFi.scanDelete();
      n = WiFi.scanNetworks(false, true);
      page = 0;
      pages = max(1, (n + perPage - 1) / perPage);
    }

    if (buttonPressed(BTN_BACK) ||
        buttonPressed(BTN_SELECT))
      break;

    delay(10);
  }

  WiFi.scanDelete();
}

void channelAnalyzer() {
  header("CHANNEL ANALYZER");
  oled.setFont(u8g2_font_5x7_tf);
  oled.drawStr(0, 23, "Scanning 2.4GHz...");
  oled.sendBuffer();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks(false, true);
  int channelCount[14] = {};

  for (int i = 0; i < n; i++) {
    int ch = WiFi.channel(i);
    if (ch >= 1 && ch <= 13)
      channelCount[ch]++;
  }

  int best = 1;
  for (int ch = 1; ch <= 13; ch++) {
    if (channelCount[ch] < channelCount[best])
      best = ch;
  }

  int maxCount = 1;
  for (int ch = 1; ch <= 13; ch++)
    maxCount = max(maxCount, channelCount[ch]);

  while (true) {
    header("CHANNEL ANALYZER");
    oled.setFont(u8g2_font_5x7_tf);

    char info[32];
    snprintf(info, sizeof(info),
             "Least busy: CH %d", best);
    oled.drawStr(0, 20, info);

    for (int ch = 1; ch <= 13; ch++) {
      int x = (ch - 1) * 9 + 1;
      int h = (channelCount[ch] * 28) / maxCount;
      if (h > 0)
        oled.drawBox(x, 49 - h, 6, h);
    }

    oled.drawStr(0, 63, "BACK = RETURN");
    oled.sendBuffer();

    if (buttonPressed(BTN_BACK) ||
        buttonPressed(BTN_SELECT))
      break;

    delay(10);
  }

  WiFi.scanDelete();
}

void bleScanner() {
  header("BLE SCANNER");
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 27, "Scanning 5 seconds...");
  oled.sendBuffer();

  BLEDevice::init("");
  BLEScan* scanner = BLEDevice::getScan();
  scanner->setActiveScan(true);

  // Fix for current ESP32 BLE API:
  // start() returns BLEScanResults*
  BLEScanResults* results = scanner->start(5, false);

  int deviceCount = results->getCount();
  const int perPage = 4;
  int page = 0;
  int pages = max(1, (deviceCount + perPage - 1) / perPage);

  while (true) {
    header("BLE SCANNER");
    oled.setFont(u8g2_font_5x7_tf);

    int start = page * perPage;

    for (int i = 0;
         i < perPage && start + i < deviceCount;
         i++) {
      BLEAdvertisedDevice device =
        results->getDevice(start + i);

      String name =
        device.haveName()
          ? device.getName().c_str()
          : "<unnamed>";

      if (name.length() > 10)
        name = name.substring(0, 10);

      char line[32];
      snprintf(line, sizeof(line),
               "%s %ddBm",
               name.c_str(),
               device.getRSSI());

      oled.drawStr(0, 21 + i * 9, line);
    }

    char f[32];
    snprintf(f, sizeof(f),
             "%d DEV %d/%d",
             deviceCount, page + 1, pages);

    oled.drawStr(0, 62, f);
    oled.sendBuffer();

    if (buttonPressed(BTN_UP) && page > 0)
      page--;

    if (buttonPressed(BTN_DOWN) && page < pages - 1)
      page++;

    if (buttonPressed(BTN_BACK) ||
        buttonPressed(BTN_SELECT))
      break;

    delay(10);
  }

  scanner->clearResults();
}

void wifiInfo() {
  while (true) {
    header("WIFI INFORMATION");
    oled.setFont(u8g2_font_5x7_tf);

    oled.drawStr(
      0, 21,
      WiFi.isConnected() ? "Connected" : "Not connected"
    );

    String mac = "MAC: " + WiFi.macAddress();
    oled.drawStr(0, 31, mac.c_str());

    if (WiFi.isConnected()) {
      String ip = "IP: " + WiFi.localIP().toString();
      String rssi = "RSSI: " + String(WiFi.RSSI()) + " dBm";
      oled.drawStr(0, 41, ip.c_str());
      oled.drawStr(0, 51, rssi.c_str());
    } else {
      oled.drawStr(0, 41, "Use WiFi Scanner");
      oled.drawStr(0, 51, "for nearby networks");
    }

    oled.drawStr(0, 63, "BACK = RETURN");
    oled.sendBuffer();

    if (buttonPressed(BTN_BACK) ||
        buttonPressed(BTN_SELECT))
      break;

    delay(10);
  }
}

void signalMonitor() {
  while (true) {
    header("SIGNAL MONITOR");
    oled.setFont(u8g2_font_6x10_tf);

    if (WiFi.isConnected()) {
      String ssid = WiFi.SSID();
      if (ssid.length() > 20)
        ssid = ssid.substring(0, 20);

      oled.drawStr(0, 23, ssid.c_str());

      int rssi = WiFi.RSSI();
      char line[32];
      snprintf(line, sizeof(line),
               "RSSI: %d dBm", rssi);
      oled.drawStr(0, 37, line);

      int bar = constrain(map(rssi, -100, -30, 0, 96), 0, 96);

      oled.drawFrame(0, 46, 100, 10);
      if (bar > 0)
        oled.drawBox(2, 48, bar, 6);
    } else {
      oled.drawStr(0, 30, "WiFi not connected");
    }

    oled.setFont(u8g2_font_5x7_tf);
    oled.drawStr(0, 63, "BACK = RETURN");
    oled.sendBuffer();

    if (buttonPressed(BTN_BACK) ||
        buttonPressed(BTN_SELECT))
      break;

    delay(1000);
  }
}

void deviceInfo() {
  while (true) {
    header("DEVICE INFORMATION");
    oled.setFont(u8g2_font_5x7_tf);

    char line[40];

    snprintf(line, sizeof(line),
             "Chip: ESP32");
    oled.drawStr(0, 21, line);

    snprintf(line, sizeof(line),
             "CPU: %d MHz",
             ESP.getCpuFreqMHz());
    oled.drawStr(0, 30, line);

    snprintf(line, sizeof(line),
             "Free heap: %lu",
             (unsigned long)ESP.getFreeHeap());
    oled.drawStr(0, 39, line);

    String mac = "MAC: " + WiFi.macAddress();
    oled.drawStr(0, 48, mac.c_str());

    oled.drawStr(0, 57, "nRF24: NOT USED");
    oled.drawStr(0, 63, "BACK = RETURN");
    oled.sendBuffer();

    if (buttonPressed(BTN_BACK) ||
        buttonPressed(BTN_SELECT))
      break;

    delay(10);
  }
}

void settings() {
  int option = 0;

  while (true) {
    header("SETTINGS");
    oled.setFont(u8g2_font_6x10_tf);

    const char* options[] = {"LED ON", "LED OFF"};

    for (int i = 0; i < 2; i++) {
      int y = 27 + i * 14;

      if (i == option) {
        oled.drawBox(0, y - 9, 128, 11);
        oled.setDrawColor(0);
        oled.drawStr(3, y, options[i]);
        oled.setDrawColor(1);
      } else {
        oled.drawStr(3, y, options[i]);
      }
    }

    oled.setFont(u8g2_font_5x7_tf);
    oled.drawStr(0, 63, "SEL=SET BACK=EXIT");
    oled.sendBuffer();

    if (buttonPressed(BTN_UP))
      option = 0;

    if (buttonPressed(BTN_DOWN))
      option = 1;

    if (buttonPressed(BTN_SELECT)) {
      if (option == 0)
        pixel.setPixelColor(0, pixel.Color(0, 30, 0));
      else
        pixel.clear();

      pixel.show();
    }

    if (buttonPressed(BTN_BACK) ||
        buttonPressed(BTN_EXTRA))
      break;

    delay(10);
  }
}

void about() {
  header("ABOUT");
  oled.setFont(u8g2_font_5x7_tf);

  oled.drawStr(0, 22, "EFLABS Portable Cyber Lab");
  oled.drawStr(0, 32, "ESP32 WiFi + BLE toolkit");
  oled.drawStr(0, 42, "5-button interface");
  oled.drawStr(0, 52, "No nRF24L01 required");
  oled.drawStr(0, 63, "BACK = RETURN");
  oled.sendBuffer();

  waitBack();
}

void runSelected() {
  switch (menuIndex) {
    case 0: wifiScanner();      break;
    case 1: channelAnalyzer();  break;
    case 2: bleScanner();       break;
    case 3: wifiInfo();         break;
    case 4: signalMonitor();    break;
    case 5: deviceInfo();       break;
    case 6: settings();         break;
    case 7: about();            break;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(BTN_EXTRA, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  oled.begin();
  oled.setContrast(255);

  pixel.begin();
  pixel.clear();
  pixel.show();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(18, 24, "EFLABS CYBER LAB");
  oled.drawStr(34, 40, "Starting...");
  oled.drawStr(45, 56, "ESP32");
  oled.sendBuffer();

  pixel.setPixelColor(0, pixel.Color(0, 0, 30));
  pixel.show();
  delay(1500);
  pixel.clear();
  pixel.show();

  drawMenu();
}

void loop() {
  if (buttonPressed(BTN_UP)) {
    menuIndex--;
    if (menuIndex < 0)
      menuIndex = MENU_COUNT - 1;
    drawMenu();
  }

  if (buttonPressed(BTN_DOWN)) {
    menuIndex++;
    if (menuIndex >= MENU_COUNT)
      menuIndex = 0;
    drawMenu();
  }

  if (buttonPressed(BTN_SELECT)) {
    runSelected();
    drawMenu();
  }

  if (buttonPressed(BTN_BACK)) {
    menuIndex = 0;
    drawMenu();
  }

  if (buttonPressed(BTN_EXTRA)) {
    drawMenu();
  }

  delay(10);
}
