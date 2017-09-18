/******************************************************************************
urc10.h
Cytron URC10 Arduino Library Main Header File
Created by Ng Beng Chet @ Cytron Technologies Sdn Bhd 
Original Creation Date: Sept 18, 2017
https://www.cytron.com.my/c-86-robot-controller/c-95-autonomous-robot-control-board/p-urc10

!!! Description Here !!!

Development environment specifics:
	IDE: Arduino 1.8.1
	Hardware Platform: Arduino Uno
	Cytron UNO Robot Controller Version: 1.0.0

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef _CYTRON_URC10_H_
#define _CYTRON_URC10_H_

#include <Arduino.h>

// i2c lcd
#include "SSD1306Ascii/SSD1306Ascii.h"
#include "SSD1306Ascii/SSD1306AsciiAvrI2c.h"

// i2c general
#define NORMAL 0
#define INVERTED 1
#define SSD1306_I2C_ADDRESS 0x3C

// lcd type
#define SSD1306_128_64
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64

// scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

// motor
#define MOTOR1_LEFT_MOTOR2_RIGHT 0
#define MOTOR1_RIGHT_MOTOR2_LEFT 1

// urc10 general
// ultrasonic
#define URC10_ULTRASONIC1_ECHO 8
#define URC10_ULTRASONIC1_TRIG 9
#define URC10_ULTRASONIC2_ECHO 10
#define URC10_ULTRASONIC2_TRIG 11
// motor
#define URC10_MOTOR_1_DIR 4
#define URC10_MOTOR_1_PWM 5
#define URC10_MOTOR_2_DIR 7
#define URC10_MOTOR_2_PWM 6

#define URC10_MOTOR_1_CW 0
#define URC10_MOTOR_1_CCW 1
#define URC10_MOTOR_2_CW 0
#define URC10_MOTOR_2_CCW 1

class urc10_motor
{
  public:
	urc10_motor()
	{
		motor1_dir_pin = URC10_MOTOR_1_DIR;
		motor1_pwm_pin = URC10_MOTOR_1_PWM;
		motor2_dir_pin = URC10_MOTOR_2_DIR;
		motor2_pwm_pin = URC10_MOTOR_2_PWM;
	}
	void begin(bool _inverse = MOTOR1_LEFT_MOTOR2_RIGHT)
	{
		// setup pins and inverse
		inverse = _inverse;

		// set dir as output as output
		pinMode(motor1_dir_pin, OUTPUT);
		pinMode(motor2_dir_pin, OUTPUT);
	}

	void setDirection(bool _inverse){
		inverse = _inverse;
	}

	// manual control
	void run(uint8_t dir1, uint8_t spd1, uint8_t dir2, uint8_t spd2)
	{
		// set dir as output as output
		pinMode(motor1_dir_pin, OUTPUT);
		pinMode(motor2_dir_pin, OUTPUT);

		// motor 1
		digitalWrite(motor1_dir_pin, dir1);
		analogWrite(motor1_pwm_pin, spd1);
		// motor 2
		digitalWrite(motor2_dir_pin, dir2);
		analogWrite(motor2_pwm_pin, spd2);
	}
	void turnLeft(uint8_t _rightspd, uint8_t _leftspd = 0)
	{
		if (inverse)
			run(URC10_MOTOR_1_CW, _rightspd, URC10_MOTOR_2_CCW, _leftspd); // left: motor2, right: motor1
		else
			run(URC10_MOTOR_1_CCW, _leftspd, URC10_MOTOR_2_CW, _rightspd); // left: motor1, right: motor2
	}
	void turnRight(uint8_t _leftspd, uint8_t _rightspd = 0)
	{
		if (inverse)
			run(URC10_MOTOR_1_CW, _rightspd, URC10_MOTOR_2_CCW, _leftspd); // left: motor2, right: motor1
		else
			run(URC10_MOTOR_1_CCW, _leftspd, URC10_MOTOR_2_CW, _rightspd); // left: motor1, right: motor2
	}
	void forward(uint8_t _leftspd, uint8_t _rightspd)
	{
		if (inverse)
			run(URC10_MOTOR_1_CW, _rightspd, URC10_MOTOR_2_CCW, _leftspd); // left: motor2, right: motor1
		else
			run(URC10_MOTOR_1_CCW, _leftspd,  URC10_MOTOR_2_CW, _rightspd); // left: motor1, right: motor2
	}
	void reverse(uint8_t _leftspd, uint8_t _rightspd)
	{
		if (inverse)
			run(URC10_MOTOR_1_CCW, _rightspd, URC10_MOTOR_2_CW, _leftspd); // left: motor2, right: motor1
		else
			run(URC10_MOTOR_1_CW, _leftspd, URC10_MOTOR_2_CCW, _rightspd); // left: motor1, right: motor2
	}
	void rotateToLeft(uint8_t _leftspd, uint8_t _rightspd)
	{
		if (inverse)
			run(URC10_MOTOR_1_CW, _rightspd, URC10_MOTOR_2_CW, _leftspd); // left: motor2, right: motor1
		else
			run(URC10_MOTOR_1_CW, _leftspd, URC10_MOTOR_2_CW, _rightspd); // left: motor1, right: motor2
	}
	void rotateToRight(uint8_t _leftspd, uint8_t _rightspd)
	{
		if (inverse)
			run(URC10_MOTOR_1_CCW, _rightspd, URC10_MOTOR_2_CCW, _leftspd); // left: motor2, right: motor1
		else
			run(URC10_MOTOR_1_CCW, _leftspd, URC10_MOTOR_2_CCW, _rightspd); // left: motor1, right: motor2
	}

	void stop(){
		if (inverse)
			run(URC10_MOTOR_1_CCW, 0, URC10_MOTOR_2_CW, 0); // left: motor2, right: motor1
		else
			run(URC10_MOTOR_1_CW, 0, URC10_MOTOR_2_CCW, 0); // left: motor1, right: motor2
	}

  private:
	uint8_t motor1_dir_pin;
	uint8_t motor1_pwm_pin;
	uint8_t motor2_dir_pin;
	uint8_t motor2_pwm_pin;

	int motor1_current_dir;
	int motor2_current_dir;

	// inverse
	// false -> left: motor 1, right: motor2
	// true -> left: motor 2, right: motor 1
	bool inverse = false;
};

class urc10_ultrasonic
{
  public:
	urc10_ultrasonic(){}
	void begin(uint8_t _echo_pin, uint8_t _trig_pin)
	{
		echo_pin = _echo_pin;
		trig_pin = _trig_pin;
		pinMode(trig_pin, OUTPUT);
		pinMode(echo_pin, INPUT);
	}
	long raw()
	{
		digitalWrite(trig_pin, LOW);
		delayMicroseconds(2);
		digitalWrite(trig_pin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trig_pin, LOW);

		long duration = pulseIn(echo_pin, HIGH);
		return duration;
	}
	uint16_t getReadingInCm(){
		return uint16_t(raw()*0.034/2);
	}

	uint16_t getReadingInInch(){
		return uint16_t(raw()*0.0133/2);
	}

  private:
	uint8_t echo_pin;
	uint8_t trig_pin;
};

class urc10_lcd : public SSD1306Ascii
{
  public:
	urc10_lcd();
	void begin(const DevType *dev = &Adafruit128x64, uint8_t i2cAddr = SSD1306_I2C_ADDRESS, bool fastMode = true)
	{
		m_nData = 0;
		m_i2cAddr = i2cAddr;

		m_i2c.begin(fastMode);
		init(dev);

		//default font is System5x7
		setFont(System5x7);
		_enable = true;
	}
	void invertDisplay(uint8_t i);
	void startScrollRight(uint8_t start, uint8_t stop);
	void startScrollLeft(uint8_t start, uint8_t stop);
	void startScrollDiagRight(uint8_t start, uint8_t stop);
	void startScrollDiagLeft(uint8_t start, uint8_t stop);
	void stopScroll(void);
	void dim(boolean dim);
	//void drawPixel(int16_t x, int16_t y, uint16_t color);
	//void writeByte(const uint8_t byte, uint8_t color=NORMAL);
	void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h,
					uint8_t color);
	void drawBitmap(int16_t x, int16_t y, const char *bitmap, int16_t w, int16_t h)
	{
		drawBitmap(x, y, (const uint8_t *)bitmap, w, h, NORMAL);
	}
	bool isEnabled() { return _enable; }

  protected:
	void writeDisplay(uint8_t b, uint8_t mode)
	{
		if ((m_nData && mode == SSD1306_MODE_CMD))
		{
			m_i2c.stop();
			m_nData = 0;
		}
		if (m_nData == 0)
		{
			m_i2c.start((m_i2cAddr << 1) | I2C_WRITE);
			m_i2c.write(mode == SSD1306_MODE_CMD ? 0X00 : 0X40);
		}
		m_i2c.write(b);
		if (mode == SSD1306_MODE_RAM_BUF)
		{
			m_nData++;
		}
		else
		{
			m_i2c.stop();
			m_nData = 0;
		}
	}

  private:
	AvrI2c m_i2c;
	uint8_t m_i2cAddr;
	uint8_t m_nData;
	bool _enable = false;
};

class urc10
{
  public:
	urc10_lcd lcd;
	urc10_ultrasonic ultrasonic1;
	urc10_ultrasonic ultrasonic2;
	urc10_motor motor;
	urc10();
	void begin(bool enableMotor=true, bool enableUltrasonic1=false, bool enableUltrasonic2=false, bool enableLcd=false);
	void enableLcd();
	void enableUltrasonic1();
	void enableUltrasonic2();
	void enableMotor();
};

extern urc10 URC10;

#endif
