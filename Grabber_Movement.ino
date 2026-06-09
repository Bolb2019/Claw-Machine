#include <Servo.h>

//----------Claw servo-----------
Servo claw_servo;
Servo spool_servo;
Servo y_servo;
Servo xl_servo;
Servo xr_servo;

bool clawInactive = true;
int pos = -20;

//----------Joystick-----------
const int xPin = A4;
const int yPin = A3;
const int butt = A2;

int xCorrection = 130;
int yCorrection = 128;

//----------End declarations-----------
void setup() {
 Serial.begin(9600);
 claw_servo.attach(A9);
 y_servo.attach(A8);
 xl_servo.attach(A7);
 xr_servo.attach(A6);
 spool_servo.attach(A5);

 pinMode(butt, INPUT_PULLUP);

 claw_servo.write(pos);
 y_servo.write(90);
 xr_servo.write(90);
 xl_servo.write(90);
 spool_servo.write(90);
}

//----------Joystick-----------
struct button {
 byte pressed = 0;
};
 
struct analog {
 short x, y;
 button button;
};

//----------Main Loop-----------
void loop() {
 joystick_controls();
}

//----------Claw servo-----------
void claw_servo_open() {
 for (int pos = -20; pos <= 70; pos += 3) {
   claw_servo.write(pos);
   delay(30);
 }
}

void claw_servo_close() {
 for (int pos = 70; pos >= -20; pos -= 3) {
   claw_servo.write(pos);
   delay(30);
 }
}

//----------Joystick-----------
void joystick_controls() {
 analog analog;

 analog.x = readAnalogAxisLevel(xPin) - xCorrection;
 analog.y = readAnalogAxisLevel(yPin) - yCorrection;


 if (analog.x >= 50){
   xl_servo.write(0);
   xr_servo.write(180);
 }
 else if (analog.x <= -50){
   xl_servo.write(180);
   xr_servo.write(0);
 }else{
   xl_servo.write(90);
   xr_servo.write(90);
 }


 if (analog.y >= 50){
   y_servo.write(0);
 }
 else if (analog.y <= -50){
   y_servo.write(180);
 }else{
   y_servo.write(90);
 }


 analog.button.pressed = isAnalogButtonPressed(butt);

 if (analog.button.pressed && clawInactive) {
   clawInactive = false;
   spool_servo.write(0);
   delay(5000);
   spool_servo.write(90);


   claw_servo_open();
   delay(500);
   claw_servo_close();


   spool_servo.write(180);
   delay(5000);
   spool_servo.write(90);
 }
 if (!analog.button.pressed) {
     clawInactive = true;
 }
}

byte readAnalogAxisLevel(int pin) {
 return map(analogRead(pin), 0, 1023, 0, 255);
}
 
bool isAnalogButtonPressed(int pin) {
 return digitalRead(pin) == 0;
}