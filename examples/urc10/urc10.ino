#include <urc10.h>

int direction = MOTOR1_LEFT_MOTOR2_RIGHT;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Testing URC10");

  URC10.begin(); // by default only motor is enabled
  // URC10.begin(true, true, true, true) - URC10.begin(motor_enable, ultrasonic1_enable, ultrasonic2_enable, lcd_enable);
  
  // set motor direction
  URC10.motor.setDirection(direction);

  // enable ultrasonic 1 manually
  URC10.enableUltrasonic1();
  
  // enable ultrasonic 2 manually
  URC10.enableUltrasonic2();
  
  // enable lcd manually (don't enable it if there is no I2C device attached
  // will cause hang on Arduino
  URC10.enableLcd();
  URC10.lcd.setCursor(0,0);
  URC10.lcd.println("Testing URC10");
  delay(1000);
}

void loop() {

  Serial.println("Running loop...");
  // display ultrasonic reading on lcd
  URC10.lcd.clear();
  URC10.lcd.setCursor(0, 1);
  URC10.lcd.println("Read Ultrasonic S1:");
  URC10.lcd.print(URC10.ultrasonic1.getReadingInCm());
  URC10.lcd.print(" cm");

  // test run motor
  testMotorRun();
  
}

void testMotorRun(){
  URC10.motor.forward(50, 50);
  delay(2000);
  URC10.motor.reverse(50, 50);
  delay(2000);
  URC10.motor.rotateToLeft(50, 50);
  delay(2000);
  URC10.motor.rotateToRight(50, 50);
  delay(2000);
  URC10.motor.turnLeft(50);
  delay(2000);
  URC10.motor.turnRight(50);
  delay(2000);
  URC10.motor.stop();

  // change direction in every cycle of run
  // from MOTOR1_LEFT_MOTOR2_RIGHT will change to MOTOR1_RIGHT_MOTOR2_LEFT
  // from MOTOR1_RIGHT_MOTOR2_LEFT will change to MOTOR1_LEFT_MOTOR2_RIGHT
  direction = ~direction;
  URC10.motor.setDirection(direction);
  
  delay(2000);
  
}


