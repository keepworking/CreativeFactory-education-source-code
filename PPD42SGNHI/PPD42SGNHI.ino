/*

보드: NodeMcu rev1.0

----------------------------------------------
핀배선
NODE MCU 1.0          PPD42 grove dustsensor
GPIO 16               SIGNALpin
GND                   GND
VIN(5v)               VCC

*/



#include <ESP8266WiFi.h>
#include <SGNHI_ESP8266_WIFI.h>
#include <ppd42.h>
#ifdef ESP8266
extern "c"{
  #include"user_interface.h"
}
#endif

PPD42 ppd(16,30000);
//권장 샘플링 주기가 30초 입니다.

dotori dustsensor("센서코드");

void setup(){
  wifi_set_sleep(NONE_SLEEP_T);
  Serial.begin(9600);
  dev.init("ID","장치코드");
  pinMode(16,INPUT);
}


float per = 0;
float pcs = 0;
float ugm3 = 0;
void loop(){
  per = ppd.read();
  pcs = per2pcs(per);
  ugm3 = pcs2ugm3(pcs);
  Serial.print(ugm3,10);
  Serial.println(" ugm3");
  //ugm3 센서값 출력
  dustsensor.set(ugm3);
  dev.send(dustsensor);
}

//퍼센트 값을 ugm3로 변환하는 값입니다.
double per2pcs(double per){
  return 1.1*pow(per,3) - 3.8*pow(per,2) + 520*per +0.62;
}

//pcs 단위를 ug/m^3 단위로 변환하는 함수 입니다.
double pcs2ugm3(double pcs){
  double den,r25,vol25,mass25;
  den=1.65*pow(10,12);
  r25=0.44*pow(10,-6);
  vol25=(4/3)*PI*pow(r25,3);
  mass25=den*vol25;
  double K = 3531.5;
  return pcs * mass25 * K;
}
