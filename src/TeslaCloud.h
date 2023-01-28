#ifndef TeslaCloud_h
#define TeslaCloud_h
#include <ArduinoJson.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>    // esp8266
#elif ESP32
  #include <WiFi.h>           // esp32
#else 
  #include <SPI.h>            //Arduino
  #include <Ethernet.h>
#endif

#if defined(ESP8266) || defined(ESP32)
    #include "FS.h"
    #include "FileManager.h"
   
    #include <GyverNTP.h>
    #include <UnixTime.h>

    struct TimeServerConfig{
      int8_t timezone;
      uint8_t ntpserver;
    };
    struct DebugConfig{
      uint8_t usedebugfile;
      uint8_t storageperiod;
    };
    struct ScreenConfig{
      uint8_t usescreenfiles;
      char mainscreen[20];
    };
    struct WiFiConfig{
      char ssid[25];
      char pass[25];
    };
#endif
struct TeslaCloudConfig{
  char username[25];
  char password[25];
  uint8_t devicenumber=0;
};
enum SendMode{
    EXCHANGE,
    SCREENSENDING,
    HISTORYDATASENDING
};
enum connectionState{
  DISCONNECTED,
  NOTCONNECTED,
  CONNECTING,
  CONNECTED,
  AUTHORIZED
};
enum StatusCode{
  RESPONSE_OK=1,
  RESPONSE_BAD=0
};
#include "TeslaCloudDebug.h"
#include "Settings.h"
#include <Array.h>
#include "Tag.h"
#include "TagCloud.h"
#include "Request.h"

#include <mString.h>
#include <TimerMs.h>
#include "Request.h"
class TeslaCloud{
public:
  TeslaCloud(){
      
  }
  TeslaCloud(const char* username, const char* userpassword, uint8_t deviceid=0,
              const char *host="cloud.teslascada.com", uint16_t port = 7002){
    
    strcpy(cloudconfig.username, username);
    strcpy(cloudconfig.password, userpassword);
    cloudconfig.devicenumber = deviceid;
    this->host = host;
    this->port = port;
  }

 void connect(){ 
    requests.clear();
    state =  NOTCONNECTED;

    if (connectToLocalNetwork())
         connectToTeslaCloud();
} 
#if defined(ESP8266) || defined(ESP32)
  void connect(const char* ssid, const char *password){
      strcpy(wificonfig.ssid, ssid);
      strcpy(wificonfig.pass, password);
      connect();
  }
  void setWiFiConfig(WiFiConfig wificonfig){
    this->wificonfig=wificonfig;
  }
  WiFiConfig getWiFiConfig(){
    return wificonfig;
  }
  String getErrorMessage(){
    return errorMessage;
  } 
  void setDebugFileMessage(String msg){
    if (!debugconfig.usedebugfile) return;
    if (_fs)
      DebugFileMessage(msg, ntp.year(), ntp.month(), ntp.day(), _fs);
  }
   void setFS(fs::FS *useFS) {
        _fs = useFS;
  }
  fs::FS * getFS(){
    return _fs;
  }
  void setTimeServerConfig(TimeServerConfig config){
    timeserver = config;
  
    setGMT(timeserver.timezone);
    switch(timeserver.ntpserver){
       case 0: setTimeServerHost("pool.ntp.org");
            break;
        case 1: setTimeServerHost("ntp1.stratum2.ru");
            break;
        case 2: setTimeServerHost("ntp2.stratum2.ru");
            break;
        case 3: setTimeServerHost("ntp.msk-ix.ru");
            break;
    }
  }
  void setDebugConfig(DebugConfig config){
        debugconfig = config;
  }
  void setScreenConfig(ScreenConfig config){
      screenconfig = config;
  }
  String getDebugMessage(){
    return debugmsg;
  }
  void setDebugMessage(String msg){
    debugmsg = msg;  
  }
   String getDebugMessage2(){
    return debugmsg2;
  }
  void setDebugMessage2(String msg){
    debugmsg2 = msg;
  }
 void setErrorMessage(String msg){
    errorMessage = msg;
  }
   void addTag(Tag tag){
    TagCloud tagcloud;
    tagcloud.usevalue = false;
    addTag(tag, tagcloud);   
  }
  void addTag(Tag tag, TagCloud tagcloud){
    
    tag.tagvalue = tag.readFromDevice();
    tags.push_back(tag);
    tagclouds.push_back(tagcloud);    
  }
#else
  void connect(IPAddress local,const byte mac[]=NULL){
            requests.clear();
            state =  NOTCONNECTED;
            SetMacAddress(cloudconfig.username, mac);
            LOG("Using static IP");
             Ethernet.begin(this->macAddress, local);
            delay(1000);
            LOG(Ethernet.localIP());
            state=CONNECTING;
            connectToTeslaCloud();
        }
    void connect(IPAddress local, IPAddress dns, IPAddress gateway, IPAddress subnet, const byte mac[]=NULL){
             requests.clear();
            state =  NOTCONNECTED;
            SetMacAddress(cloudconfig.username, mac);
            LOG("Using static IP");
            Ethernet.begin(this->macAddress, local, dns, gateway, subnet);
            delay(1000);
            LOG(Ethernet.localIP());
             state=CONNECTING;
            
        }
      void addTag(Tag tag){
         tag.tagvalue = tag.readFromDevice();
         tags.push_back(tag);  
        }        
#endif
void setTimerPeriod(int period){
    if (period<1000) period=1000;
    requesttimer.setTime(period);
  }
char* readValue(const char* name){
    for (int i=0; i<tags.size();i++){   
        if (tags[i].name==name)
        return tags[i].tagvalue.buf;

      }
      //char buf[1]='0';
      char c = '0';
      return &c;
  }
 
