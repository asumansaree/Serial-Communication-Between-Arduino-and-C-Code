#define ledPin 13
#define button 8

int button_status = 0;

void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++)
  {
    Serial.write(stringData[i]);   // Push each char 1 by 1 on each loop pass
    delay(100);
  }
  Serial.write('\0');

}// end writeString

void setup() {
  pinMode(button,INPUT);
  //Create Serial Object
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  //Have the Arduino wait to recieve inpit
  while(Serial.available() == 0);//wait until we get something from the computer, checks to see if there’s something waiting for the Arduino, loops the line until it gets something from computer

  //Read the Input sent
  String val = Serial.readStringUntil('\n');
  void led_on(void);
  void led_off(void);
  void led_on_three_times(void);
  void calculate_square(void);
  void stop();
  
  if(val == "1")
    led_on();
  else if(val == "2")
    led_off();
  else if(val == "3")
    led_on_three_times();
  else if(val == "4")
    calculate_square();
  else if(val == "5")
    button_press();
  else if(val == "0") //Once you press 0,program wont't accept your inputs anymore because it is terminated.
  {
    stop(); 
  }
  else
  {
    //Serial.println("Invalid");
  }
  
  Serial.flush();
}
void led_on(void)
{
    digitalWrite(ledPin,HIGH);
}
void led_off(void)
{
    digitalWrite(ledPin,LOW);
}
void led_on_three_times(void)
{
     for(int i=0; i<3; i++)
     {
      digitalWrite(ledPin,HIGH);
      delay(300);
      digitalWrite(ledPin,LOW);
      delay(300);
     }
}
void calculate_square(void)
{
    while(Serial.available() == 0);
    String x = Serial.readStringUntil('\0');
    //Serial.flush();
    double conv = x.toInt();
    double square = (conv*conv);
    delay(50);
    String myString = String(square);
    writeString(myString);
    //Serial.flush();
}
void button_press(void)
{
    String count = "1";
    while(Serial.available() == 0)
    {
      button_status = digitalRead(button);
      if(button_status == 1)
      {
        writeString(count);
        int count_as_int = count.toInt();
        count_as_int++;
        count = String(count_as_int);
        delay(800); //According to press speed we want,we can change this value 
      }
    }
    stop();
}
void stop()
{
 while(1);
}
