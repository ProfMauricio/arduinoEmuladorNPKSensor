#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial portaComm(8,9);

int pinPortaDE_RE = 7;

byte InquiryFrame[8];
byte respostaNigrogenio[7] = {0x01, 0x03, 0x02, 0x00, 0x20, 0x5A, 0x3D};

void SimularResposta( )
{
    if( (InquiryFrame[0] != 0x01) || (InquiryFrame[1] != 0x03))
    {
      Serial.println("Falha no pacote");
      return;
    } 
    if ( (InquiryFrame[2] == 0x00) && (InquiryFrame[3] == 0x1E) )
    {
      Serial.println("Solicitou Info Nitrogênio");
      if ( (InquiryFrame[4] != 0x00) && (InquiryFrame[5] != 0x01))
      {
          Serial.println("Falha no pacote no Register Lenght");
          return;
      }
      if ( InquiryFrame[6] != 0xb5 )
        { 
          Serial.println("Falha no pacote no Register CRC_L");
          return;
      }      

      if ( InquiryFrame[7] != 0xcc )
      { 
          Serial.println("Falha no pacote no Register CRC_H");
          return;
      }

      Serial.println("Enviando nitrogenio com 32 mg/kg");
      digitalWrite(pinPortaDE_RE, HIGH);
      if ( portaComm.write(respostaNigrogenio, sizeof(respostaNigrogenio))== 7 )
      {
          digitalWrite(pinPortaDE_RE, LOW);
      }
      Serial.println("Enviada nitrogenio");
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(pinPortaDE_RE, OUTPUT);
  digitalWrite(pinPortaDE_RE, LOW);
  Serial.begin(9600);
  portaComm.begin(9600);
  portaComm.setTimeout(100);
  Serial.println("Iniciando");
}

void loop() {
  
  // put your main code here, to run repeatedly:
  if (portaComm.available() > 0)
  {
      Serial.println("Chegando solicitação");
      for (int i = 0; i < 8; i++)
      {
        InquiryFrame[i] = portaComm.read();        
      }
      for (int i = 0; i < 8; i++)
      {
        /* code */
        Serial.print(InquiryFrame[i], HEX);
      }
      SimularResposta();

  }
}