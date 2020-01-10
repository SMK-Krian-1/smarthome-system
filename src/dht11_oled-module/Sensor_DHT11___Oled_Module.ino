#include "U8glib.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

#include <DHT.h>
 DHT dht;

/*
  Bitmaps created with The Gimp (http://www.gimp.org) and exported as XBM files
  Add U8G_PROGMEM to the exported XBM data since The Gimp does not add this automatically.
  In case you forget you end up with a scrambled image.
*/

#define humid_width 35
#define humid_height 48
static unsigned char humid_bits[] U8G_PROGMEM = {
   0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x80,
   0x0f, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0xe0, 0x1d, 0x00,
   0x00, 0x00, 0xe0, 0x38, 0x00, 0x00, 0x00, 0xf0, 0x78, 0x00, 0x00, 0x00,
   0x78, 0xf0, 0x00, 0x00, 0x00, 0x38, 0xe0, 0x00, 0x00, 0x00, 0x1c, 0xe0,
   0x01, 0x00, 0x00, 0x1e, 0xc0, 0x03, 0x00, 0x00, 0x0f, 0x80, 0x03, 0x00,
   0x00, 0x07, 0x00, 0x07, 0x00, 0x80, 0x07, 0x00, 0x0f, 0x00, 0xc0, 0x03,
   0x00, 0x0e, 0x00, 0xc0, 0x01, 0x00, 0x1c, 0x00, 0xe0, 0x01, 0x00, 0x3c,
   0x00, 0xe0, 0x00, 0x00, 0x38, 0x00, 0x70, 0x00, 0x00, 0x78, 0x00, 0x78,
   0x00, 0x00, 0x70, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x38, 0x00, 0x00,
   0xe0, 0x00, 0x1c, 0x00, 0x00, 0xc0, 0x01, 0x1c, 0x00, 0x00, 0xc0, 0x01,
   0x0e, 0x00, 0x00, 0xc0, 0x03, 0x0e, 0x00, 0x00, 0x80, 0x03, 0x0e, 0x00,
   0x00, 0x80, 0x03, 0x07, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00,
   0x07, 0x07, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x07, 0x07,
   0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00,
   0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x07, 0x0e, 0x00, 0x00, 0x80, 0x03,
   0x0e, 0x00, 0x00, 0x80, 0x03, 0x1e, 0x00, 0x00, 0xc0, 0x03, 0x1c, 0x00,
   0x00, 0xc0, 0x01, 0x3c, 0x00, 0x00, 0xe0, 0x01, 0x78, 0x00, 0x00, 0xf0,
   0x00, 0xf0, 0x00, 0x00, 0x78, 0x00, 0xe0, 0x01, 0x00, 0x3c, 0x00, 0xc0,
   0x07, 0x00, 0x3f, 0x00, 0x80, 0x1f, 0xc0, 0x0f, 0x00, 0x00, 0xff, 0xff,
   0x07, 0x00, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00 };


#define temperature_width 18
#define temperature_height 47
static unsigned char temperature_bits[] U8G_PROGMEM = {
   0xc0, 0x0f, 0x00, 0xe0, 0x1f, 0x00, 0x70, 0x38, 0x00, 0x30, 0x30, 0x00,
   0x30, 0x30, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00,
   0x30, 0x30, 0x00, 0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00, 0x30, 0x30, 0x00,
   0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00,
   0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00, 0x30, 0x30, 0x00, 0xb0, 0x37, 0x00,
   0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00, 0x30, 0x30, 0x00, 0xb0, 0x37, 0x00,
   0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00,
   0xb0, 0x37, 0x00, 0xb0, 0x37, 0x00, 0x98, 0x67, 0x00, 0x8c, 0xc7, 0x00,
   0xc6, 0x8f, 0x01, 0xe2, 0x1f, 0x01, 0xf3, 0x3f, 0x03, 0xf3, 0x3f, 0x03,
   0xf3, 0x3f, 0x03, 0xf3, 0x3f, 0x03, 0xf3, 0x3f, 0x03, 0xf3, 0x3f, 0x03,
   0xf3, 0x3f, 0x03, 0xe6, 0x9f, 0x01, 0xc6, 0x8f, 0x01, 0x0c, 0xc0, 0x00,
   0x38, 0x70, 0x00, 0xf0, 0x3f, 0x00, 0xc0, 0x0f, 0x00 };


void drawTemp(void) {

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 10);
  u8g.print("TEMPERATURE  1/2");

  u8g.setFont(u8g_font_fub25);

  u8g.setPrintPos(10, 50);

  float temp = round(dht.getTemperature()*10)/10;

  Serial.println(temp);

  u8g.print(String(round(dht.getTemperature()*10)/10) + " C");

  // Celcius symbol
  u8g.drawCircle(50, 23, 2);

  // Thermometer icon
  u8g.drawXBMP(100, 16, temperature_width, temperature_height, temperature_bits);

}

void drawHumid(void) {


  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 10);
  u8g.print("HUMIDITY     2/2");

  u8g.setFont(u8g_font_fub25);
  u8g.setPrintPos(10, 50);
  u8g.print(String(int(dht.getHumidity())) + "%");

  // Humidity icon
  u8g.drawXBMP( 90, 16, humid_width, humid_height, humid_bits);

}


uint8_t draw_state = 0;


void draw() {

  switch(draw_state ) {
    case 0: drawTemp(); break;
    case 1: drawHumid(); break;
  }

}
void setup() {
  // put your setup code here, to run once:
  dht.setup(2); // data pin 2

}

void loop() {
  // put your main code here, to run repeatedly:
  {// picture loop
  delay(dht.getMinimumSamplingPeriod());

  u8g.firstPage();
  do {
    draw();

  } while( u8g.nextPage() );

   // increase the state
  draw_state++;

  // Used if instead of mod operator to avoid running out of int size
  if ( draw_state > 1  )
    draw_state = 0;

  // Switch draw_state after delay
  delay(1500);}

}