/*
 * Work in progress thermostat.
 * 
 * A desired temperature is entered using an IR remote.
 * The Arduino uses a stepper to turn on and off a physical AC switch
 * depending on the current temperature sensed by the DHT11.
 * The current temperature and desired temperature are displayed on an LCD.
 */

#include <dht_nonblocking.h> //For DHT11
#include <LiquidCrystal.h> //For LCD
#include <IRremote.h> //For IR Remote
#include <Stepper.h> //For stepper


#define DHT_SENSOR_TYPE DHT_TYPE_11 //For DHT11
static const int DHT_SENSOR_PIN = 13; // For DHT11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );  // For DHT11

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //LCD pins

int receiver = 12; //Reciver in pin 12
IRrecv irReceiver(receiver);
decode_results results;
String enteredTempStr; //Temperature entered by user



/*
 * Sets up LCD, IR remote, and Serial.
 */
void setup() 
{
  //LCD setup
  lcd.begin(16, 2);

  //Remote
  irReceiver.enableIRIn();
  pinMode(LED_BUILTIN, OUTPUT);

  //For serial output
  Serial.begin( 9600);

}



/*
 * Main loop. Mainly calls functions.
 */
void loop() 
{
  float temperature;
  String btnNum;
  int enteredTemperature;
  
 
  temperature = getTemp();
  displayCurrentTemp(temperature);

  enteredTemperature = getBtnNum().toInt(); //Converts temperature to int
  
  adjustTemperature(enteredTemperature);
    
}



/*
 * Calls measure(), converts temperature to fahrenheit and returns
 * temperature in fahrenheit.
 */
float getTemp()
{
  float temperature;
  float humidity;
  float tempF;

  lcd.setCursor(0, 0);

  if (measure (&temperature, &humidity) == true)
  {
    tempF = ((temperature * 1.8) + 32); //Fahrenheit conversion
  }

  return tempF;
}



/*
 * Displays the current temperature on the LCD.
 */
void displayCurrentTemp(float temperature)
{
  lcd.setCursor(1, 0);

  lcd.print("Currently: ");
  lcd.print(temperature);
}



/*
 * Takes a measurement of the temperature and humidity of the DHT11.
 */
static bool measure(float *temperature, float *humidity)
{
  static unsigned long timeStamp = millis();
  if (millis() - timeStamp > 1000ul)
  {
    if (dht_sensor.measure( temperature, humidity ) == true)
    {
      timeStamp = millis();
      return (true);
    }
  }
  return false;
}



/*
 * Decodes the button pressed on the IR remote.
 * Only allows the entered number to be at least 2 digets.
 */
String getBtnNum()
{
  String btnNum;
  
  if (irReceiver.decode(&results))
  {
    switch (results.value)
    {
      case 0xFF6897:
        btnNum = '0';
        break;
  
      case 0xFF30CF:
        btnNum = '1';
        break;
  
      case 0xFF18E7:
        btnNum = '2';
        break;
  
      case 0xFF7A85:
        btnNum = '3';
        break;
  
      case 0xFF10EF:
        btnNum = '4';
        break;
  
      case 0xFF38C7:
        btnNum = '5';
        break;
  
      case 0xFF5AA5:
        btnNum = '6';
        break;
  
      case 0xFF42BD:
        btnNum = '7';
        break;
  
      case 0xFF4AB5:
        btnNum = '8';
        break;
  
      case 0xFF52AD:
        btnNum = '9';
        break;
    }
  }
  irReceiver.resume(); // receive the next value
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  enteredTempStr += btnNum; //Adds the new button pressed

  //Only allows the temperature to be 2 digets.
  if (enteredTempStr.length() > 2)
  {
    lcd.setCursor(8, 1);
    enteredTempStr = "";
    lcd.print("   ");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Target: ");
    lcd.print(enteredTempStr);
    return enteredTempStr;
  }
}



/* 
 *  Adjusts the temperature of the room.
 * If the DHT11 reads above the set temperature by 5 degrees,
 * the stepper turns (however many steps) to turn the AC on (in summer).
 * If the DHT11 reads below the set temperature by 5 degrees,
 * the stepper turns (however many steps) to turn the heat on (in winter).
*/ 
void adjustTemperature(int enteredTemperature)
{
  if (enteredTemperature < 100 && enteredTemperature > 0)
  {
    
  }
}
