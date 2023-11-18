#include<SoftwareSerial.h>
SoftwareSerial mySerial (D1, D2);
bool getPosition(void) ;
void serialFlush(void) ;
float latitude, longitude, speed;

#define trigger A0

unsigned long previousMillis = 0;
const long interval = 1000;
int count = 0;

#include <TridentTD_LineNotify.h>
#define SSID        "Pet" 
#define PASSWORD    "88888888"
#define LINE_TOKEN  "CWoRfKTXzlbKc9ZN2OtrVFP5oTXp9Lk4X5Q8Wcj78sP"

int c1, c2;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(trigger, INPUT_PULLUP);
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Se
  }rial.print(".");
    delay(400);
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  LINE.setToken(LINE_TOKEN);

} // end setup

void loop()
{

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {

    previousMillis = currentMillis;
    count += 1;
    Serial.println("                                           count = " + String(count));
    if ( count >= 60 )
    {
      count = 0;
    }

  }// end millis

  if ( count >= 40 && count <= 50)
  {
    if (getPosition()) {
      Serial.print(latitude, 6);
      Serial.print(",");
      Serial.print(longitude, 6);
      Serial.print(" | Speed (Km) : ");
      Serial.print(speed, 2);
      Serial.println();
      delay(2000);
      Serial.println("******************************");
    }// end  getPosition

  }

  Serial.println("                           analog = " + String(analogRead(trigger)));
  
    if ( analogRead(trigger) <= 100 )
    {
      Serial.println("                         Raspberry Trigger !! ");
      c1 += 1;
      c2 = 0;
      if ( c1 == 1 )
      {
        String pos = "http://maps.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
        // ตัวอย่างส่งข้อความ
        Serial.println("                    Line !!");
        LINE.notify(pos);
        delay(2000);
      }
      if ( c1 == 10 )
      {
        c1 = 2;
      }

    }
    else
    {
      c2 += 1;
      c1 = 0;
      if ( c2 == 1 )
      {

      }
      if ( c2 == 10 )
      {
        c2 = 2;
      }
      Serial.println("waiting Signal....");
    }

  

}  // end loop

bool getPosition() {
  if (mySerial.available()) {
    String line = "";
    while (mySerial.available()) {
      char c = mySerial.read();
      Serial.print(c);
      if (c == '\r') {
        if (line.indexOf("$GPRMC") >= 0) {
          Serial.println(line);
          String dataCut[13];
          int index = 0;
          for (int dataStart = 0; dataStart < line.length();) {
            dataCut[index] = line.substring(dataStart + 1, line.indexOf(',', dataStart + 1));
            //    Serial.println(dataCut[index]);
            dataStart = line.indexOf(',', dataStart + 1);
            index++;
          }
          if (dataCut[2] == "A") {
            int dotPos = 0;
            dotPos = dataCut[3].indexOf('.');
            String latDeg = dataCut[3].substring(0, dotPos - 2);
            String latMin = dataCut[3].substring(dotPos - 2, dotPos + 10);
            dotPos = dataCut[5].indexOf('.');
            String lngDeg = dataCut[5].substring(0, dotPos - 2);
            String lngMin = dataCut[5].substring(dotPos - 2, dotPos + 10);
            latitude = (latDeg.toFloat() + (latMin.toFloat() / 60.0)) * (dataCut[4] == "N" ? 1 : -1);
            longitude = (lngDeg.toFloat() + (lngMin.toFloat() / 60.0)) * (dataCut[6] == "E" ? 1 : -1);
            speed = dataCut[7].toFloat() * 1.652;

            return true;
            // re
          } else {
            Serial.println("Error : No fix now.");
          }
          Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>");
          serialFlush();
        }
        line = "";
      } else if (c == '\n') {
        // pass
      } else {
        line += c;
      }
      delay(1);
    }
  }
  Serial.println("....................");
  return false;
  // connect_();
}

void serialFlush() {
  while (Serial.available()) Serial.read();
}


// SSID is called from Service Set Identifier
