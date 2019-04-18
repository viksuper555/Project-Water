long duration;
int distance;
const int pump = 2;
const int button = 3;
const int trigPin = 4;
const int echoPin = 5;

float RT, VR, ln, TX, T0, VRT;

unsigned long currentMillis;
unsigned long tempMillis;
unsigned long tempMillis2;
unsigned long distanceMillis;
const unsigned long distPeriodSend = 0.002;
const unsigned long distPeriodStop = 0.01;
const unsigned long tempPeriod = 500;
void setup() {
  pinMode(pump,OUTPUT);
  pinMode(button,INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
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
  TX = GetTemp(VRT);               
  Serial.print("Temperature:");       
  Serial.print("\t");
  Serial.println(TX);
  tempMillis = currentMillis;
  }
  currentMillis = millis();
  if (currentMillis - tempMillis2 >= tempPeriod)    //Temperature
  {
  VRT = analogRead(A1);  
  TX = GetTemp(VRT);               
  Serial.print("Temperature 2:");       
  Serial.print("\t");
  Serial.println(TX);
  tempMillis2 = currentMillis;
  }
  currentMillis = millis();
  if (currentMillis - distanceMillis >= distPeriodSend + distPeriodStop)    //Distance
  {
  distance= GetDistance();       
  Serial.print("Distance: ");         
  Serial.println(distance);
  distanceMillis = currentMillis;
  }

if(digitalRead(button) == HIGH && distance <= 6 && TX >= 20 && TX <= 30){
 digitalWrite(pump,HIGH);
 }
else
 digitalWrite(pump,LOW);
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

int GetTemp(float VRT)
{       
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = 5 - VRT;
  RT = VRT / (VR / 10000);               //Resistance of RT

  ln = log(RT / 10000);
  TX = (1 / ((ln / 3988) + (1 / 298.15))); //Temperature from thermistor

  TX = TX - 273.15;                 //Conversion to Celsius
  return TX;
}
