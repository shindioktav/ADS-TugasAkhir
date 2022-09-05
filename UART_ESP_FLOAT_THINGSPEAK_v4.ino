#include <SoftwareSerial.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>


char ssid[] = "SUPIAN";
char pass[] = "sakinah31";

WiFiClient  client;

unsigned long Skripsi_one = 1765578; // Your Channel ID
unsigned long Skripsi_two = 1765650; // Your Channel ID
unsigned long Skripsi_three = 1765651; // Your Channel ID
unsigned long Skripsi_four = 1709038; // Your Channel ID

const char * myWriteAPIKey_one = "BK6RB5QX2TNYL7DX"; //Your write API key
const char * myWriteAPIKey_two = "4LEJFFXKVCI8G8FG"; //Your write API key
const char * myWriteAPIKey_three = "OS1JS9AMLNF9D9I3"; //Your write API key
const char * myWriteAPIKey_four = "T760L7VMF88QSCY8"; //Your write API key
  
const int _SuhuD = 2;
const int _SuhuC = 3;
const int _SuhuF = 4;
const int _SuhuA = 5;
const int _IncE = 1;
const int _IncD = 2;
const int _IncC = 3;
const int _IncF = 4;
const int _IncA = 5;
const int _Yaw = 7;
const int _Pitch = 6;
const int _SuhuE= 8;
const int _ArusE = 1;
const int _ArusC = 2;
const int _ArusD = 3;
const int _ArusF = 4;
const int _ArusA = 5;
const int _Az_rot = 6;
const int _El_rot = 7;
const int _Roll = 8;
const int _ImaxE = 1;
const int _ImaxC = 2;
const int _ImaxD = 3;
const int _ImaxF = 4;
const int _ImaxA = 5;
const int _El_init = 6;
const int _Az_init = 7;
const int _Status = 8;

float fr_XZ[37] = {0};
float fr_YZ[37] = {0};

float arusE[37] = {0};
float arusD[37] = {0};
float arusC[37] = {0};
float arusF[37] = {0};
float arusA[37] = {0};

float ImaxE;
float ImaxD;
float ImaxC;
float ImaxF;
float ImaxA;

float suhuE[37] = {0};
float suhuD[37] = {0};
float suhuC[37] = {0};
float suhuF[37] = {0};
float suhuA[37] = {0};

float az[37] = {0};
float elev[3] = {0};
float azimuth[37] = {0};

float rolly[37] = {0};
float pitchy[37] = {0};
float yawy[37] = {0};

float rad_E[37] = {0};
float rad_D[37] = {0};
float rad_C[37] = {0};
float rad_F[37] = {0};
float rad_A[37] = {0};

String myString;
char a;

int idx0, idx1, idx2, idx3, idx4,
    idx5, idx6, idx7, idx8, idx9,
    idx10, idx11, idx12, idx13, idx14,
    idx15, idx16, idx17, idx18, idx19,
    idx20, idx21, idx22,
    idx23,
    idx24, idx25, idx26, idx27, idx28;
    
String data1, data2, data3, data4, data5,
       data6, data7, data8, data9, data10,
       data11, data12, data13, data14, data15,
       data16, data17, data18, data19, data20,
       data21, data22,
       data23,
       data24, data25, data26, data27, data28;

int handshake = 0;
int handshake_nodemcu_ardu = 5;
int handshake_ardu_nodemcu = 15;
int handshake_done = 8;

float kode = 0;
float kode_arusmax = 18;
float kode_arusop = 55;

int i = 0;

SoftwareSerial comm_nodemcu(D7,D8); //rx,tx


void setup() {
   Serial.begin(115200);
   comm_nodemcu.begin(115200);
   WiFi.mode(WIFI_STA);
   ThingSpeak.begin(client);
   yield();
   internet();

   int update_status = random(0,100);

  comm_nodemcu.write(handshake_nodemcu_ardu);
  handshake = comm_nodemcu.read();
  while(handshake != handshake_ardu_nodemcu)
  {
    handshake = comm_nodemcu.read();
  }

   ThingSpeak.writeField(Skripsi_one, _Status, update_status, myWriteAPIKey_one);
   yield();
}