  void writeValue(const char* name, int value){
    
      for (int i=0; i<tags.size();i++){
        if (tags[i].name==name){
          tags[i].readValue(value);
          break;
        }
      }
  }
  void writeValue(const char* name, float value){
   
      for (int i=0; i<tags.size();i++){   
        if (tags[i].name==name){
          tags[i].readValue(value);
          break;
        }
      }
  }
  void writeValue(const char* name, bool value){
     for (int i=0; i<tags.size();i++){   
        if (tags[i].name==name){
          tags[i].readValue(value);
          break;
        }
      }
  }
  void writeValue(Tag tag, int value){
      writeValue(tag.name, value);
  }
  void writeValue(Tag tag, float value){    
      writeValue(tag.name, value);
  }
  void writeValue(Tag tag, bool value){
    writeValue(tag.name, value);
  }
  void setHost(const char* host){
    this->host= host;
  }
  void setPort(uint16_t port){
    this->port = port;
  }
  void setTeslaCloudConfig(TeslaCloudConfig cloudconfig){
    this->cloudconfig = cloudconfig;
  }
  TeslaCloudConfig getTeslaCloudConfig(){
    return this->cloudconfig;
  }
  uint8_t getTagMaxCount(){
    return TAG_COUNT_MAX;
  }  
  void clearTags(){
    tags.clear();
    #if defined(ESP8266) || defined(ESP32)
      tagclouds.clear();
    #endif
  }

bool run(){
    if (state== NOTCONNECTED) return false;
    #if defined(ESP8266) || defined(ESP32)
    if (tick())
        checkStoragePeriods();
    #endif
     if (keepalivetimer.tick()){
       // LOG("keep");
        keepalive();
     } 
      readClient();  
      if (requesttimer.tick()) {
        checkTagsUpdate();
        this->send();
        return true;
      }
      return false;
  }

protected:

void clientwrite(char strrequest[REQUEST_SIZE]){
  client.write(strrequest);
}
void connectToTeslaCloud(){
    if (state== NOTCONNECTED)
      return;
    lastresponsetime = millis();
    #if defined(ESP8266) || defined(ESP32)
      ntp.begin();
      ntp.updateNow();
      LOG2(ntp.timeString(),":connectToTeslaCloud");
    #else
      LOG("connectToTeslaCloud");
    #endif
    client.connect(host, port);
    delay(1000);   
    if (client.connected()){       
      state = CONNECTED; 
      #ifdef ESP8266 
        client.setSync(true);
      #endif
      if (token.length()==0)
        authToCloud();
      else{ 
         state = AUTHORIZED;
        subscribeTags();
      }    
    }
  }
 #if defined(ESP8266) || defined(ESP32)
 bool synced(){
    return ntp.synced();
  }
void updateNow(){
    ntp.updateNow();
  }
String timeString(){
    return ntp.timeString();
  }
String dateString(){
    return ntp.dateString();
  }
void setGMT(int8_t timezone){
    ntp.setGMT(timezone);
  }
void setTimeServerHost(const char* host){
    ntp.setHost(host);
  }
bool tick(){
    return ntp.tick();
  }

void writeHistory(Tag tag){
    if (_fs)
     writeFileHistory(tag, ntp.unix(), ntp.year(), ntp.month(), ntp.day(), _fs);
  }
void checkStoragePeriods(){
    if (_fs)
     checkFilesStoragePeriods(tags, timeserver.timezone,debugconfig.usedebugfile, debugconfig.storageperiod,ntp.unix(), _fs);
  }
void handleGetHistoryData(StaticJsonDocument<RESPONSE_SIZE> doc){
  if (_fs){
    LOG("handleGetHistoryData");
    historytagname = doc["b"]["name"].as<String>();
    String begindatestr = doc["b"]["begindate"];
    String enddatestr = doc["b"]["enddate"];
    begindate = begindatestr.substring(0,10).toInt();
    enddate = enddatestr.substring(0,10).toInt();
    Request request(HISTORYDATA_COMMAND);     
    char strrequest[REQUEST_SIZE];
   char tagname[historytagname.length()+1];
   historytagname.toCharArray(tagname, historytagname.length()+1);
   generateGetHistoryFromDeviceMessage(request.id,  request.route,tagname);
    //generateJsonRequest(request.id,  request.route, token.buf, cloudconfig.devicenumber,  tagname, strrequest,false);
   // clientsend(strrequest);
    mode = HISTORYDATASENDING;
  }
  }
void handleGetScreenRequest(StaticJsonDocument<RESPONSE_SIZE> doc){
    if (_fs){
    LOG("handleGetScreenRequest");
        
      if (!screenconfig.usescreenfiles) return;
    
      String screen = doc["b"];
      String screenname = screen;
      if (screen=="" && strcmp(screenconfig.mainscreen, "")!=0)
        screenname = screenconfig.mainscreen;
      else if (screen=="" && strcmp(screenconfig.mainscreen, "")==0){
        File dir = _fs->open("/","r");
       while (true) {
         File file = dir.openNextFile();
         if (!file){
           break;
         }
           String filename = file.name();
           int index = filename.indexOf(".tsp2json");
          if (index>0){
              screenname = filename.substring(0,index);
               break;
            }
        }
    }
    if (screenname=="") return;
    String screenfilepath = "/"+screenname+".tsp2json";
    if (!_fs->exists(screenfilepath)) return;
     f = _fs->open(screenfilepath, "r");
    Request request(GETSCREEN_COMMAND);  
    generateGetScreenFromDeviceMessage(request.id, request.route, screen, f.size());
    mode = SCREENSENDING;
    }
    
  }
  void ScreenSending(){
    while(f.available()){
       char buf[SCREENFILE_SIZE];
      size_t size = f.readBytes(buf, SCREENFILE_SIZE);
      if (size<SCREENFILE_SIZE){
          char buf2[size+3];      
          strncpy(buf2, buf,size);
          buf2[size]='|';
          buf2[size+1]='|';
          buf2[size+2]='|';
          client.write(buf2);
      }
      else
          client.write(buf, size);
    }
    mode = EXCHANGE;
    f.close();
  }
  void HistoryDataSending(){
    if (_fs){
      mString<50> sum;
      sum="[";
      bool sent = false;
    for (long i = begindate;i<=enddate;i=i+60*60*24){
      UnixTime stamp(timeserver.timezone);
      stamp.getDateTime(i);
      String taghistorypath = "/history/"+historytagname+"/"+stamp.year+stamp.month+stamp.day+".txt";
      LOG(taghistorypath);
      if(!_fs->exists(taghistorypath)) continue;
      File historyfile = _fs->open(taghistorypath, "r");
      while (historyfile.available()){
        String datetimestr = historyfile.readStringUntil(';');
        String valuestr = historyfile.readStringUntil(';');
        if (valuestr=="nan") continue;
        long  datetime = datetimestr.toInt();
        float  value = valuestr.toDouble();

        if (datetime>enddate || datetime<begindate)
           continue;
        if (sum=="")
          sum=",";
        sum+= "{\"x\":";
        sum+=String(datetime);
        sum+="000,\"y\":";
        sum+=String(value);
        sum+="}  ";// whitespaces need to exclude mistakes in big file requests. Helps to exclude mistakes
        sent = true;

        client.write(sum.buf, sum.length());
        /*#ifdef ESP32        
        delay(3);
        #endif*/
        sum.clear();
      }
      historyfile.close();
    }
  
    if (!sent) //if no values sent we have to sent "[""
      client.write(sum.buf);
     
      client.write("]|||");
      mode = EXCHANGE;
    }
  }
 bool connectToLocalNetwork(){
    LOG("Connecting to");
    LOG(wificonfig.ssid);
    LOG(wificonfig.pass);
    TimerMs wifitimer;
    wifitimer.setTime(10000);
    wifitimer.setPeriodMode();
    wifitimer.start();
    WiFi.mode(WIFI_STA);
    WiFi.begin(wificonfig.ssid, wificonfig.pass);
    while (WiFi.status()!=WL_CONNECTED){
      delay(100);
      L(".");
      if (wifitimer.tick()){
          wifitimer.stop();
          this->errorMessage="Wrong Wi-Fi settings (SSID or password)";
          state = NOTCONNECTED;
          connectToAPWiFi();
          return false;
      }
    }
    LOG("");
    LOG("WiFi connected");
    LOG(WiFi.localIP());
    WEBLOG(WiFi.localIP().toString());
    state=CONNECTING;
    return true;
  }
  #else
  bool connectToLocalNetwork(){
      LOG("Getting IP...");
      SetMacAddress(cloudconfig.username,NULL);
      if (!Ethernet.begin(this->macAddress)){
                  LOG("DHCP Failed!");  
                  return false;        
            }
             delay(1000);    
      IPAddress myip = Ethernet.localIP();
      LOG(myip);
      state=CONNECTING;
      return true;
  }
  #endif

  
void readClient(){
if (!client.connected()){
       requests.clear();
      state = DISCONNECTED;
      connectToTeslaCloud();
      return;
  }
      mString<RESPONSE_SIZE> str;
      str="";  
     //int len=0;      
       while(client.available()){
            char ch = static_cast<char>(client.read());
            str+=ch;
        }
       //  DeserializationError error = deserializeJson(doc, client);
        // handleResponse(doc);
      if (str.length()!=0)
              handleResponse(str.buf);
        //}
    
   
}

