#include <Arduino_ESP32SPI.h>
#include <Arduino_GC9A01.h>
#include <Arduino_GFX.h>
#include <gfx_2d.h>
#include <gfx_util.h>
#include <math_osm.h>
#include <pngle.h>

#include "ArduinoGraphics2DCanvas.h"
#include "config.h"
#include "osw_hal.h"
#include "osw_pins.h"

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO, VSPI /* spi_num */);
Arduino_GC9A01 *tft = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
ArduinoGraphics2DCanvas *canvas = new ArduinoGraphics2DCanvas(DISP_W, DISP_H, tft);

void OswHal::setupDisplay(void) {
#ifdef ESP32
  ledcAttachPin(TFT_LED, 1);
  ledcSetup(1, 12000, 8);  // 12 kHz PWM, 8-bit resolution
#else
  pinMode(TFT_LED, OUTPUT);
#endif
  canvas->begin();
  this->displayOn();
}

Arduino_TFT *OswHal::getArduino_TFT(void) { return tft; }
ArduinoGraphics2DCanvas *OswHal::getCanvas(void) { return canvas; }
void OswHal::flushCanvas(void) { canvas->flush(); }

void OswHal::displayOff(void) {
  tft->displayOff();
}

void OswHal::displayOn(void) {
  tft->displayOn();
}

void OswHal::setBrightness(uint8_t b) {
  _brightness = b;
#ifdef ESP32
  ledcWrite(1, _brightness);
#else
  digitalWrite(TFT_LED, brightness);
#endif
}

void OswHal::increaseBrightness(uint8_t v) {
  if (_brightness > 255 - v) {
    _brightness = 255;
  } else {
    _brightness += v;
  }
};
void OswHal::decreaseBrightness(uint8_t v) {
  if (_brightness < v) {
    _brightness = 0;
  } else {
    _brightness -= v;
  }
};
