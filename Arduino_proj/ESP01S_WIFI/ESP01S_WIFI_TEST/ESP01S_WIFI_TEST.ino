/*
 Configure ESP8266 as TCP Server
*/

#include <ESP8266WiFi.h>

const char* ssid = "AAAAAA_TEST";
const char* password = "adminadmin";
#define MAX_SRV_CLIENTS 1 //最多多少个客户端可以链接
String tcp_str = "";      //接收到的信息

// 创建一个服务器实例
// 指定要监听的端口作为参数
WiFiServer server(80); //创建server
WiFiClient serverClients[MAX_SRV_CLIENTS]; //定义多少个client可以链接

void setup() {
//  Serial.begin(115200);
//  delay(20);

//  delay(20);
  WiFi.mode(WIFI_AP);
  IPAddress softLocal(192,168,7,1);
  IPAddress softGateway(192,168,7,1);
  IPAddress softSubnet(255,255,255,0);
  WiFi.softAPConfig(softLocal,softGateway,softSubnet);
  WiFi.softAP(ssid,password);

  // 启动服务器
  server.begin();
  server.setNoDelay(true); // 关闭小包合并包功能，不会延时发送数据
//  Serial.println("Server started!");

  // prepare GPIOs, set the GPIO mode
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT); //ESP8266's TX
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT); //ESP8266's RX
  //Set All the GPIO Low
  digitalWrite(0, 0); // Set low
  digitalWrite(1, 0); // Set low
  digitalWrite(2, 0); // Set low
  digitalWrite(3, 0); // Set low
}

void loop() {   
    TCP_Server();                           //循环运行服务器处理
}

void TCP_Server()
{
   uint8_t i;
   if(server.hasClient()){                 //判断当前是否有客户端
      for(i = 0; i < MAX_SRV_CLIENTS; i++){
        //查找空闲或者断开连接的客户端，并置为可用
        if(!serverClients[i] || !serverClients[i].connected()){ //如果数组中的某一项没有连接
          if(serverClients[i]) { //如果有空闲连接，则断开连接
            serverClients[i].stop();
          }
          serverClients[i] = server.available(); //加入连接的客户端
//          Serial.print("New client: ");
//          Serial.println(i);
          continue;
        }
      }
    }
    //检查客户端的数据
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if(serverClients[i] && serverClients[i].connected()){
        if(serverClients[i].available()){
          //从客户端获取数据，并推送到URAT端口
          tcp_str ="";
          while(serverClients[i].available()) 
          {
            tcp_str += (char)serverClients[i].read();
          }
//          Serial.println(tcp_str); //打印到串口
          if(tcp_str=="A"){ //GPIO 控制
            digitalWrite(0, 1);
          } else if(tcp_str=="B") {
            digitalWrite(1, 1);
          } else if(tcp_str=="C") {
            digitalWrite(2, 1);
          } else if(tcp_str=="D") {
            digitalWrite(3, 1);
          } else if(tcp_str=="E") {
            digitalWrite(0, 0);
          } else if(tcp_str=="F") {
            digitalWrite(1, 0);
          } else if(tcp_str=="G") {
            digitalWrite(2, 0);
          } else if(tcp_str=="H") {
            digitalWrite(3, 0);
          } else {
            // do nothing
          }
        }
      }
    }
}
