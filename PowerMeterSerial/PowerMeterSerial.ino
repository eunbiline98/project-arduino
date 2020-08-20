//Michael Klements
//The DIY Life
//27 October 2014

int currentPin = A0;              //Assign CT input to pin 1
double kilos = 0;
int peakPower = 0;

void setup() 
{ 
  Serial.begin(9600);            //Start serial communication
  Serial.println("Running");
}

void loop() 
{ 
  int current = 0;
  int maxCurrent = 0;
  int minCurrent = 1000;
  for (int i=0 ; i<=200 ; i++)  //Monitors and logs the current input for 200 cycles to determine max and min current
  {
    current = analogRead(currentPin);    //Reads current input and records maximum and minimum current
    if(current >= maxCurrent)
      maxCurrent = current;
    else if(current <= minCurrent)
      minCurrent = current;
  }
  if (maxCurrent <= 517)
  {
    maxCurrent = 516;
  }
  double RMSCurrent = ((maxCurrent - 516)*0.707)/11.8337;    //Calculates RMS current based on maximum value
  int RMSPower = 220*RMSCurrent;    //Calculates RMS Power Assuming Voltage 220VAC, change to 110VAC accordingly
  if (RMSPower > peakPower)
  {
    peakPower = RMSPower;
  }
  kilos = kilos + (RMSPower * (2.05/60/60/1000));    //Calculate kilowatt hours used
  delay (2000);
  Serial.print(RMSCurrent);
  Serial.println("A");
  Serial.print(RMSPower);
  Serial.println("W");
  Serial.print(kilos);
  Serial.println("kWh");
  Serial.print(peakPower);
  Serial.println("W");
}
