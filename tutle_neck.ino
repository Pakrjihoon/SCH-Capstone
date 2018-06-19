#include <SoftwareSerial.h>   // 시리얼통신 라이브러리 호출
#define FLEX_PIN1 A5
#define FLEX_PIN2 A1
#define FLEX_PIN3 A2
#define BLUE_TX 2
#define BLUE_RX 3
const int fslpSenseLine = A3;
const int fslpDriveLine1 = 8;
const int fslpDriveLine2 = A4;
const int fslpBotR0 = 9;

SoftwareSerial mySerial(BLUE_TX, BLUE_RX);  //시리얼 통신을 위한 객체선언

void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);
  //delay(1000);
}
char data[13] = {0,};
//flex값 3바이트 필요
void loop()
{
  int pressure, position;
  pressure = fslpGetPressure();
  int flexVal = analogRead(FLEX_PIN1);
  int flexVal2 = analogRead(FLEX_PIN2);
  int flexVal3 = analogRead(FLEX_PIN3);
  if (pressure == 0)
  {
    position = 0;
  }
  else
  {
    position = fslpGetPosition();  // Raw reading, from 0 to 1023.
  }

  sprintf(data,"%03d%03d%03d%03d",flexVal,flexVal2,flexVal3,pressure);

  Serial.println(data);
  mySerial.print(data);
  delay(1500);
}

int fslpGetPosition()
{
  pinMode(fslpSenseLine, OUTPUT);
  digitalWrite(fslpSenseLine, LOW);

  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, LOW);

  pinMode(fslpDriveLine2, OUTPUT);
  digitalWrite(fslpDriveLine2, LOW);

  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);

  digitalWrite(fslpDriveLine1, HIGH);
  pinMode(fslpBotR0, INPUT);
  pinMode(fslpSenseLine, INPUT);

  delayMicroseconds(10);

  analogReset();
  return analogRead(fslpSenseLine);
}

int fslpGetPressure()
{
  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, HIGH);
  
  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);

  pinMode(fslpSenseLine, INPUT);

  pinMode(fslpDriveLine2, INPUT);

  delayMicroseconds(10);

  analogReset();
  int v1 = analogRead(fslpDriveLine2);
  analogReset();
  int v2 = analogRead(fslpSenseLine);

  if (v1 == v2)
  {
    return 32 * 1023;
  }
  return 32 * v2 / (v1 - v2);
}

void analogReset()
{
#if defined(ADMUX)
#if defined(ADCSRB) && defined(MUX5)
    ADCSRB |= (1 << MUX5);
#endif
    ADMUX = 0x1F;
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
#endif
}
