#include <Wire.h>
#include <math.h>
#include <Adafruit_INA219.h>

#define comm_nodemcu Serial2

#define LED_Merah 2
#define LED_Hijau 5

Adafruit_INA219 ina219_E(0x40);
Adafruit_INA219 ina219_D(0x41); //tadinya D 45
Adafruit_INA219 ina219_C(0x45); //tadinya C 41
Adafruit_INA219 ina219_F(0x44); //tadinya B
Adafruit_INA219 ina219_A(0xF4240);

//150watt
float ImaxE = 18.4;//sumbu x
float ImaxD = 147.2;//sumbu y
float ImaxF = 161.3;//sumbu z
float ImaxA = 19;//sumbu (-1)*x
float ImaxC = 148;//sumbu (-1)*y
float ImaxB = 0;//sumbu (-1)*z

//100watt
//float ImaxE = 6.1;//sumbu x
//float ImaxD = 80.6;//sumbu y
//float ImaxF = 93.2;//sumbu z
//float ImaxA = 6.3;//sumbu (-1)*x
//float ImaxC = 81.8;//sumbu (-1)*y
//float ImaxB = 0;//sumbu (-1)*z

//50watt
//float ImaxE = 0.1;//sumbu x
//float ImaxD = 57.9;//sumbu y
//float ImaxF = 61;//sumbu z
//float ImaxA = 0.1;//sumbu (-1)*x
//float ImaxC = 55.4;//sumbu (-1)*y
//float ImaxB = 0;//sumbu (-1)*z

float fr_XZ[37] =
{0 ,5  ,10 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50 ,55 ,60 ,65 ,70 ,75 ,80 ,85 ,90 ,95 ,100  ,105  ,110  ,115  ,120  ,125  ,130  ,135  ,140  ,145  ,150  ,155  ,160  ,165  ,170  ,175  ,180};

float fr_YZ[37] = {0};

float arusE[37] = {0};
float arusD[37] = {0};
float arusC[37] = {0};
float arusF[37] = {0};
float arusA[37] = {0};

float IE[10] = {0};
float ID[10] = {0};
float IC[10] = {0};
float IF[10] = {0};
float IA[10] = {0};

float Imax_E[10] = {0};
float Imax_D[10] = {0};
float Imax_C[10] = {0};
float Imax_F[10] = {0};
float Imax_A[10] = {0};

float suhuE[37] = {0};
float suhuD[37] = {0};
float suhuC[37] = {0};
float suhuF[37] = {0};
float suhuA[37] = {0};

float IE_init[37] = {0};
float ID_init[37] = {0};
float IC_init[37] = {0};
float IF_init[37] = {0};
float IA_init[37] = {0};

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

int opsi[37] = {0};

String kirim_nodemcu;

int notif = 0;
int notif_done = 5;

int handshake = 0;
int handshake_nodemcu_ardu = 8;
int handshake_ardu_nodemcu = 15;
int handshake_done = 5;

float kode_arusmax = 18;
float kode_arusop = 55;

void setup(void)
{
  Serial.begin(115200);
  comm_nodemcu.begin(115200);

  //initialize system
   handshake = comm_nodemcu.read();
   while(handshake != handshake_nodemcu_ardu)
   {
    handshake = comm_nodemcu.read();
   }

   delay(1000);

   comm_nodemcu.write(handshake_ardu_nodemcu);

   handshake = 0;
   
//  Wire.begin(8);

  pinMode(LED_Hijau,OUTPUT);
  pinMode(LED_Merah,OUTPUT);

  digitalWrite(LED_Hijau,LOW);
  digitalWrite(LED_Merah,LOW);
  
  ina219_E.begin();
  ina219_D.begin();
  ina219_C.begin();
  ina219_F.begin();
  ina219_A.begin();

  uint32_t currentFrequency;

  ina219_E.setCalibration_16V_400mA();
  ina219_D.setCalibration_16V_400mA();
  ina219_C.setCalibration_16V_400mA();
  ina219_F.setCalibration_16V_400mA();
  ina219_A.setCalibration_16V_400mA();

   for(int y = 0; y<37; y++)
   {
    fr_YZ[y] = 90;
   }

}