  private:
  void sendReadConfirmationResponse(int id){
       Request request(READ_COMMAND);
       // strncpy(request.body,"",sizeof(request.body));
      request.id = id;
      addRequest(request);
  }
  void handleSignResponse(StaticJsonDocument<RESPONSE_SIZE> doc){
    LOG("handleSignResponse");
    const char * t = doc["b"]["t"];
    token = t;
    cloudconfig.devicenumber=doc["b"]["n"];
    #if defined(ESP8266) || defined(ESP32)
        LOG4(String("handleSignResponse:"),String("[device:"),cloudconfig.devicenumber, String(";token:"));
        LOG(token.buf);
    #endif
    state = AUTHORIZED;
    subscribeTags();
  }
  void  handleReadRequest(StaticJsonDocument<RESPONSE_SIZE> doc){
    int id = doc["id"];
   // mString<TAGNAME_SIZE>
    const char* namec = doc["b"]["n"];
    mString<TAGNAME_SIZE> name;
    name = namec;
    const char* valuec = doc["b"]["v"];
    mString<TAGVALUE_SIZE> value;
    value = valuec;
     for (int i=0; i<tags.size();i++){
        if (tags[i].name==name){   
          tags[i].write(value);
          sendReadConfirmationResponse(id);
          break;
        }
      }
  }
void handleResponse(char* response){
   #if defined(ESP8266) || defined(ESP32)
    LOG3(dateString()+" "+timeString(),String(":RESPONSE:"),String(response));
  #else
    LOG(response);
  #endif
    lastresponsetime = millis();
    StaticJsonDocument<RESPONSE_SIZE> doc;
    deserializeJson(doc, response);
    int statuscode = doc["c"];
    int route = doc["r"];
    long id = doc["id"];

    #if defined(ESP8266) || defined(ESP32)
      WEBLOG2(timeString()+String(":RES(")+String(id)+String(");CMD=")+String(route)+String(";Code=")+String(statuscode));
      setDebugFileMessage(timeString()+String(":RES(")+String(id)+String(");CMD=")+String(route)+String(";Code=")+String(statuscode));
    #endif

    if (statuscode!=RESPONSE_OK) {
     #if defined(ESP8266) || defined(ESP32)
      setErrorMessage(doc["b"]["message"].as<String>());
    #endif
      removeRequest(id, route);
      client.flush();
      return;
    }
     if (route == READ_COMMAND)
         handleReadRequest(doc);
    #if defined(ESP8266) || defined(ESP32)
    else if (route == GETSCREEN_COMMAND)
        handleGetScreenRequest(doc);
    else if (route == HISTORYDATA_COMMAND)
        handleGetHistoryData(doc);
    #endif
      else{
            if (route == SIGN_COMMAND)  
               handleSignResponse(doc);
            
               removeRequest(id,route);
      }
    doc.clear(); 
  }
  void authToCloud(){
        LOG("authtocloud");
        Request request(SIGN_COMMAND);
        //generateDeviceRequest(cloudconfig.username, cloudconfig.password,request.body);
        requests.push_back(request);
        requesttimer.start();
        keepalivetimer.setTime(60000);
        keepalivetimer.start();
  }
   void subscribeTags(){
  
    for (int i=0; i<tags.size();i++){
        Request request(WRITE_COMMAND);
        request.tagid = i;
        #if defined(ESP8266) || defined(ESP32)
          request.tagcloudid = i;
        #endif
        //generateWriteRequestBody(tags[i],unix(),tagclouds[i], request.body);
        requests.push_back(request);
    }   
  }
   void clientsend(char strrequest[REQUEST_SIZE]){
    if (mode!=EXCHANGE) return;
    //Serial.println(strrequest);
    #if defined(ESP8266) || defined(ESP32)
    LOG3(dateString()+" "+timeString(),String(":REQUEST:"),String(strrequest));
    #else
    LOG(strrequest);
    #endif
    clientwrite(strrequest);
  }
private:

void checkTagsUpdate(){
 if (state!=AUTHORIZED) return;
 for (int i=0; i<tags.size();i++){   
      tags[i].run();
      if (tags[i].update){
             writeTag(i);
            #if defined(ESP8266) || defined(ESP32)
             if (tags[i].history)
               writeHistory(tags[i]);
              #endif
            tags[i].update = false;
      }
    }
}

void addRequest(Request request){
   if (requests.size()>=REQUEST_COUNT_MAX)
        requests.remove(0);
    requests.push_back(request);
}
void writeTag(uint8_t tagid){
  
  
    removeRequestWithTagID(tagid);
  
    Request request(WRITE_COMMAND);
    //request.tagname = tag.name;
    request.tagid = tagid;
     #if defined(ESP8266) || defined(ESP32)
      request.tagcloudid = -1;
    #endif
   // generateShortWriteRequestBody(tag,unix(),request.body);
    addRequest(request);
  }
  void removeRequestWithTagID(uint8_t tagid){
    int i;
    for (i=0; i<this->requests.size();i++)
        if (requests[i].route==WRITE_COMMAND && requests[i].tagid==tagid){
          #if defined(ESP8266) || defined(ESP32)
            if (requests[i].tagcloudid>=0) return;
          #endif
          break;
        }
    if (i<this->requests.size())
        this->requests.remove(i);
  }
 void keepalive(){
       // LOG("keepalive");
        Request request(KEEPALIVE_COMMAND);
        //strncpy(request.body,"", sizeof(request.body));
        addRequest(request);
  }
void removeRequest(int id, int route){
    //LOG4(String("removeRequest:id="),String(id),String("; route="), String(route));
    int i;
    for (i=0; i<this->requests.size();i++)
      if ((this->requests[i].id==id)&& (route==requests[i].route))
         break;
    this->requests.remove(i);
  }
  
 
   void send(){
    #if defined(ESP8266) || defined(ESP32) 
    if (!synced())
        updateNow();    
    if (mode == SCREENSENDING) ScreenSending();
    if (mode == HISTORYDATASENDING) HistoryDataSending();
    #endif
    if (millis()-lastresponsetime>RESPONSE_TIME){
      client.stop();
      return;
    }
     for (int i=0; i<this->requests.size();i++){
      if (millis()-requests[i].requesttime>REQUEST_TIME_REPEAT){
       #if defined(ESP8266) || defined(ESP32)
        WEBLOG(timeString()+String(":REQ:(")+String(requests[i].id)+");CMD="+String(requests[i].route));
        setDebugFileMessage(timeString()+String(":REQ:(")+String(requests[i].id)+");CMD="+String(requests[i].route));
        #endif
        bool sign = false;
        if(requests[i].route==SIGN_COMMAND){
         generateDeviceRequest(requests[i].id, SIGN_COMMAND);
           sign = true;
        }
        else if (requests[i].route==WRITE_COMMAND){
                           #if defined(ESP8266) || defined(ESP32)
                             if (requests[i].tagcloudid<0)
                                generateShortWriteRequestBody(requests[i].id, WRITE_COMMAND,tags[requests[i].tagid],ntp.unix());
                              else
                                 generateWriteRequestBody(requests[i].id, WRITE_COMMAND,tags[requests[i].tagid],ntp.unix(), tagclouds[requests[i].tagcloudid]);
                            #else
                              generateShortWriteRequestBody(requests[i].id, WRITE_COMMAND,tags[requests[i].tagid],0);
                            #endif 

        } 
        else if (requests[i].route==KEEPALIVE_COMMAND || requests[i].route==READ_COMMAND)
             generateReadORKeepAliveMessage(requests[i].id, requests[i].route);
        if (requests[i].route==READ_COMMAND || requests[i].route == KEEPALIVE_COMMAND)
            removeRequest(requests[i].id,requests[i].route);
        else
          requests[i].requesttime=millis();
        break;
      }
    }
  }
void generateDeviceRequest(uint32_t id, int route){
      StaticJsonDocument<REQUEST_SIZE> doc;
      doc["b"]["username"] = cloudconfig.username;
      doc["b"]["password"]= cloudconfig.password;
      generateHeaderAndSend(doc, id, route);
  }
  #if defined(ESP8266) || defined(ESP32)
  void generateWriteRequestBody(uint32_t id, int route,Tag tag, uint32_t time, TagCloud tagcloud){
    StaticJsonDocument<SUBSCRIBE_SIZE> doc;
    doc["id"] = id;
    doc["r"] = route;
    doc["t"] = token.buf;        
    doc["d"] = cloudconfig.devicenumber;
    doc["b"]["n"]=tag.name.buf;
    doc["b"]["v"]=tag.tagvalue.buf;
    doc["b"]["dt"]=time;
    if (tagcloud.usevalue){
      if (tag.history)
      doc["b"]["h"]=tag.history;
      doc["b"]["u"]=tagcloud.usevalue;
      if (tagcloud.unit!="")
          doc["b"]["un"]=tagcloud.unit;
      doc["b"]["t"]=tagcloud.viewtype;
      if (tagcloud.icon!="")
          doc["b"]["i"]=tagcloud.icon;
      if (tagcloud.description!="")
        doc["b"]["d"]=tagcloud.description;
      if (tagcloud.maxvalue!=tagcloud.minvalue){
          doc["b"]["mi"]=tagcloud.minvalue;
          doc["b"]["ma"]=tagcloud.maxvalue;
      }  
      if (tagcloud.color1.color<=0xFFFFFF){
        doc["b"]["c"][0]["f"] =tagcloud.color1.from;
        doc["b"]["c"][0]["t"] =tagcloud.color1.to;
        doc["b"]["c"][0]["c"] =tagcloud.color1.color;
      }
      if (tagcloud.color2.color<=0xFFFFFF){
        doc["b"]["c"][1]["f"] =tagcloud.color2.from;
        doc["b"]["c"][1]["t"] =tagcloud.color2.to;
        doc["b"]["c"][1]["c"] =tagcloud.color2.color;
      }
      if (tagcloud.bgcolor1.color<=0xFFFFFF){
        doc["b"]["bg"][0]["f"] =tagcloud.bgcolor1.from;
        doc["b"]["bg"][0]["t"] =tagcloud.bgcolor1.to;
        doc["b"]["bg"][0]["c"] =tagcloud.bgcolor1.color;
      }
      if (tagcloud.bgcolor2.color<=0xFFFFFF){
        doc["b"]["bg"][1]["f"] =tagcloud.bgcolor2.from;
        doc["b"]["bg"][1]["t"] =tagcloud.bgcolor2.to;
        doc["b"]["bg"][1]["c"] =tagcloud.bgcolor2.color;
      }
      
    }
    char buffer[SUBSCRIBE_SIZE];
    serializeJson(doc, buffer);
    clientsend(buffer);
    doc.clear();
  }
   void generateGetScreenFromDeviceMessage(uint32_t id, int route, String screen, long filesize){
    StaticJsonDocument<REQUEST_SIZE> doc;
   // generateJsonRequest(doc, id, route, false);
    doc["id"] = id;
    doc["r"] = route;
    doc["t"] = token.buf;        
    doc["d"] = cloudconfig.devicenumber;
    doc["b"]["s"]=screen;
    doc["b"]["l"]=filesize;
    char buffer[REQUEST_SIZE];
    serializeJson(doc, buffer);
    clientsend(buffer);
    doc.clear();
  }
  void generateGetHistoryFromDeviceMessage(uint32_t id, int route, const char* tagname){
    StaticJsonDocument<REQUEST_SIZE> doc;
   // generateJsonRequest(doc, id, route, false);
    doc["id"] = id;
    doc["r"] = route;
    doc["t"] = token.buf;        
    doc["d"] = cloudconfig.devicenumber;
    doc["b"]=tagname;
    char buffer[REQUEST_SIZE];
    serializeJson(doc, buffer);
    clientsend(buffer);
    doc.clear();
  }
  #endif
  void generateShortWriteRequestBody(uint32_t id, int route,Tag tag,uint32_t time){
    StaticJsonDocument<REQUEST_SIZE> doc;
    doc["b"]["n"]=tag.name.buf;
    doc["b"]["v"]=tag.tagvalue.buf;
    #if defined(ESP8266) || defined(ESP32)
      doc["b"]["dt"]=time;
    #endif
    generateHeaderAndSend(doc, id, route);
  }
  void generateReadORKeepAliveMessage(uint32_t id, int route){
    StaticJsonDocument<REQUEST_SIZE> doc;
    doc["b"]="";
    generateHeaderAndSend(doc, id, route);
  }
  void generateHeaderAndSend(StaticJsonDocument<REQUEST_SIZE> doc, uint32_t id, int route){
    doc["id"] = id;
    doc["r"] = route;
    doc["t"] = token.buf;        
    doc["d"] = cloudconfig.devicenumber;
    char buffer[REQUEST_SIZE];
    serializeJson(doc, buffer);
    clientsend(buffer);
    doc.clear();
  }
  enum connectionState state;
  TeslaCloudConfig cloudconfig;
  TimerMs requesttimer;
  TimerMs keepalivetimer;
  Array<Request, REQUEST_COUNT_MAX> requests;
  Array<Tag, TAG_COUNT_MAX> tags;
  #if defined(ESP8266) || defined(ESP32)
    Array<TagCloud, TAG_COUNT_MAX> tagclouds;
  #endif

