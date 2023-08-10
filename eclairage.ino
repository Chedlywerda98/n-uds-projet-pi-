#include <Wire.h>
#include <ds3231.h>
struct ts t;
#define LUMcap 2
#define MVcap 18
#define EXTlampe 22
#define SAlampe 23
#define GUIlampe 25
#define SRlampe 26
#define BClampe 27
#define Clampe 33
#define GClampe 32
float LUMval ;
void setup() {
  // put your setup code here, to run once:
    Wire.begin();
  DS3231_init(DS3231_INTCN);
pinMode(LUMcap,INPUT);
pinMode(MVcap,INPUT);
pinMode(EXTlampe,OUTPUT);
pinMode(SAlampe,OUTPUT);
pinMode(GUIlampe,OUTPUT);
pinMode(SRlampe,OUTPUT);
pinMode(BClampe,OUTPUT);
pinMode(GClampe,OUTPUT);
pinMode(Clampe,OUTPUT);
}

void loop() {
  
  DS3231_get(&t);
if((t.hour== 8)){
  digitalWrite(SAlampe,HIGH);
  digitalWrite(GUIlampe,HIGH);
}
  else if(t.hour == 18);{
  
  digitalWrite(SAlampe,LOW);
  digitalWrite(GUIlampe,LOW);
  }  
if(digitalRead(MVcap)==HIGH){
  digitalWrite(Clampe,HIGH);

  }
  if(digitalRead(MVcap)==LOW){
   digitalWrite(Clampe,LOW);;
  }

delay(200);

  LUMval = analogRead(LUMcap);
  if(LUMval<500){
    digitalWrite(EXTlampe,HIGH);
  }
  else
  { 
    digitalWrite(EXTlampe,LOW);
  }
  
}
