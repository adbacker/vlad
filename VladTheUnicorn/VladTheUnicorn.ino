#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif



#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

//use simple timer events instead of running inside the loop()
SimpleTimer timer;

bool vladIsOn=0;
int secondsToRun=0;
int CXNDELAY=10;
int activatedCount=0;
int secondsActive=0;
String vladLcdString1;
String vladLcdString2;


int led=LED_BUILTIN;
int hornControl=12;
uint16_t currentPixel=0;
#define stripPixels 13
#define PIN 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripPixels, PIN, NEO_GRB + NEO_KHZ800);
SimpleTimer stripTimer;
SimpleTimer displayTimer;

char auth[] = "1b5a7a77f78945eeac0f402f3c45537a";
#define WIFI_SSID "key-guest"
#define WIFI_PW "917$L^sk"
const int LED_PIN=4;



int currentColor=0;
int displayDelay=40;
int intensity=10;
String wipeType="chase";

String myIp;

//save the LCD lines so we're not blanking the whole thing every time
String lcdLine1;
String lcdLine2;

BLYNK_WRITE(V1)
{
  //int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  String wipe = param.asStr();
  // double d = param.asDouble();
  Serial.print("wipe value is: ");
  Serial.println(wipe);
  handleOn();
  wipeType=wipe;
}


void updateLcd() {
  String vladIsOnMessage = "";
  printToLcd("","");
  if (vladIsOn) {
    vladIsOnMessage = "Vlad ON: ";
    vladIsOnMessage += String(secondsToRun);
    vladIsOnMessage += " sec";
  }
  else {
    vladIsOnMessage="Vlad OFF";
  }
  printToLcd(myIp,vladIsOnMessage);
}

void printToLcd(String line1, String line2) {
  Serial.println(line1);
  Serial.println(line2);
  int line1Length=line1.length();
  int line2Length=line2.length();

  int line1Padding=16-line1.length();
  int line2Padding=16-line2.length();

  String line1PadString="";
  for (int i=0;i<line1Padding;i++) {
    line1PadString=line1PadString+" ";
  }
  String line2PadString="";
  for (int i=0;i<line2Padding;i++) {
    line2PadString=line2PadString+" ";
  }
  
  String line1Padded=line1+line1PadString;
  String line2Padded=line2+line2PadString;
  
  lcd.setCursor(0, 0);
  lcd.print(line1Padded); // Start Print text to Line 1
  lcd.setCursor(0, 1);      
  lcd.print(line2Padded); // Start Print Test to Line 2

  lcdLine1=line1;
  lcdLine2=line2;
}

//void printState() {
//  String stateString = "<html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><head style=\"font-size:36px\"><title>Vlad 3.0</title><p>VladServer 3.0</p><p style=\"font-size:24px\"><a href=\"http://" + myIp + "/\">reload page</a></p></head><body style=\"font-size:24px\"><p/><p/><p>vlad is currently: ";
//  if (vladIsOn) {
//    stateString = stateString + " ON for the next ";
//    stateString = stateString + secondsToRun;
//    stateString = stateString + " seconds ";
//  }
//  else {
//    stateString = stateString + " OFF ";
//  }
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/vladoff\">turn Vlad's horn off</a></p>";
//
//  stateString = stateString + "<hr>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/wipe?t=chase\">Horn chase mode LED</a></p>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/wipe?t=split\">Horn split mode LED</a></p>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/wipe?t=reversesplit\">Horn reverse split mode LED</a></p>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/wipe?t=lsd\">Horn LSD LED (dude....)</a></p>";
//  stateString = stateString + "<hr>";
//
//
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/intensity?t=10\">Horn intensity to mellow</a></p>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/intensity?t=30\">Horn intensity to medium</a></p>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/intensity?t=255\">Horn intensity to *** NUCLEAR*** </a></p>";
//  stateString = stateString + "<hr>";
//
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/speed?t=40\">Horn speed to normal</a></p>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/speed?t=20\">Horn speed to fast</a></p>";
//  stateString = stateString + "<p><a href=\"http://"+myIp+"/speed?t=10\">Horn speed to seizure</a></p>";
//
//  stateString = stateString + "</body></html>";
//  server.send(200, "text/html", stateString);
//}

