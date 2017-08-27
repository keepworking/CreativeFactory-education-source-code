#include <ESP8266WiFi.h>
WiFiClient client;
IPAddress server(*,*,*,*);

void setup(){
  Serial.begin(9600);

}

int value;

void loop(){
  value = 20;
  //임의의 값을 넣어둔다. 추후 변수형을 바꾸어서 미세먼지의 값을 넣어 보도록 한다.
  Serial.println("start connect");
  int state = client.connect(server,80);
  if(!state){
    Serial.println("connect fail");
    return;
  }
  client.print("GET /lssm/up.php?");
  client.print("name=dust&");
  client.print(value,10);
  //변수가 소수일 경우 소숫점 아래 10자리까지 표기하도록 함.
  client.print(" HTTP/1.1\r\n");
  client.print("Host: *.*.*.*\r\n");
  client.print("Connection: close\r\n\r\n");
  delay(100);//서버 응답을 잠시 기다린다.
  while(client.available()){
    Serial.write(client.read());
  }
  Serial.println("-------");
  Serial.println("connection end");
  delay(1000);
}
