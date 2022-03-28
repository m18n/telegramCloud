#pragma once
#include "core.h"
#include "Moduls.h"

class TdCloud : public Tdlib
{
public:
  TdCloud()
  {
    td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
    client_manager_ = std::make_unique<td::ClientManager>();
    client_id_ = client_manager_->create_client_id();
    send_query(td_api::make_object<td_api::getOption>("use_quick_ack"), [this](Object object)
               { std::cout << to_string(object) << std::endl; });
    hist.SetTd(this);
    file.SetTd(this);
    file.SetHistoryChat(&hist);
    downloaded.resize(block);
    
  }

public:
  void GetUpdate();
  void Login();
  void SetBlockSize(int block){
    this->block=block;

    downloaded.resize(block);
  }
  int GetBlockSize(){
    return block;
  }
  void CreateChannel();
  void GetNameChannel();
  void ChannelInit();
  TdMedia *DownloadManager(int loc_id, bool mini);
  void RenderMedia()
{
  for(int i=0;i<download.size();i++){
    std::cout << "RENDER: " << download[i]->media.GetMediaTypeString() << " PATH: " << download[i]->media.GetPath() << "\n";
  }
}
void StatusDownload()
{
  bool down=true;
  while (down)
  {
    std::system("clear");
    for (int i = 0; i < downloaded.size(); i++)
    {
      if (downloaded[i] != NULL)
      {
        if (downloaded[i]->procDown() == 100)
        {
         
          download.push_back(downloaded[i]);
          
          downloaded[i] = NULL;
        }
        else
        {
          std::cout << "MED path: " << downloaded[i]->media.GetPath() << "  PROC DOWN: " << downloaded[i]->procDown() << "%\n";
        }
      }
    }
    
    for(int i=0;i<downloaded.size();i++){
      if(downloaded[i]==NULL){
        down=false;
      }
      else {
        down=true;
        break;
      }
    }
  }
  std::system("clear");
  RenderMedia();
}
  void DownloadBlock(int start,int sizeblock)
  {
    SetBlockSize(sizeblock);
    for (int i = start; i < start+block; i++)
    {
      TdMedia *td = DownloadManager(i, true);
      downloaded[i-start]=td;
    }
    StatusDownload();
  }
  
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