/*
 * 
 * Web endpoint handlers
 * 
 * */

//void handleRoot() {
//    printState();
//}

void handleOn() {
  vladIsOn=true;
  secondsToRun=8;
}

void handleOff() {
  vladIsOn=false;
  colorWipe(strip.Color(0,0,0),(uint8_t)0);

}
    
void handleSpeed() {
  handleOn();  
//  displayDelay=server.arg(0).toInt();
  if(displayDelay>150) {
    displayDelay=150;
  }
  //printState();
}
void handleIntensity() {
  handleOn();
 // intensity=server.arg(0).toInt();
  if (intensity>255) {
    intensity=255;
  }
  //printState();
}

void handleWipe() {
  handleOn();
  //wipeType=server.arg(0);
  //printState();
}



void handleWake() {
  lcd.backlight();// Enable or Turn On the backlight 
  //printState();
}


//void handleNotFound(){
//  String message = "No soup for you! (file not found).\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET)?"GET":"POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  for (uint8_t i=0; i<server.args(); i++){
//    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//  }
//  server.send(404, "text/plain", message);
//}

/*
 * END web endpoint handlers
 * */

void displayTick() {
  if (secondsToRun > 0) {
    secondsToRun--;
  }
  if (secondsToRun<=0) {
    secondsToRun=0;
    handleOff();
  }
  updateLcd();
}

// Fill the dots one after the other with a color

void hornDisplayStep() {
  if (vladIsOn == true) {
    if(wipeType=="chase") {
      colorWipeStep();
    } else if (wipeType=="split") {
      splitWipeStep();
    } else if (wipeType=="reversesplit") {
      reverseSplitWipeStep();
    } else if (wipeType=="lsd") {
      lsdWipeStep();
    }
      else {
      //do the default
      colorWipeStep();
    }
  }
}

void colorWipeStep() {
// Fill the dots one after the other with a color
  switch(currentColor) {
    case 0:
      colorWipe(strip.Color(intensity,0,0),(uint8_t)displayDelay);
      currentColor=1;
      break;
    case 1:
      colorWipe(strip.Color(0,intensity,0),(uint8_t)displayDelay);
      currentColor=2;
      break;
    case 2:
      colorWipe(strip.Color(0,0,intensity),(uint8_t)displayDelay);
      currentColor=0;
      break;
    default:
      break;      
  }
}

void splitWipeStep() {
// Fill the dots one after the other with a color
  switch(currentColor) {
    case 0:
      splitWipe(strip.Color(intensity,0,0),(uint8_t)displayDelay);
      currentColor=1;
      break;
    case 1:
      splitWipe(strip.Color(0,intensity,0),(uint8_t)displayDelay);
      currentColor=2;
      break;
    case 2:
      splitWipe(strip.Color(0,0,intensity),(uint8_t)displayDelay);
      currentColor=0;
      break;
    default:
      break;      
  }
}

void reverseSplitWipeStep() {
// Fill the dots one after the other with a color
  switch(currentColor) {
    case 0:
      reverseSplitWipe(strip.Color(intensity,0,0),(uint8_t)displayDelay);
      currentColor=1;
      break;
    case 1:
      reverseSplitWipe(strip.Color(0,intensity,0),(uint8_t)displayDelay);
      currentColor=2;
      break;
    case 2:
      reverseSplitWipe(strip.Color(0,0,intensity),(uint8_t)displayDelay);
      currentColor=0;
      break;
    default:
      break;      
  }
}

void lsdWipeStep() {
    for(uint16_t i=0; i<strip.numPixels(); i++) {

      int whichSeed=random(3);
      int r=0;
      int g=0;
      int b=0;
      switch(whichSeed) {
        case 0:
          r=random(intensity);
          g=0;
          b=intensity;
          break;
        case 1:
          r=intensity;
          g=random(intensity);
          b=0;
          break;
        case 2:
          r=0;
          g=intensity;
          b=random(intensity);
          break;
        default:
          break;
      }
      uint32_t pixelColor=strip.Color(r,g,b);
      strip.setPixelColor(i, pixelColor);
  }
  strip.show();
  //delay(displayDelay);
}

