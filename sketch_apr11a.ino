long duration;
int distance;
const int pump = 2;
const int button = 3;
const int trigPin = 4;
const int echoPin = 5;
const int peltier1 = 6;
const int peltier2 = 7;
const int LED = 8;

float RT, VR, ln, TXH1, T0, VRT, TXH2, TXC, TXH;

unsigned long currentMillis;
unsigned long tempMillis;
unsigned long tempMillis2;
unsigned long tempMillis3;
unsigned long distanceMillis;
const unsigned long distPeriodSend = 0.002;
const unsigned long distPeriodStop = 0.01;
const unsigned long tempPeriod = 500;

void setup() {
  pinMode(pump,OUTPUT);
  pinMode(button,INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(peltier1, OUTPUT); 
  pinMode(peltier2, OUTPUT); 
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  tempMillis = millis();
  tempMillis2 = millis();
  distanceMillis = millis();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - tempMillis >= tempPeriod)    //Temperature
  {
  VRT = analogRead(A0);
  TXH1 = GetTemp(VRT);               
  Serial.print("HOT Temperature 1:");       
  Serial.print("\t");
  Serial.println(TXH1);
  tempMillis = currentMillis;
  }
  currentMillis = millis();
  if (currentMillis - tempMillis2 >= tempPeriod)    //Temperature
  {
  VRT = analogRead(A1);  
  TXH2 = GetTemp(VRT);               
  Serial.print("HOT Temperature 2:");       
  Serial.print("\t");
  Serial.println(TXH2);
  tempMillis2 = currentMillis;
  }
  currentMillis = millis();
  if (currentMillis - tempMillis3 >= tempPeriod)    //Temperature
  {
  VRT = analogRead(A2);
  TXC = GetTemp(VRT);               
  Serial.print("COLD Temperature:");       
  Serial.print("\t");
  Serial.println(TXC);
  tempMillis3 = currentMillis;
  }
  
  currentMillis = millis();
  if (currentMillis - distanceMillis >= distPeriodSend + distPeriodStop)    //Distance
  {
  distance= GetDistance();       
  Serial.print("Distance: ");         
  Serial.println(distance);
  distanceMillis = currentMillis;
  }
  if(digitalRead(button) == HIGH && distance <= 6){
   digitalWrite(pump,HIGH);
   }
  else
  {
   digitalWrite(pump,LOW);
  }
  if(TXC < 7)
    digitalWrite(LED,HIGH);
  else
    digitalWrite(LED,LOW);
    
  if(TXH1 >= 15 && TXH1 <= 45)
    digitalWrite(peltier1,HIGH);
  else
    digitalWrite(peltier1,LOW);
    
  if(TXH2 >= 15 && TXH2 <= 45)
    digitalWrite(peltier2,HIGH);
  else
    digitalWrite(peltier2,LOW);
    
  if(TXC < 0.5)
   {
       digitalWrite(peltier1,LOW);
       digitalWrite(peltier2,LOW);
       while(TXC < 2)
       {
        Serial.print("Too cold! Heating up! Temp: ");
        Serial.println(TXC);
        TXC = GetTemp(analogRead(A2));               
       }
   }
}
int GetDistance()
{
  digitalWrite(trigPin, LOW);         
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  return distance;
}

float GetTemp(float VRT)
{       
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = 5 - VRT;
  RT = VRT / (VR / 10000);               //Resistance of RT      
  ln = log(RT / 10000);
  TXH = (1 / ((ln / 3988) + (1 / 298.15))); //Temperature from thermistor
  TXH = TXH - 273.15;                 //Conversion to Celsius
  return TXH;
}