  const uint8_t SIGN_COMMAND = 0;
  const uint8_t WRITE_COMMAND = 1;
  const uint8_t READ_COMMAND = 2;
  const uint8_t KEEPALIVE_COMMAND=3;
  const uint8_t GETSCREEN_COMMAND=4;
  const uint8_t HISTORYDATA_COMMAND=5;
  const char* host;
  uint16_t port;
  mString<30> token;
  SendMode mode =EXCHANGE;
  long lastresponsetime;

#if defined(ESP8266) || defined(ESP32)
  void  connectToAPWiFi(){
      LOG("");
      LOG("WiFi AP start");
      IPAddress apIP(SP_AP_IP);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, subnet);
      WiFi.softAP(SP_AP_NAME);
  }
 
  WiFiConfig wificonfig;
  WiFiClient client;
  TimeServerConfig timeserver;
  DebugConfig debugconfig;
  ScreenConfig screenconfig;
  File f;
  fs::FS *_fs = nullptr;

  String historytagname = "";
  long begindate = 0;
  long enddate = 0; 
  GyverNTP ntp;
  String debugmsg="";
  String debugmsg2="";
  String errorMessage="";
#else
   void SetMacAddress(const char* username, const byte mac[])
    {
        if (mac != NULL) {
          for (int i = 0; i < 6; i++) 
             macAddress[i] = mac[i];
    
        }

        macAddress[0] = 0xFE;
        macAddress[1] = 0xED;
        macAddress[2] = 0xBA;
        macAddress[3] = 0xFE;
        macAddress[4] = 0xFE;
        macAddress[5] = 0xED;

        int len = strlen(username);
        int mac_index = 1;
        for (int i=0; i<len; i++) {
            macAddress[mac_index++] ^= username[i];
            if (mac_index > 5) { mac_index = 1; }
        }  
    }
    byte macAddress[6];
    EthernetClient client;
  #endif
 
};

#endif