void loop()
{
  
  while(comm_nodemcu.available() <= 0)
    {
      ;
    }
  
    while(comm_nodemcu.available()>0){
      delay(10);
      a = comm_nodemcu.read();
      myString += a;    
    }
    if(myString.length()>0)
    {
      idx0 = myString.indexOf(',');
      idx19 = myString.indexOf(',',idx0+1);
  
      data19 = myString.substring(idx0+1, idx19);
  
      kode = data19.toFloat();
  
      if(kode == kode_arusop)
      {
        idx20 = myString.indexOf(',',idx19+1);
    
        idx21 = myString.indexOf(',',idx20+1);
        idx22 = myString.indexOf(',',idx21+1);
        idx23 = myString.indexOf(',',idx22+1); 
    
        data19 = myString.substring(idx0+1, idx19);
        data20 = myString.substring(idx19+1, idx20);
    
        data21 = myString.substring(idx20+1, idx21);
        data22 = myString.substring(idx21+1, idx22);
        data23 = myString.substring(idx22+1, idx23);
        
        rad_D[i] = data20.toFloat();
    
        rad_C[i] = data21.toFloat();
        rad_F[i] = data22.toFloat();
        rad_A[i] = data23.toFloat();
        
        myString = "";
  
        while(comm_nodemcu.available() <= 0)
          {
            ;
          }
          
          while(comm_nodemcu.available()>0){
            delay(10);
            a = comm_nodemcu.read();
            myString += a;    
          }
          if(myString.length()>0)
          {
            idx0 = myString.indexOf(',');
            idx1 = myString.indexOf(',',idx0+1);
            idx2 = myString.indexOf(',',idx1+1);
            idx3 = myString.indexOf(',',idx2+1);
            idx4 = myString.indexOf(',',idx3+1);
        
            idx5 = myString.indexOf(',',idx4+1);
            idx6 = myString.indexOf(',',idx5+1);
            idx7 = myString.indexOf(',',idx6+1);
            idx8 = myString.indexOf(',',idx7+1);
            
            idx9 = myString.indexOf(',',idx8+1);
            
            data1 = myString.substring(idx0+1, idx1);
            data2 = myString.substring(idx1+1, idx2);
            data3 = myString.substring(idx2+1, idx3);
            data4 = myString.substring(idx3+1, idx4);
            data5 = myString.substring(idx4+1, idx5);
        
            data6 = myString.substring(idx5+1, idx6);
            data7 = myString.substring(idx6+1, idx7);
            data8 = myString.substring(idx7+1, idx8);
            data9 = myString.substring(idx8+1, idx9);
        
            suhuE[i] = data1.toFloat();
            suhuD[i] = data2.toFloat();
            suhuC[i] = data3.toFloat();
            suhuF[i] = data4.toFloat();
            suhuA[i] = data5.toFloat();
        
            rolly[i] = data6.toFloat();
            pitchy[i] = data7.toFloat();
            yawy[i] = data8.toFloat();
            rad_E[i] = data9.toFloat();
            myString = "";
          }
          
          while(comm_nodemcu.available() <= 0)
          {
            ;
          }
        
          while(comm_nodemcu.available()>0){
            delay(10);
            a = comm_nodemcu.read();
            myString += a;    
          }
          if(myString.length()>0)
          {
            idx0 = myString.indexOf(',');
            idx10 = myString.indexOf(',',idx0+1);
            idx11 = myString.indexOf(',',idx10+1);
            idx12 = myString.indexOf(',',idx11+1);
        
            idx13 = myString.indexOf(',',idx12+1);
            idx14 = myString.indexOf(',',idx13+1);
            idx15 = myString.indexOf(',',idx14+1);
            idx16 = myString.indexOf(',',idx15+1);
        
            idx17 = myString.indexOf(',',idx16+1);
            idx18 = myString.indexOf(',',idx17+1);
            
            data10 = myString.substring(idx0+1, idx10);
        
            data11 = myString.substring(idx10+1, idx11);
            data12 = myString.substring(idx11+1, idx12);
            data13 = myString.substring(idx12+1, idx13);
            data14 = myString.substring(idx13+1, idx14);
            data15 = myString.substring(idx14+1, idx15);
        
            data16 = myString.substring(idx15+1, idx16);
            data17 = myString.substring(idx16+1, idx17);
            data18 = myString.substring(idx17+1, idx18);
            
        
            arusE[i] = data10.toFloat();
        
            arusD[i] = data11.toFloat();
            arusC[i] = data12.toFloat();
            arusF[i] = data13.toFloat();
            arusA[i] = data14.toFloat();
            fr_XZ[i] = data15.toFloat();
        
            fr_YZ[i] = data16.toFloat();
            az[i] = data17.toFloat();
            elev[i] = data18.toFloat();
            
            myString = "";
          }
            
            ThingSpeak.setField(_ArusE,arusE[i]);
            ThingSpeak.setField(_ArusD,arusD[i]);
            ThingSpeak.setField(_ArusC,arusC[i]);
            ThingSpeak.setField(_ArusF,arusF[i]);
            ThingSpeak.setField(_ArusA,arusA[i]);
            ThingSpeak.setField(_Az_rot,az[i]); 
            ThingSpeak.setField(_El_rot,elev[i]); 
            ThingSpeak.setField(_Roll,rolly[i]);                    
            ThingSpeak.writeFields(Skripsi_two, myWriteAPIKey_two); //ThingSpeak.writeField(channel ID, fields ID, data yg mau disend, write api key);
                                      
            yield();
            delay(5000);
                              
            //sudut penyinaran
            ThingSpeak.setField(_IncE,rad_E[i]);
            ThingSpeak.setField(_IncD,rad_D[i]);
            ThingSpeak.setField(_IncC,rad_C[i]);
            ThingSpeak.setField(_IncF,rad_F[i]);
            ThingSpeak.setField(_IncA,rad_A[i]);
            ThingSpeak.setField(_Yaw,yawy[i]);
            ThingSpeak.setField(_Pitch,pitchy[i]);
            ThingSpeak.setField(_SuhuE,suhuE[i]);
            ThingSpeak.writeFields(Skripsi_three, myWriteAPIKey_three); //ThingSpeak.writeField(channel ID, fields ID, data yg mau disend, write api key);
                  
            yield();
            delay(5000);
                  
            ThingSpeak.setField(_Az_init,fr_XZ[i]);
            ThingSpeak.setField(_El_init,fr_YZ[i]);
            ThingSpeak.writeFields(Skripsi_one, myWriteAPIKey_one); //ThingSpeak.writeField(channel ID, fields ID, data yg mau disend, write api key);
                  
            yield();
            delay(5000);
                  
            ThingSpeak.setField(_SuhuD,suhuD[i]);
            ThingSpeak.setField(_SuhuC,suhuC[i]);
            ThingSpeak.setField(_SuhuF,suhuF[i]);
            ThingSpeak.setField(_SuhuA,suhuA[i]);
            ThingSpeak.writeFields(Skripsi_four, myWriteAPIKey_four); //ThingSpeak.writeField(channel ID, fields ID, data yg mau disend, write api key);

            i += 1;

            yield();
            delay(5000);
          
          comm_nodemcu.write(handshake_done);

          if(i == 36)
          {
            i = 0;
            i = 0;
          }

          else if(i < 36)
          {
            ;
          }
      }
  
      else if(kode == kode_arusmax)
      {
        idx24 = myString.indexOf(',',idx19+1);
    
        idx25 = myString.indexOf(',',idx24+1);
        idx26 = myString.indexOf(',',idx25+1);
        idx27 = myString.indexOf(',',idx26+1);
        idx28 = myString.indexOf(',',idx27+1); 
    
        data19 = myString.substring(idx0+1, idx19);
        data24 = myString.substring(idx19+1, idx24);
    
        data25 = myString.substring(idx24+1, idx25);
        data26 = myString.substring(idx25+1, idx26);
        data27 = myString.substring(idx26+1, idx27);
        data28 = myString.substring(idx27+1, idx28);
  
            ImaxE = data24.toFloat();
            ImaxD = data25.toFloat();
            ImaxC = data26.toFloat();
            ImaxF = data27.toFloat();
            ImaxA = data28.toFloat();
        
        myString = "";
  
            ThingSpeak.setField(_ImaxE,ImaxE);
            ThingSpeak.setField(_ImaxD,ImaxD);
            ThingSpeak.setField(_ImaxC,ImaxC);
            ThingSpeak.setField(_ImaxF,ImaxF);
            ThingSpeak.setField(_ImaxA,ImaxA);
              
            ThingSpeak.writeFields(Skripsi_one, myWriteAPIKey_one); //ThingSpeak.writeField(channel ID, fields ID, data yg mau disend, write api key);
            yield();
            delay(5000); 
          
          comm_nodemcu.write(handshake_done);
      }
    }
  
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      delay(5000);
      yield();
    }
  }
}
