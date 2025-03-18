#ifndef TeslaCloudMQTT_h
#define TeslaCloudMQTT_h

#include "TeslaCloud.h"
#include <MQTT.h>

class TeslaCloudMQTT: public TeslaCloud{
private:
	
	void subscribeTags(){
		LOG("subscribeTags");
		LOG(tags.size());
		for (int i=0; i<tags.size();i++){
			tags[i].run();
			subscribeTag(tags[i]);	
		}   
	}
	void subscribeTag(Tag tag){
		if (tag.mode==OUTPUT || tag.mode==100){
				mqttclient.subscribe(tag.name.buf);
		}
		publishTag(tag);
	}
	
public:
	TeslaCloudMQTT(){
      this->host ="cloud.teslascada.com";
	  this->port = 1883;
	}
	TeslaCloudMQTT(const char* username, const char* userpassword, uint8_t deviceid=0,
              const char *host="cloud.teslascada.com", uint16_t port = 1883):TeslaCloud(username, userpassword,deviceid,host,port){
			this->clientid = "clientid";
	}
	TeslaCloudMQTT(const char* id, const char* username, const char* userpassword, uint8_t deviceid=0,
              const char *host="cloud.teslascada.com", uint16_t port = 1883):TeslaCloud(username, userpassword,deviceid,host,port){
		this->clientid = id;
	}
	void setClientID(const char* id){
		this->clientid = id;
	}
	virtual void addTag(Tag tag){
		//tag.oldvalue = tag.readFromDevice();
		//tag.tagvalue = tag.oldvalue;
		//tag.update = false;	
		tags.push_back(tag);
		subscribeTag(tag);
	}
	virtual bool run(){
		if (state== NOTCONNECTED) return false;
		checkWifiAvailable();
		mqttclient.loop();
		delay(10);
		if (!mqttclient.connected()){
			state== NOTCONNECTED;
			connect();
		}
		 if (requesttimer.tick()) {
			 checkTagsUpdate();
			 return true;
		 }
		return false;
	}
	void subscribe(const char *topic){
		mqttclient.subscribe(topic);
	}
	
protected:
	MQTTClient mqttclient;
	const char* clientid;
	void messageReceived(String &topic, String &payload){
		LOG3(timeString(),topic,payload);
		mString<TAGNAME_SIZE> name;
		name = topic;
		mString<TAGVALUE_SIZE> value;
		value = payload;
		for (int i=0; i<tags.size();i++){
        if (tags[i].name==name){   
          tags[i].write(value);
          break;
        }
      }
	}
	virtual void connectToTeslaCloud(){
		LOG("TeslaCloudMQTT:connectToTeslaCloud");
		NTP.begin();
		NTP.updateNow();
		LOG2(NTP.toString(),":connectToTeslaCloud");
		LOG(clientid);
		int len1 = strlen(clientid);
		char *result = new char[len1+11];
		strcpy(result, clientid);
		strcat(result, "_device");
		LOG(result);
		if (state== NOTCONNECTED) return;
		mqttclient.begin(host, this->client);
		mqttclient.setKeepAlive(60);
		while (!mqttclient.connect(result, cloudconfig.username, cloudconfig.password)){
			LOG(".");
			delay(1000);
		}
		mqttclient.onMessage([this](String &topic, String &payload){
			this->messageReceived(topic, payload);
		});
		subscribeTags();
		requesttimer.start();
		state = AUTHORIZED;
	}
	virtual void writeTag(uint8_t tagid){
		Tag tag = tags[tagid];
		publishTag(tag);
	}
	void publishTag(Tag tag){
		LOG("writeTag");
		LOG(tag.name.buf);
		LOG(tag.tagvalue.buf);
		mqttclient.publish(tag.name.buf, tag.tagvalue.buf);
	}
	
};

#endif