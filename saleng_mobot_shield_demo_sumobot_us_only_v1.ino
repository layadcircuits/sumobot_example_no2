/***************************************************************************
 Example Code of a  sumobot with 2 line trackers and an ultrasonic front obstacle sensors
 
 This software is free provided that this notice is not removed and proper attribution 
 is accorded to Layad Circuits and its Author(s).
 Layad Circuits invests resources in producing free software. By purchasing Layad Circuits'
 products or utilizing its services, you support the continuing development of free software
 for all.
  
 Author(s): C.D.Malecdan for Layad Circuits Electronics Engineering
 Revision: 1.0 - 2019/03/01 - initial creation
 Layad Circuits Electronics Engineering Supplies and Services
 B314 Lopez Bldg., Session Rd. cor. Assumption Rd., Baguio City, Philippines
 www.layadcircuits.com
 general: info@layadcircuits.com
 sales: sales@layadcircuits.com
 +63-916-442-8565
 * NOTES:
 * Install right motor(s) on AIN1 & AIN2
 * Install left motor(s) on BIN1 & BIN2
 * When the Single Supply microjumper is installed, install just ONE power source for 
 * both Arduino (6-12V) and Motors(voltage dependes on motors). This single power source must be a voltage accepted by both Arduino and motors
 ***************************************************************************/


// change below to define if line 
// is black on white background or
// white on black background.
// These define the sensor value when
// under black and white surface
// You may also use the DO and ~DO
// pins of the Saleng Tracker to 
// change line configuration

#define BLK LOW  // line
#define WHT HIGH // background

// speed setting based on movement
// 255=full speed
#define SPEED_ON_TURNS 80
#define SPEED_FORWARD 255
#define SPEED_BACKWARD 255

#define AIN1 2 
#define BIN1 7
#define AIN2 4
#define BIN2 5
#define PWMA 3
#define PWMB 6
#define STBY 8

#define PIN_IR_DIST  A0
#define US_TRIG 12
#define US_ECHO A3
#define FRONT_LINE_SENSOR 10
#define REAR_LINE_SENSOR 11

unsigned long us_dist, ir_dist;

void speedSetting(byte val)
{
  analogWrite(PWMA,val);
  analogWrite(PWMB,val);
}

void forward()
{
  speedSetting(SPEED_FORWARD);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void backward()
{
  speedSetting(SPEED_BACKWARD);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void turnleft()
{
  speedSetting(SPEED_ON_TURNS);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void turnright()
{
  speedSetting(SPEED_ON_TURNS);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void motorstop()
{
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,LOW);
}

void shortbreak()
{
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,HIGH);
}


int ir_dist_sensor()
{
  return analogRead(PIN_IR_DIST);
}


unsigned long us_sensor()
{
  static unsigned long t;
  if(millis() - t < 50) return;
  t = millis();
  
  unsigned long d;
  float f;
  digitalWrite(US_TRIG,LOW);
  delayMicroseconds(20);
  digitalWrite(US_TRIG,HIGH); 
  delayMicroseconds(100);
  digitalWrite(US_TRIG,LOW); 
  d = pulseIn(US_ECHO,HIGH, 50000);
  f = ( (float)d / 29.4 ) / 2;
  d = (unsigned long)f;
  if( d ==0 ) d = 200;
  
  return d;
}

// generate a number 0-1
byte generate_random()
{
  randomSeed(micros()%100);
  return   random(0,1);
}



void setup() {
  pinMode(US_TRIG,OUTPUT);
  pinMode(US_ECHO,INPUT);
  pinMode(FRONT_LINE_SENSOR, INPUT);
  pinMode(REAR_LINE_SENSOR, INPUT);

  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);
  digitalWrite(STBY,HIGH);//enable driver
  
  // we use a low power to avoid
  // overshooting lines
  // and conserve battery
  // full POWER = 255
  speedSetting(200); 
  delay(1000); // add 1s delay 
  Serial.begin(115200);
  
}


void loop() {

  if(digitalRead(FRONT_LINE_SENSOR) == HIGH) // front sensor was tripped
  {
      // move back
      backward();
      delay(500);
      if(generate_random()) turnright();
      else turnleft();
      delay(500);
  }
  else if(digitalRead(REAR_LINE_SENSOR) == HIGH)// rear sensor was tripped
  {
    // move forward
    forward();
    delay(500);
    if(generate_random()) turnright();
    else turnleft();
    delay(500);
  }
  else // no line sensors were touched so do scan algorithm
  {
    ir_dist = ir_dist_sensor();
    us_dist = us_sensor();
    
    
    if(us_dist < 50 )
    {
      forward(); 
    }
    else
    {
      turnleft();
      us_dist = us_sensor();
    }
  }
} 
