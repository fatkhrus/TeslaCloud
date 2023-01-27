#ifndef WebSettings_h
#define WebSettings_h
#include "TeslaCloud.h"// it should be before GyverPortal
#include <GyverPortal.h>
#include "EEPromManager.h"
#include "TeslaCloudDebug.h"


//GyverPortal ui;
GyverPortal ui;
TeslaCloud *teslacloud;
int selectedtag=0;
String olddebugmsg;
void build() {
  GP.BUILD_BEGIN(500);
  GP.THEME(GP_DARK);
  GP.PAGE_TITLE("TeslaCloud Settings");

  String list = "";
  for (int i=0;i<TAG_COUNT_MAX;i++){
      list+=String("en");
      list+=String(i);
      list+=String(",env");
      list+=String(i);
      list+=String(",hen");
      list+=String(i);
      if (i+1<TAG_COUNT_MAX)
        list+=",";
    }
    list+=",";
    list+="usedebug";
     list+=",";
    list+="usescreen";
  GP.UPDATE("err");
  GP.ALERT("err");
  GP.RELOAD_CLICK(list);
  GP.TITLE("TeslaCloud Settings");

  GP.FORM_BEGIN("/save");
  if (teslacloud->getFS())
    GP.NAV_TABS_LINKS("/,/tags,/files,/debug", "Connections,Tags,Files,Debug");
  else
     GP.NAV_TABS_LINKS("/,/tags,/debug", "Connections,Tags,Debug");
 if (ui.uri("/")) {
  GP_MAKE_BLOCK_TAB(
    "Connections",
    GP_MAKE_BLOCK_THIN_TAB(
      "WiFi Settings",
      GP_MAKE_BOX(GP.LABEL("SSID:");    GP.TEXT("lg", "SSID",wificonfig.ssid)         );
      GP_MAKE_BOX(GP.LABEL("Password:");GP.TEXT("ps", "WiFi Password",wificonfig.pass));
    );
    GP_MAKE_BLOCK_THIN_TAB(
      "TeslaCloud Settings",
      GP_MAKE_BOX(GP.LABEL("Username:");GP.TEXT("un", "Username", cloudconfig.username)     );
      GP_MAKE_BOX(GP.LABEL("Password:");GP.TEXT("up", "Password", cloudconfig.password));  
      GP_MAKE_BOX(GP.LABEL("Device:"); GP.SPINNER("dev",cloudconfig.devicenumber,1,255,1,0));
  );
   GP_MAKE_BLOCK_THIN_TAB(
      "Time server",
      GP_MAKE_BOX(GP.LABEL("GMT zone:"); GP.SPINNER("gmt",timeserver.timezone,-12,13,1,0)); 
      GP_MAKE_BOX(GP.LABEL("Server:"); GP.SELECT("ntps", "pool.ntp.org, ntp1.stratum2.ru, ntp2.stratum2.ru, ntp.msk-ix.ru", timeserver.ntpserver,0,0,0));
  );
  )
 }
else if (ui.uri("/tags")){
    String list= "";
    for (int i=0;i<TAG_COUNT_MAX;i++){
      list+=String("Tag");
      list+=String(i+1);
      if (i+1<TAG_COUNT_MAX)
        list+=",";
    }
    GP_MAKE_BLOCK(
        GP.LABEL("Select:");
        GP.SELECT("seltag",list,selectedtag,0,0,1);
        GP.SWITCH(String("en")+String(selectedtag),tagsettings[selectedtag].enable););

    if (tagsettings[selectedtag].enable){
        GP.BLOCK_BEGIN(GP_THIN,"","Settings");   
        GP_MAKE_BOX(GP.LABEL("Name:");    GP.TEXT(String("n")+String(selectedtag), "Name",tagsettings[selectedtag].name));
        GP_MAKE_BOX(GP.LABEL("Mode:"); GP.SELECT(String("m")+String(selectedtag), "VIRTUAL, INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN", tagsettings[selectedtag].mode,0,0,1));
        if (tagsettings[selectedtag].mode!=0){
          GP_MAKE_BOX(GP.LABEL("Pin:"); GP.SPINNER(String("p")+String(selectedtag),tagsettings[selectedtag].pin,0,50,1,0));
          GP_MAKE_BOX(GP.LABEL("Type:"); GP.SELECT(String("t")+String(selectedtag), "DIGITAL, ANALOG", tagsettings[selectedtag].pintype,0,0,1));
          if (tagsettings[selectedtag].pintype==1){
            GP_MAKE_BOX(GP.LABEL("Scale:");    GP.NUMBER_F(String("s")+String(selectedtag), "Scale",tagsettings[selectedtag].scale,5));
            GP_MAKE_BOX(GP.LABEL("Offset:");    GP.NUMBER_F(String("o")+String(selectedtag), "Offset",tagsettings[selectedtag].offset));  
          }     
        }
          if (tagsettings[selectedtag].pintype==1 || tagsettings[selectedtag].mode==0){
           GP_MAKE_BOX(GP.LABEL("Deadband:");    GP.NUMBER_F(String("d")+String(selectedtag), "Deadband",tagsettings[selectedtag].deadband));
           }
        GP.BLOCK_END();
        GP.BLOCK_BEGIN(GP_THIN,"","History"); 
        GP_MAKE_BOX(GP.LABEL("Enable:");GP.SWITCH(String("hen")+String(selectedtag),tagsettings[selectedtag].history));
        if (tagsettings[selectedtag].history){
          GP_MAKE_BOX(GP.LABEL("Storage period:"); GP.SELECT(String("stp")+String(selectedtag), "1 DAY, 3 DAYS, 7 DAYS", tagsettings[selectedtag].storageperiod,0,0,0));
        }
        GP.BLOCK_END();  
        GP.BLOCK_BEGIN(GP_THIN,"","Tag card");
        GP_MAKE_BOX(GP.LABEL("Enable:"); GP.SWITCH(String("env")+String(selectedtag),tagsettings[selectedtag].usevalue););
        if (tagsettings[selectedtag].usevalue){
           GP_MAKE_BOX(GP.LABEL("View type:"); GP.SELECT(String("vt")+String(selectedtag), "VALUE, CIRPROGRESS, PROGRESS, ICON, BUTTON, INPUT, SLIDER, KNOB",
               tagsettings[selectedtag].viewtype,0,0,1));
          GP_MAKE_BOX(GP.LABEL("Description:");GP.TEXT(String("de")+String(selectedtag), "Description", tagsettings[selectedtag].description)     );
          GP_MAKE_BOX(GP.LABEL("Unit:");GP.TEXT(String("u")+String(selectedtag), "Unit", tagsettings[selectedtag].unit)     );
          GP_MAKE_BOX(GP.LABEL("Icon:");GP.TEXT(String("i")+String(selectedtag), "Icon", tagsettings[selectedtag].icon)     );
          if (tagsettings[selectedtag].viewtype==1 || tagsettings[selectedtag].viewtype==2
               || tagsettings[selectedtag].viewtype==6 || tagsettings[selectedtag].viewtype==7   ){
                 GP_MAKE_BOX(GP.LABEL("Minimum:");    GP.NUMBER_F(String("min")+String(selectedtag), "Minimum",tagsettings[selectedtag].minvalue));
                 GP_MAKE_BOX(GP.LABEL("Maximum:");    GP.NUMBER_F(String("max")+String(selectedtag), "Maximum",tagsettings[selectedtag].maxvalue));
               }
           GP.HR();  
           GP.LABEL("Color 1");
           GP_MAKE_BOX(GP.LABEL("From:");    GP.NUMBER_F(String("from1")+String(selectedtag), "From",tagsettings[selectedtag].color1from));
           GP_MAKE_BOX(GP.LABEL("To:");    GP.NUMBER_F(String("to1")+String(selectedtag), "To",tagsettings[selectedtag].color1to));
           GP_MAKE_BOX(GP.LABEL("Color:");    GP.COLOR(String("col1")+String(selectedtag),tagsettings[selectedtag].color1));     
            GP.HR();  
           GP.LABEL("Color 2");
           GP_MAKE_BOX(GP.LABEL("From:");    GP.NUMBER_F(String("from2")+String(selectedtag), "From",tagsettings[selectedtag].color2from));
           GP_MAKE_BOX(GP.LABEL("To:");    GP.NUMBER_F(String("to2")+String(selectedtag), "To",tagsettings[selectedtag].color2to));
           GP_MAKE_BOX(GP.LABEL("Color:");    GP.COLOR(String("col2")+String(selectedtag),tagsettings[selectedtag].color2));   
            GP.HR();  
           GP.LABEL("Background color 1");
           GP_MAKE_BOX(GP.LABEL("From:");    GP.NUMBER_F(String("bgfrom1")+String(selectedtag), "From",tagsettings[selectedtag].bgcolor1from));
           GP_MAKE_BOX(GP.LABEL("To:");    GP.NUMBER_F(String("bgto1")+String(selectedtag), "To",tagsettings[selectedtag].bgcolor1to));
           GP_MAKE_BOX(GP.LABEL("Color:");    GP.COLOR(String("bgcol1")+String(selectedtag),tagsettings[selectedtag].bgcolor1));     
            GP.HR();  
           GP.LABEL("Background color 2");
           GP_MAKE_BOX(GP.LABEL("From:");    GP.NUMBER_F(String("bgfrom2")+String(selectedtag), "From",tagsettings[selectedtag].bgcolor2from));
           GP_MAKE_BOX(GP.LABEL("To:");    GP.NUMBER_F(String("bgto2")+String(selectedtag), "To",tagsettings[selectedtag].bgcolor2to));
           GP_MAKE_BOX(GP.LABEL("Color:");    GP.COLOR(String("bgcol2")+String(selectedtag),tagsettings[selectedtag].bgcolor2));  
        }
        GP.BLOCK_END();
    }

}
else if (ui.uri("/files")){
  GP.FILE_UPLOAD("file_upl"); // download button
   GP.FILE_UPLOAD("folder_upl");    // download button
  GP.FILE_MANAGER(teslacloud->getFS());    // file manager

   GP.BLOCK_BEGIN(GP_THIN,"","Screen file"); 
    GP_MAKE_BOX(GP.LABEL("Enable:");GP.SWITCH("usescreen",screenconfig.usescreenfiles));
    if (screenconfig.usescreenfiles){
    GP_MAKE_BOX(GP.LABEL("Main screen:");  GP.TEXT("mainscreen", "Main Screen",screenconfig.mainscreen));
    }
    GP.BLOCK_END();

   GP.BLOCK_BEGIN(GP_THIN,"","Debug file"); 
    GP_MAKE_BOX(GP.LABEL("Enable:");GP.SWITCH("usedebug",debugconfig.usedebugfile));
    if (debugconfig.usedebugfile){
    GP_MAKE_BOX(GP.LABEL("Storage period:"); GP.SELECT("stpdebug", "1 DAY, 3 DAYS, 7 DAYS",debugconfig.storageperiod,0,0,0));
    }
    GP.BLOCK_END();
   
}
else if (ui.uri("/debug")){
    
    GP.AREA_LOG(20); 
}

  GP.SUBMIT("Save");
  GP.FORM_END();

  GP.BUILD_END();
}
void action(GyverPortal& p) {
    if (ui.update()){
          if (teslacloud->getErrorMessage()!=""){
            ui.answer(teslacloud->getErrorMessage());
            teslacloud->setErrorMessage("");
          } 
          if (teslacloud->getDebugMessage()!=""){
            ui.log.println(teslacloud->getDebugMessage());
            teslacloud->setDebugMessage("");
          }      
          if (teslacloud->getDebugMessage2()!=""){
            ui.log.println(teslacloud->getDebugMessage2());
            teslacloud->setDebugMessage2("");
          }    
    }
    if (ui.click()){
      for (int i=0;i<TAG_COUNT_MAX;i++){
        String ennum = String("en")+String(i);
        String nnum = String("n")+String(i);
        String mnum = String("m")+String(i);
        String pnum = String("p")+String(i);
        String tnum = String("t")+String(i);
        String snum = String("s")+String(i);
        String onum = String("o")+String(i);
        String dnum = String("d")+String(i);
        String envnum = String("env")+String(i);
        String vtnum = String("vt")+String(i);
        String denum = String("de")+String(i);
        String unum = String("u")+String(i);
        String inum = String("i")+String(i);
        String minnum = String("min")+String(i);
        String maxnum = String("max")+String(i);
        String from1num = String("from1")+String(i);
        String to1num = String("to1")+String(i);
        String col1num = String("col1")+String(i);
        String from2num = String("from2")+String(i);
        String to2num = String("to2")+String(i);
        String col2num = String("col2")+String(i);

        String bgfrom1num = String("bgfrom1")+String(i);
        String bgto1num = String("bgto1")+String(i);
        String bgcol1num = String("bgcol1")+String(i);
        String bgfrom2num = String("bgfrom2")+String(i);
        String bgto2num = String("bgto2")+String(i);
        String bgcol2num = String("bgcol2")+String(i);

        String hen = String("hen")+String(i);
        String stp = String("stp")+String(i);
        if (ui.click(ennum))
         tagsettings[i].enable = ui.getBool(ennum);
          if (ui.click(nnum))
              ui.copyStr(nnum, tagsettings[i].name);
        if (ui.click(mnum))
         tagsettings[i].mode = ui.getInt(mnum);
         if (ui.click(pnum))
         tagsettings[i].pin = ui.getInt(pnum);
         if (ui.click(tnum))
         tagsettings[i].pintype = ui.getInt(tnum);
          if (ui.click(snum))
         tagsettings[i].scale = ui.getFloat(snum);
          if (ui.click(onum))
         tagsettings[i].offset = ui.getFloat(onum);
          if (ui.click(dnum))
         tagsettings[i].deadband = ui.getFloat(dnum);
         if (ui.click(envnum))
         tagsettings[i].usevalue= ui.getBool(envnum);
          if (ui.click(vtnum))
         tagsettings[i].viewtype= ui.getInt(vtnum);
         if (ui.click(denum))
            ui.copyStr(denum, tagsettings[i].description);
         if (ui.click(unum))
            ui.copyStr(unum, tagsettings[i].unit);
         if (ui.click(inum))
            ui.copyStr(inum, tagsettings[i].icon);
         if (ui.click(minnum))
           tagsettings[i].minvalue = ui.getFloat(minnum);
         if (ui.click(maxnum))
           tagsettings[i].maxvalue = ui.getFloat(maxnum);
         if (ui.click(from1num))
           tagsettings[i].color1from = ui.getFloat(from1num);
          if (ui.click(to1num))
           tagsettings[i].color1to = ui.getFloat(to1num);
          if (ui.click(col1num))
           tagsettings[i].color1 = ui.getColor(col1num).getHEX();
          if (ui.click(from2num))
           tagsettings[i].color2from = ui.getFloat(from2num);
          if (ui.click(to2num))
           tagsettings[i].color2to = ui.getFloat(to2num);
          if (ui.click(col2num))
           tagsettings[i].color2 = ui.getColor(col2num).getHEX();

            if (ui.click(bgfrom1num))
           tagsettings[i].bgcolor1from = ui.getFloat(bgfrom1num);
          if (ui.click(bgto1num))
           tagsettings[i].bgcolor1to = ui.getFloat(bgto1num);
          if (ui.click(bgcol1num))
           tagsettings[i].bgcolor1 = ui.getColor(bgcol1num).getHEX();
          if (ui.click(bgfrom2num))
           tagsettings[i].bgcolor2from = ui.getFloat(bgfrom2num);
          if (ui.click(bgto2num))
           tagsettings[i].bgcolor2to = ui.getFloat(bgto2num);
          if (ui.click(bgcol2num))
           tagsettings[i].bgcolor2 = ui.getColor(bgcol2num).getHEX();

          if (ui.click(hen))
             tagsettings[i].history = ui.getBool(hen);
          if (ui.click(stp))
              tagsettings[i].storageperiod = ui.getInt(stp);
         
      }
      if (ui.click("seltag"))
        selectedtag = ui.getInt("seltag");
      if (ui.click("lg"))
         ui.copyStr("lg", wificonfig.ssid); 
      if (ui.click("ps"))
         ui.copyStr("ps", wificonfig.pass);
      if (ui.click("un"))
        ui.copyStr("un",cloudconfig.username);
      if (ui.click("up"))
        ui.copyStr("up",cloudconfig.password);
      if (ui.click("dev"))
         cloudconfig.devicenumber = ui.getInt("dev");
      if (ui.click("gmt"))
         timeserver.timezone = ui.getInt("gmt");
      if (ui.click("ntps"))
         timeserver.ntpserver = ui.getInt("ntps");
      if (ui.click("usedebug"))
          debugconfig.usedebugfile=ui.getInt("usedebug");
      if (ui.click("stpdebug"))
          debugconfig.storageperiod=ui.getInt("stpdebug");
      if (ui.click("usescreen"))
          screenconfig.usescreenfiles=ui.getInt("usescreen");
      if (ui.click("mainscreen"))
          ui.copyStr("mainscreen",screenconfig.mainscreen);
    }
   if (p.form("/save")) {      
    saveConfigToEEPROM();
    fillConfig(teslacloud);
    teslacloud->connect();
  }
  if (ui.uploadEnd()) {
    Serial.print("Uploaded file: ");
    Serial.print(ui.fileName());
    Serial.print(", from: ");
    Serial.println(ui.uploadName());
  }

}
void webinit(){
  ui.start();
  ui.attachBuild(build);
  ui.attach(action);
  ui.log.start(REQUEST_SIZE);

}
void webtick(){
  ui.tick();
}

class WebSettings{
  public:
      void init(){
        webinit();
      }
      void tick(){
        webtick();
      }
      void setTeslaCloud(TeslaCloud *cloud){
        teslacloud = cloud;
        ui.setFS(teslacloud->getFS());        
        readConfig(teslacloud);
      }
};

#endif