#pragma once
#include "core.h"
#include "Moduls.h"
class TdCloud : public Tdlib
{
public:
    TDCLOUD_API  TdCloud();
    TDCLOUD_API  ~TdCloud();
public:
    TDCLOUD_API void GetUpdate();
    TDCLOUD_API void Login();
    TDCLOUD_API void SetBlockSize(int block);
    TDCLOUD_API int GetBlockSize();
    TDCLOUD_API void CreateChannel();
    TDCLOUD_API void GetNameChannel();
    TDCLOUD_API void ChannelInit();
    TDCLOUD_API TdMedia *DownloadManager(int loc_id, bool mini);
    TDCLOUD_API void RenderMedia();
    TDCLOUD_API void StatusDownload();
    TDCLOUD_API void DownloadBlock(int start, int sizeblock);
  
  HistoryChat hist;
  MenagerUDFile file;

protected:
  std::string channel_td = "TEST";
  int block=10;
  std::vector<TdMedia *> downloaded;
  std::vector<TdMedia *> download;
  std::string name_chennal;
  int64_t last_mess = 0;
  bool3 channel_b = -1;
  bool gethistory = false;
  int last=0;
};