void fadeUp(int mswait) {
    colorWipe(strip.Color(0,0,0),(uint8_t)0);
    for (int i=0;i<255;i+=2) {
        colorWipe(strip.Color(i,0,0),(uint8_t)0);
        delay(mswait);
    }
}

void fadeDown(int mswait) {
    colorWipe(strip.Color(255,0,0),(uint8_t)0);
    for (int i=255;i>0;i-=2) {
        colorWipe(strip.Color(i,0,0),(uint8_t)0);
        delay(mswait);
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void pulse(int pulses, int mswait) {
  if (mswait > 3) {
    mswait = 3; 
  }
  for (int i=0;i<pulses;i++) {
    fadeUp(mswait);
    fadeDown(mswait);
  }
}

void splitWipe(uint32_t c, uint8_t wait) {
  int totalPixels=13;
  int middlePixel=6;
  for(uint16_t i=0;i<=middlePixel;i++){
    int pixel1 = middlePixel+i;
    int pixel2 = middlePixel-i;
    strip.setPixelColor(pixel1,c);
    strip.setPixelColor(pixel2,c);
    strip.show();
    delay(wait);
  }
}

void reverseSplitWipe(uint32_t c, uint8_t wait) {
  int totalPixels=13;
  int middlePixel=6;
  for(uint16_t i=0;i<=middlePixel;i++){
    int pixel1 = i;
    int pixel2 = totalPixels-i;
    strip.setPixelColor(pixel1,c);
    strip.setPixelColor(pixel2,c);
    strip.show();
    delay(wait);
  }
}


void setup(void){
  vladIsOn=0;
  lcd.begin(16,2);   // initializing the LCD
  lcd.init(); 
  lcd.backlight();// Enable or Turn On the backlight 

  pinMode(led, OUTPUT);
  pinMode(hornControl,OUTPUT);
      
  Serial.begin(115200);
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  // (WiFi.status() != WL_CONNECTED) {
  for (int i=CXNDELAY;i>0;i--) {
    String num=String(i);
    printToLcd("cxn countdown",num);
    delay(1000);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
  }
  
  //  delay(500);
  //  Serial.print(".");
  //  printToLcd("waiting....","connection....");
  //}
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());

//  if (MDNS.begin("esp8266")) {
//    Serial.println("MDNS responder started");
//  }

//  server.on("/", handleRoot);
//  server.on("/vladon", handleOn);
//  server.on("/vladoff", handleOff);
//  server.on("/speed", handleSpeed);
//  server.on("/intensity", handleIntensity);
//  server.on("/wipe", handleWipe);
//  server.on("/wakeup", handleWake);
//  server.onNotFound(handleNotFound);

//  server.begin();
  Serial.println("HTTP server started");
  myIp=WiFi.localIP().toString();
//  flash(3);
  
  /*
   * BEGIN Arduino Over-the-air update stuff
   */

   //printToLcd("starting OTA","initialization");
   // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
 
  // Hostname defaults to esp8266-[ChipID]
  //ArduinoOTA.setHostname("vladTheImpaler");

  // No authentication by default
//  ArduinoOTA.setPassword((const char *)"Abcd1234");
//
//  ArduinoOTA.onStart([]() {
//    Serial.println("Start");
//  });
//  ArduinoOTA.onEnd([]() {
//    Serial.println("End");
//  });
//  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
//  });
//  ArduinoOTA.onError([](ota_error_t error) {
//    Serial.printf("Error[%u]: ", error);
//    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
//    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
//    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
//    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
//    else if (error == OTA_END_ERROR) Serial.println("End Failed");
//  });
//  ArduinoOTA.begin();
//  printToLcd("OTA init","complete");
//  flash(4);
  /*
  * END Arduino Over-the-air update stuff
  */
    Blynk.begin(auth, WIFI_SSID, WIFI_PW);

  stripTimer.setInterval(100L, hornDisplayStep);
  displayTimer.setInterval(1000L,displayTick);
  updateLcd();
  pulse(3,1);
}


void loop(void){
//  ArduinoOTA.handle();
//  server.handleClient();
  Blynk.run(); // Initiates Blynk
  stripTimer.run();
  displayTimer.run();
}
