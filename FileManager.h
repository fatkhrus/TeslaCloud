#ifndef FileManager_h
#define FileManager_h
#include "FS.h"
#include "Settings.h"
#include <Array.h>
#include <mString.h>
#include "Tag.h"
#include <UnixTime.h>

void checkFilesStoragePeriods(Array<Tag, TAG_COUNT_MAX> &tags, uint8_t timezone, bool usedebugfile,uint8_t storageperiod, uint32_t unix, fs::FS *fs){
    
    for (int i=0; i<tags.size();i++){
      if (!tags[i].history) continue;
      int day =1;
      switch (tags[i].storagePeriod){
        case 0: day =1;
            break;
        case 1: day = 3;
            break;
        case 2: day = 7;
            break;
      }
      UnixTime stamp(timezone);
      stamp.getDateTime(unix-day*24*60*60);
      mString<50> taghistorypath;
      taghistorypath = "/history/";      
      taghistorypath +=tags[i].name.buf;
      taghistorypath +="/";
      taghistorypath = taghistorypath+stamp.year+stamp.month+stamp.day+".txt";
    bool exist = fs->exists(taghistorypath);
    if (exist)
        fs->remove(taghistorypath);
    }
    if (!usedebugfile) return;
      int day =1;
      switch (storageperiod){
        case 0: day =1;
            break;
        case 1: day = 3;
            break;
        case 2: day = 7;
            break;
      }
      UnixTime stamp(timezone);
      stamp.getDateTime(unix-day*24*60*60);
      mString<50> debugpath;
      debugpath = "/debug/";
      debugpath=debugpath+stamp.year+stamp.month+stamp.day+".txt";
     // LOG("checkTagsStoragePeriod");
      //LOG(debugpath.buf);
      bool exist = fs->exists(debugpath.buf);
      if (exist)
        fs->remove(debugpath.buf);
  }
   void createDirs(const char * path, fs::FS *fs){
  if (strchr(path, '/')) {
      Serial.printf("Create missing folders of: %s\r\n", path);
			char *pathStr = strdup(path);
			if (pathStr) {
				char *ptr = strchr(pathStr, '/');
				while (ptr) {
					*ptr = 0;
					fs->mkdir(pathStr);
					*ptr = '/';
					ptr = strchr(ptr+1, '/');
				}
			}
			free(pathStr);
		}
   }
  void DebugFileMessage(String msg, uint16_t year, uint8_t month, uint8_t day, fs::FS *fs){
    
    if (year<2023)
      return;
    mString<50> debugpath;
      debugpath = "/debug/";
      debugpath = debugpath+year+month+day+".txt";
    bool exist = fs->exists(debugpath.buf);
    #ifdef ESP32
    if (!exist)
      createDirs(debugpath.buf,fs);
    #endif
    File debugfile;
    if (exist)
        debugfile = fs->open(debugpath.buf, "a");
    else
        debugfile = fs->open(debugpath.buf, "w");
    debugfile.println(msg);
    debugfile.close();
  }

  void writeFileHistory(Tag tag, uint32_t unix, uint16_t year, uint8_t month, uint8_t day, fs::FS *fs){
  if (year<2023)
      return;
    mString<50> taghistorypath;
      taghistorypath = "/history/";
      //taghistorypath = "/";
      taghistorypath+=tag.name.buf;
      taghistorypath=taghistorypath+"/"+year+month+day+".txt";
    bool exist = fs->exists(taghistorypath.buf);
    #ifdef ESP32
    if (!exist)
      createDirs(taghistorypath.buf,fs);
    #endif
    File historyfile;

    if (exist){
        historyfile = fs->open(taghistorypath.buf, "a");
        historyfile.print(";");
    }
    else
        historyfile = fs->open(taghistorypath.buf, "w");
    float value = tag.tagvalue.toFloat(0);
    if (isnan(value))
      return;
    historyfile.print(unix);
    historyfile.print(';');
    historyfile.print(value);
    historyfile.close();
}

 
    




#endif