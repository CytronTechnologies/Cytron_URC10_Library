
#include "urc10.h"

// ****************************************
// Main
// ****************************************
urc10 URC10;
urc10::urc10(){
}

void urc10::begin(bool enableMotor, bool enableUltrasonic1, bool enableUltrasonic2, bool enableLcd){
  if(enableMotor){
    motor.begin();
  }
  if(enableUltrasonic1){
    ultrasonic1.begin(URC10_ULTRASONIC1_ECHO, URC10_ULTRASONIC1_TRIG);
  }
  if(enableUltrasonic2){
    ultrasonic2.begin(URC10_ULTRASONIC2_ECHO, URC10_ULTRASONIC2_TRIG);
  }
  if(enableLcd && !lcd.isEnabled()){
    lcd.begin();
  }
}

void urc10::enableLcd(){
  if(!lcd.isEnabled())
    lcd.begin();
}

void urc10::enableUltrasonic1(){
  ultrasonic1.begin(URC10_ULTRASONIC1_ECHO, URC10_ULTRASONIC1_TRIG);
}

void urc10::enableUltrasonic2(){
  ultrasonic2.begin(URC10_ULTRASONIC2_ECHO, URC10_ULTRASONIC2_TRIG);
}

void urc10::enableMotor(){
  motor.begin();
}

// ****************************************
// lcd (SSD1306) extension
// ****************************************
urc10_lcd::urc10_lcd():SSD1306Ascii(){
}

void urc10_lcd::invertDisplay(uint8_t i) {
  if (i) {
    ssd1306WriteCmd(SSD1306_INVERTDISPLAY);
  } else {
    ssd1306WriteCmd(SSD1306_NORMALDISPLAY);
  }
}

// startscrollright
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void urc10_lcd::startScrollRight(uint8_t start, uint8_t stop){
  ssd1306WriteCmd(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(start);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(stop);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(0XFF);
  ssd1306WriteCmd(SSD1306_ACTIVATE_SCROLL);
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void urc10_lcd::startScrollLeft(uint8_t start, uint8_t stop){
  ssd1306WriteCmd(SSD1306_LEFT_HORIZONTAL_SCROLL);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(start);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(stop);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(0XFF);
  ssd1306WriteCmd(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void urc10_lcd::startScrollDiagRight(uint8_t start, uint8_t stop){
  ssd1306WriteCmd(SSD1306_SET_VERTICAL_SCROLL_AREA);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(SSD1306_LCDHEIGHT);
  ssd1306WriteCmd(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(start);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(stop);
  ssd1306WriteCmd(0X01);
  ssd1306WriteCmd(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void urc10_lcd::startScrollDiagLeft(uint8_t start, uint8_t stop){
  ssd1306WriteCmd(SSD1306_SET_VERTICAL_SCROLL_AREA);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(SSD1306_LCDHEIGHT);
  ssd1306WriteCmd(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(start);
  ssd1306WriteCmd(0X00);
  ssd1306WriteCmd(stop);
  ssd1306WriteCmd(0X01);
  ssd1306WriteCmd(SSD1306_ACTIVATE_SCROLL);
}

void urc10_lcd::stopScroll(void){
  ssd1306WriteCmd(SSD1306_DEACTIVATE_SCROLL);
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void urc10_lcd::dim(boolean dim) {
  uint8_t contrast;

  if (dim) {
    contrast = 0; // Dimmed display
  } else {
    contrast = 0xCF;
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  ssd1306WriteCmd(SSD1306_SETCONTRAST);
  ssd1306WriteCmd(contrast);
}

void urc10_lcd::drawBitmap(int16_t x, int16_t y,
 const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color) {

  int16_t i, j;
  int16_t y_start = y >> 3; //divided by 8
  uint8_t row; 

  if (h%8==0) {
    row=h/8;//row from 0 to 7 
  }
  
  else{
    row=h/8+1;//Quotient+1
  }

  for(j = 0 ; j < row; j++){

    setCursor(x, y_start);
    
    for(i = 0; i < w; i++) {  
      
      ssd1306WriteRamBuf(color==NORMAL?(pgm_read_byte(bitmap + i + j * w)):~(pgm_read_byte(bitmap + i + j * w)));

      if((w*h/8 - 1) == (i + j * w)) return; //if pixels are enough

    }

    y_start++;

  }
  setCursor(x, y_start);
}