void loop(void)
{
  int statearusmax = analogRead(A8);
  int statearusop = analogRead(A15);

  if(statearusmax > 1020 && statearusop < 1020)
  {
    ukur_arusmax();
    handshake = comm_nodemcu.read();
    digitalWrite(LED_Merah,HIGH);
    while(handshake != handshake_done)
    {
      handshake = comm_nodemcu.read();
    }
    delay(1000);
    kirim_nodemcu = "";
    handshake = 0;
    digitalWrite(LED_Merah,LOW);
  }

  else if(statearusmax < 1020 && statearusop > 1020)
  {
    handshake = handshake_done;
    ukur_arus();
  }

  else if(statearusmax < 1020 && statearusop < 1020)
  {
    ;
  }

}

float thisdegree(double thisdeg)
{
  return (thisdeg * 180 * 7) / 22;
}

float degr(double deg)
{
  return (deg*22)/(7*180);
}

void ukur_arus()
{ 
  for(int y = 0; y<3; y++)
  {
    digitalWrite(LED_Hijau,HIGH);
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Hijau,LOW);
    digitalWrite(LED_Merah,LOW);
  }

  for(int i = 0; i<37; i++)
  {

    while(handshake != handshake_done)
    {
      handshake = comm_nodemcu.read();
    }
    
    for(int y = 0; y<5; y++)
    {
      digitalWrite(LED_Hijau,HIGH);
      delay(500);
      digitalWrite(LED_Hijau,LOW);
    }

    for(int y = 0; y<5; y++)
    {
      digitalWrite(LED_Merah,HIGH);
      delay(500);
      digitalWrite(LED_Merah,LOW);
    }

    handshake = 0;

    for(int p = 0; p<7; p++)
    {
      digitalWrite(LED_Merah,HIGH);
      for(int y = 0; y<10; y++)
      {
        IE[y] = ina219_E.getCurrent_mA() - 0.1;
        arusE[i] = max(arusE[i],IE[y]);
        for(int o = 0; o < 5; o++)
        {
          if(arusE[18+o] <= 1.3)
          {
            arusE[18+o] = 0;
          }

          else
          {
            ;
          }
        }
        suhuE[i] = (analogRead(A0)*100*5/1023) + 0.16;
        if(arusE[i] < 0)
        {
          arusE[i] = 0;
        }

        else if(arusE[i] >= 0)
        {
          if(arusE[i] > ImaxE)
          {
            arusE[i] = ImaxE;
          }

          else if(arusE[i] < ImaxE)
          {
            ;
          }
        }

        ID[y] = ina219_D.getCurrent_mA() - 0.1;
        arusD[i] = max(arusD[i],ID[y]);
        if(arusD[i] <= 2.6)
        {
          arusD[i] = 0;
        }

        else
        {
          ;
        }
        suhuD[i] = (analogRead(A1)*100*5/1023) + 0.16;
        if(arusD[i] < 0)
        {
          arusD[i] = 0;
        }

        else if(arusD[i] >= 0)
        {
          if(arusD[i] > ImaxD)
          {
            arusD[i] = ImaxD;
          }

          else if(arusD[i] < ImaxD)
          {
            ;
          }
        }

        IC[y] = ina219_C.getCurrent_mA() - 0.1;
        arusC[i] = max(arusC[i],IC[y]);
        if(arusC[i] <= 2.6)
        {
          arusC[i] = 0;
        }

        else
        {
          ;
        }
        suhuC[i] = (analogRead(A2)*100*5/1023) + 0.16;
        if(arusC[i] < 0)
        {
          arusC[i] = 0;
        }

        else if(arusC[i] >= 0)
        {
          if(arusC[i] > ImaxC)
          {
            arusC[i] = ImaxC;
          }

          else if(arusC[i] < ImaxC)
          {
            ;
          }
        }

        IF[y] = ina219_F.getCurrent_mA() - 0.1;
        arusF[i] = max(arusF[i],IF[y]);
        suhuF[i] = (analogRead(A3)*100*5/1023) + 0.16;
        if(arusF[i] < 0)
        {
          arusF[i] = 0;
        }

        else if(arusF[i] >= 0)
        {
          if(arusF[i] > ImaxF)
          {
            arusF[i] = ImaxF;
          }

          else if(arusF[i] < ImaxF)
          {
            ;
          }
        }

        IA[y] = ina219_A.getCurrent_mA() - 0.1;
        arusA[i] = max(arusA[i],IA[y]);
        for(int o = 0; o<5; o++)
        {
          if(arusA[15+o] <= 1.3)
          {
            ArusA[15+o] = 0; 
          }

          else
          {
            ;
          }
        }         
        suhuA[i] = (analogRead(A4)*100*5/1023) + 0.16;
        if(arusA[i] < 0)
        {
          arusA[i] = 0;
        }

        else if(arusA[i] >= 0)
        {
          if(arusA[i] > ImaxA)
          {
            arusA[i] = ImaxA;
          }

          else if(arusA[i] < ImaxA)
          {
            ;
          }
        }
      }
    }

    IE_init[i] = ImaxE*cos(degr(fr_XZ[i]));

    if(IE_init[i] < 0)
    {
      IE_init[i] = 0;
    }

    else if(IE_init[i] >= 0)
    {
      ;
    }
    
    ID_init[i] = ImaxD*sin(degr(fr_XZ[i]))*cos(degr(fr_YZ[i]));

    if(ID_init[i] < 0)
    {
      ID_init[i] = 0;
    }

    else if(ID_init[i] >= 0)
    {
      ;
    }
    
    IF_init[i] = ImaxF*sin(degr(fr_XZ[i]))*sin(degr(fr_YZ[i]));

    if(IF_init[i] < 0)
    {
      IF_init[i] = 0;
    }

    else if(IF_init[i] >= 0)
    {
      ;
    }
    
    IA_init[i] = (-1)*ImaxA*cos(degr(fr_XZ[i]));

    if(IA_init[i] < 0)
    {
      IA_init[i] = 0;
    }

    else if(IA_init[i] >= 0)
    {
      ;
    }
    
    IC_init[i] = (-1)*ImaxC*sin(degr(fr_XZ[i]))*cos(degr(fr_YZ[i]));

    if(IC_init[i] < 0)
    {
      IC_init[i] = 0;
    }

    else if(IC_init[i] >= 0)
    {
      ;
    }
    
     rad_E[i] = acos(thisdegree(arusE[i]/ImaxE));
     rad_D[i] = acos(thisdegree(arusD[i]/ImaxD));
     rad_C[i] = acos(thisdegree(arusC[i]/ImaxC));
     rad_F[i] = acos(thisdegree(arusF[i]/ImaxF));
     rad_A[i] = acos(thisdegree(arusA[i]/ImaxA));
     
    if (arusE[i] > arusA[i])
       {
        if (arusE[i] >= IE_init[i])
        {
            yawy[i] = 0;
            pitchy[i] = 0;
            if (arusD[i] > 0 || arusC[i] > 0)
            {
                if (arusD[i] > arusC[i])
                {
                    rolly[i] = asin(thisdegree(arusD[i]/(ImaxD*sin(degr(fr_XZ[i]))*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])))));
                    azimuth[i] = asin(thisdegree(arusD[i]/(ImaxD*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])*sin(degr(rolly[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = asin(thisdegree((-1)*arusD[i]/(ImaxD*sin(degr(azimuth[i])*cos(degr(yawy[i]))))));
                    opsi[i] = 1;
                }
                else if (arusD[i] < arusC[i])
                {
                    rolly[i] = asin(thisdegree(arusC[i]/((-1)*ImaxC*sin(degr(fr_XZ[i]))*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])))));
                    azimuth[i] = asin(thisdegree(arusC[i]/((-1)*ImaxC*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])*sin(degr(rolly[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = asin(thisdegree(arusC[i]/(ImaxC*sin(degr(azimuth[i])*cos(degr(yawy[i]))))));
                    opsi[i] = 2;
                }
            }
            else if (arusD[i] == arusC[i])
            {
                rolly[i] = 0;
                azimuth[i] = acos(thisdegree(arusE[i]/(ImaxE*cos(degr(pitchy[i])*cos(degr(yawy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = rolly[i]*(-1);
                opsi[i] = 3;
            }
        }
        else if (arusE[i] < IE_init[i])
        {
            pitchy[i] = 0;
            rolly[i] = 0;
            if (arusD[i] > arusC[i])
            {
                yawy[i] = asin(thisdegree(arusD[i]/((-1)*ImaxD*cos(degr(fr_XZ[i]))*cos(degr(pitchy[i])))));
                azimuth[i] = acos(thisdegree(arusD[i]/((-1)*ImaxD*sin(degr(yawy[i])*cos(degr(pitchy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = rolly[i] * (-1);
                opsi[i] = 4;
            }
            else if (arusD[i] < arusC[i])
            {
                yawy[i] = asin(thisdegree(arusC[i]/(ImaxC*cos(degr(fr_XZ[i]))*cos(degr(pitchy[i])))));
                azimuth[i] = acos(thisdegree(arusC[i]/(ImaxC*sin(degr(yawy[i])*cos(degr(pitchy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = rolly[i] * (-1);
                opsi[i] = 5;
            }
            else if (arusD[i] == arusC[i])
            {
                yawy[i] = 0;
                azimuth[i] = asin(thisdegree(arusF[i]/(ImaxF*sin(degr(fr_YZ[i]))*cos(degr(rolly[i])*cos(degr(pitchy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = asin(thisdegree(arusF[i]/(ImaxF*sin(degr(azimuth[i])*cos(degr(pitchy[i]))))));
                opsi[i] = 6;
            }
        }
    }
        
    else if (arusE[i] < arusA[i])
    {
        if (arusA[i] >= IA_init[i])
        {
            pitchy[i] = 0;
            yawy[i] = 0;
            if (arusD[i] > 0 || arusC[i] > 0)
            {
                if (arusD[i] > arusC[i])
                {
                    rolly[i] = asin(thisdegree(arusD[i]/(ImaxD*sin(degr(fr_XZ[i]))*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])))));
                    azimuth[i] = 180 - asin(thisdegree(arusD[i]/(ImaxD*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])*sin(degr(rolly[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = asin(thisdegree((-1)*arusD[i]/(ImaxD*sin(degr(azimuth[i])*cos(degr(yawy[i]))))));
                    opsi[i] = 7;
                }
                else if (arusD[i] < arusC[i])
                {
                    rolly[i] = asin(thisdegree(arusC[i]/((-1)*ImaxC*sin(degr(fr_XZ[i]))*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])))));
                    azimuth[i] = 180 - asin(thisdegree(arusC[i]/((-1)*ImaxC*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])*sin(degr(rolly[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = asin(thisdegree(arusC[i]/(ImaxC*sin(degr(azimuth[i])*cos(degr(yawy[i]))))));
                    opsi[i] = 8;
                }
            }
            else if (arusD[i] == arusC[i])
            {
                rolly[i] = 0;
                azimuth[i] = acos(thisdegree(arusA[i]/((-1)*ImaxA*cos(degr(pitchy[i])*cos(degr(yawy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = rolly[i] * (-1);
                opsi[i] = 9;
            }
        }
        else if (arusA[i] < IA_init[i])
        {
            pitchy[i] = 0;
            rolly[i] = 0;
            if (arusD[i] > arusC[i])
            {
                yawy[i] = asin(thisdegree(arusD[i]/((-1)*ImaxD*cos(degr(fr_XZ[i]))*cos(degr(pitchy[i])))));
                azimuth[i] = acos(thisdegree(arusD[i]/((-1)*ImaxD*sin(degr(yawy[i])*cos(degr(pitchy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = rolly[i] * (-1);
                opsi[i] = 10;
            }
            else if (arusD[i] < arusC[i])
            {
                yawy[i] = asin(thisdegree(arusC[i]/(ImaxC*cos(degr(fr_XZ[i]))*cos(degr(pitchy[i])))));
                azimuth[i] = acos(thisdegree(arusC[i]/(ImaxC*sin(degr(yawy[i])*cos(degr(pitchy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = rolly[i] * (-1);
                opsi[i] = 11;
            }
            else if (arusD[i] == arusC[i])
            {
                yawy[i] = 0;
                azimuth[i] = asin(thisdegree(arusF[i]/(ImaxF*sin(degr(fr_YZ[i]))*cos(degr(rolly[i])*cos(degr(pitchy[i]))))));
                az[i] = azimuth[i] - 90;
                elev[i] = asin(thisdegree(arusF[i]/(ImaxF*sin(degr(azimuth[i])*cos(degr(pitchy[i]))))));
                opsi[i] = 12;
            }
         }
    }
        
        
    else if (arusE[i] == arusA[i])
    {
        if (arusD[i] > 0 || arusC[i] > 0)
        {
            pitchy[i] = 0;
            if (arusF[i] < IF_init[i])
            {
                yawy[i] = 0;
                if (arusD[i] > arusC[i])
                {
                    rolly[i] = asin(thisdegree(arusD[i]/(ImaxD*sin(degr(fr_XZ[i]))*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])))));
                    azimuth[i] = asin(thisdegree(arusD[i]/(ImaxD*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])*sin(degr(rolly[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = asin(thisdegree((-1)*arusD[i]/(ImaxD*sin(degr(azimuth[i])*cos(degr(yawy[i]))))));
                    opsi[i] = 13;
                }
                else if (arusD[i] < arusC[i])
                {
                    rolly[i] = asin(thisdegree(arusC[i]/((-1)*ImaxC*sin(degr(fr_XZ[i]))*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])))));
                    azimuth[i] = asin(thisdegree(arusC[i]/((-1)*ImaxC*sin(degr(fr_YZ[i]))*cos(degr(yawy[i])*sin(degr(rolly[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = asin(thisdegree((-1)*arusC[i]/((-1)*ImaxC*sin(degr(azimuth[i])*cos(degr(yawy[i]))))));
                    opsi[i] = 14;
                }
            }
            else if (arusF[i] >= IF_init[i])
            {
                rolly[i] = 0;
                if (arusD[i] > arusC[i])
                {
                    yawy[i] = asin(thisdegree(arusD[i]/((-1)*ImaxD*cos(degr(fr_XZ[i]))*cos(degr(pitchy[i])))));
                    azimuth[i] = acos(thisdegree(arusD[i]/((-1)*ImaxD*sin(degr(yawy[i])*cos(degr(pitchy[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = rolly[i] * (-1);
                    opsi[i] = 15;
                }
                else if (arusD[i] < arusC[i])
                {
                    yawy[i] = asin(thisdegree(arusC[i]/(ImaxC*cos(degr(fr_XZ[i]))*cos(degr(pitchy[i])))));
                    azimuth[i] = acos(thisdegree(arusC[i]/(ImaxC*sin(degr(yawy[i])*cos(degr(pitchy[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = rolly[i] * (-1);
                    opsi[i] = 16;
                }
                else if (arusD[i] == arusC[i])
                {
                    yawy[i] = 0;
                    azimuth[i] = asin(thisdegree(arusF[i]/(ImaxF*sin(degr(fr_YZ[i]))*cos(degr(rolly[i])*cos(degr(pitchy[i]))))));
                    az[i] = azimuth[i] - 90;
                    elev[i] = asin(thisdegree((-1)*arusF[i]/(ImaxF*sin(degr(azimuth[i])*cos(degr(pitchy[i]))))));
                    opsi[i] = 17;
                }
             }
        }
        else if (arusD[i] == arusC[i])
        {
            pitchy[i] = 0;
            rolly[i] = 0;
            yawy[i] = 0;
            azimuth[i] = asin(thisdegree(arusF[i]/(ImaxF*sin(degr(fr_YZ[i]))*cos(degr(rolly[i])*cos(degr(pitchy[i]))))));
            az[i] = azimuth[i] - 90;
            elev[i] = asin(thisdegree(arusF[i]/(ImaxF*sin(degr(azimuth[i])*cos(degr(pitchy[i])))))) - 90;
            opsi[i] = 18;
        }
    }

      kirim_nodemcu = ","+String(kode_arusop)+","+String(rad_D[i])+","+String(rad_C[i])+","+String(rad_F[i])+","+String(rad_A[i]);

      comm_nodemcu.println(kirim_nodemcu);

      delay(1000);

      kirim_nodemcu = "";

      delay(1000);

      kirim_nodemcu = ","+String(suhuE[i])+","+String(suhuD[i])+","+String(suhuC[i])+","+String(suhuF[i])+","+String(suhuA[i])+
                     ","+String(rolly[i])+","+String(pitchy[i])+","+String(yawy[i])+","+String(rad_E[i]);

      comm_nodemcu.println(kirim_nodemcu);

      delay(1000);

      kirim_nodemcu = "";

      delay(1000);

      kirim_nodemcu = ","+String(arusE[i])+","+String(arusD[i])+","+String(arusC[i])+","+String(arusF[i])+","+String(arusA[i])+
                     ","+String(fr_XZ[i])+","+String(fr_YZ[i])+","+String(az[i])+","+String(elev[i]);

      comm_nodemcu.println(kirim_nodemcu);

      delay(1000);

      kirim_nodemcu = "";
                     
  }
}

void ukur_arusmax()
{
  for(int y = 0; y<3; y++)
  {
    digitalWrite(LED_Hijau,HIGH);
    delay(500);
    digitalWrite(LED_Hijau,LOW);
  } 

  for(int y = 0; y<10; y++)
  {
    digitalWrite(LED_Hijau,HIGH);
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Hijau,LOW);
    digitalWrite(LED_Merah,LOW);
  }

  for(int y = 0; y<5; y++)
  {
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Merah,LOW);
  }

    for(int p = 0; p<7; p++)
    {
      digitalWrite(LED_Merah,HIGH);
      for(int y = 0; y<10; y++)
      {
        Imax_E[y] = ina219_E.getCurrent_mA();
        ImaxE = max(ImaxE,Imax_E[y]);

        if(ImaxE < 0)
        {
          ImaxE = 0;
        }

        else if(ImaxE >= 0)
        {
          ;
        }

        delay(100);
      }
    }

  for(int y = 0; y<10; y++)
  {
    digitalWrite(LED_Hijau,HIGH);
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Hijau,LOW);
    digitalWrite(LED_Merah,LOW);
  }

  for(int y = 0; y<5; y++)
  {
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Merah,LOW);
  }

    for(int p = 0; p<7; p++)
    {
      digitalWrite(LED_Merah,HIGH);
      for(int y = 0; y<10; y++)
      {
        Imax_A[y] = ina219_A.getCurrent_mA();
        ImaxA = max(ImaxA,Imax_A[y]);

        if(ImaxA < 0)
        {
          ImaxA = 0;
        }

        else if(ImaxA >= 0)
        {
          ;
        }

        delay(100);
      }
    }

  for(int y = 0; y<10; y++)
  {
    digitalWrite(LED_Hijau,HIGH);
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Hijau,LOW);
    digitalWrite(LED_Merah,LOW);
  }

  for(int y = 0; y<5; y++)
  {
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Merah,LOW);
  }

    for(int p = 0; p<7; p++)
    {
      digitalWrite(LED_Merah,HIGH);
      for(int y = 0; y<10; y++)
      {
        Imax_F[y] = ina219_F.getCurrent_mA();
        ImaxF = max(ImaxF,Imax_F[y]);

        if(ImaxF < 0)
        {
          ImaxF = 0;
        }

        else if(ImaxF >= 0)
        {
          ;
        }

        delay(100);
      }
    }

  for(int y = 0; y<10; y++)
  {
    digitalWrite(LED_Hijau,HIGH);
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Hijau,LOW);
    digitalWrite(LED_Merah,LOW);
  }

  for(int y = 0; y<5; y++)
  {
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Merah,LOW);
  }

    for(int p = 0; p<7; p++)
    {
      digitalWrite(LED_Merah,HIGH);
      for(int y = 0; y<10; y++)
      {
        Imax_D[y] = ina219_D.getCurrent_mA();
        ImaxD = max(ImaxD,Imax_D[y]);

        if(ImaxD < 0)
        {
          ImaxD = 0;
        }

        else if(ImaxD >= 0)
        {
          ;
        }

        delay(100);
      }
    }

  for(int y = 0; y<10; y++)
  {
    digitalWrite(LED_Hijau,HIGH);
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Hijau,LOW);
    digitalWrite(LED_Merah,LOW);
  }

  for(int y = 0; y<5; y++)
  {
    digitalWrite(LED_Merah,HIGH);
    delay(500);
    digitalWrite(LED_Merah,LOW);
  }

    for(int p = 0; p<7; p++)
    {
      digitalWrite(LED_Merah,HIGH);
      for(int y = 0; y<10; y++)
      {
        Imax_C[y] = ina219_C.getCurrent_mA();
        ImaxC = max(ImaxC,Imax_C[y]);

        if(ImaxC < 0)
        {
          ImaxC = 0;
        }

        else if(ImaxC >= 0)
        {
          ;
        }

        delay(100);
      }
    }

      kirim_nodemcu = ","+String(kode_arusmax)+","+String(ImaxE)+","+String(ImaxD)+","+String(ImaxC)+","+String(ImaxF)+","+String(ImaxA);

      comm_nodemcu.println(kirim_nodemcu);
}
