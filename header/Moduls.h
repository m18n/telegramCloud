#pragma once

#include "core.h"
#include <fstream>
#include <string.h>
typedef char bool3;
namespace td_api = td::td_api;
struct messageHistory
{
  messageHistory(const messageHistory &x)
  {
  }
  messageHistory &operator=(const messageHistory &x)
  {
  }
  messageHistory()
  {
  }
  messageHistory(messageHistory &&x) noexcept
  {
    mess = std::move(x.mess);
    doc = std::move(x.doc);
    photo = std::move(x.photo);
    video = std::move(x.video);
  }
  td_api::object_ptr<td_api::message> mess = NULL;
  td_api::object_ptr<td_api::messageDocument> doc = NULL;
  td_api::object_ptr<td_api::messagePhoto> photo = NULL;
  td_api::object_ptr<td_api::messageVideo> video = NULL;
};
class messagemedia
{
private:
  td_api::file *GetFile_id(int file_id)
  {
    td_api::file *file = NULL;
    if (mini)
    {
      if (this->mess->doc)
      {
        file = &(*mess->doc->document_->thumbnail_->file_);
      }
      else if (this->mess->photo)
      {
        file = &(*mess->photo->photo_->sizes_[0]->photo_);
      }
      else if (this->mess->video)
      {
        file = &(*mess->video->video_->thumbnail_->file_);
      }
    }
    else
    {
      
      if (this->mess->doc)
      {
        file = &(*mess->doc->document_->document_);
      }
      else if (this->mess->photo)
      {
        int len=mess->photo->photo_->sizes_.size();
        file = &(*mess->photo->photo_->sizes_[len-1]->photo_);
      }
      else if (this->mess->video)
      {
        file = &(*mess->video->video_->video_);
      }
    }
    return file;
  }

public:
  messagemedia()
  {
  }
  bool Que(messagemedia &mes)
  {
    if (mes.GetFileID() == GetFileID())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  void SetMessageHistory(messageHistory *mess)
  {
    this->mess = mess;
  }
  std::string GetPath()
  {
    std::string path;
    if (mini)
    {
      if (mess->doc)
      {
        if(mess->doc->document_->thumbnail_)
          return mess->doc->document_->thumbnail_->file_->local_->path_;
      }
      else if (mess->photo)
      {
        return mess->photo->photo_->sizes_[0]->photo_->local_->path_;
      }
      else if (mess->video)
      {
        if(mess->video->video_->thumbnail_)
          return mess->video->video_->thumbnail_->file_->local_->path_;
      }
    }
    else
    {
      if (mess->doc)
      {
        return mess->doc->document_->document_->local_->path_;
      }
      else if (mess->photo)
      {
        int len=mess->photo->photo_->sizes_.size();
        return mess->photo->photo_->sizes_[len-1]->photo_->local_->path_;
      }
      else if (mess->video)
      {
        return mess->video->video_->video_->local_->path_;
      }
    }
    return "";
  }
  void Usage()
  {
    count_usage++;
  }
  int GetCountUsage()
  {
    return count_usage;
  }
  void SetSize(int file_id, int size)
  {
    td_api::file *file = GetFile_id(file_id);
    if (file)
    {
      file->local_->downloaded_size_ = size;
    }
  }
  void SetRemoteSize(int file_id, int size)
  {
    td_api::file *file = GetFile_id(file_id);
    if (file)
    {
      file->remote_->uploaded_size_ = size;
    }
  }
  void SetPath(int file_id,std::string path){
    td_api::file *file = GetFile_id(file_id);
    if (file)
    {
      file->local_->path_=path;
    }
  }
  inline bool GetMini()
  {
    return mini;
  }
  void SetMini(bool mini)
  {
    this->mini = mini;
  }
  inline int32_t GetSize();
  inline int32_t GetRemSize();
  inline int GetFileID();
  void SetMessage(td_api::object_ptr<td_api::message> mess)
  {
    this->mess->mess = std::move(mess);
  }
  void SetDocumentHistory(td_api::object_ptr<td_api::messageDocument> doc)
  {
    this->mess->doc = std::move(doc);
  }
  void SetPhotoHistory(td_api::object_ptr<td_api::messagePhoto> photo)
  {
    this->mess->photo = std::move(photo);
  }
  void SetVideoHistory(td_api::object_ptr<td_api::messageVideo> video)
  {
    this->mess->video = std::move(video);
  }
  td_api::object_ptr<td_api::message> &GetMessage()
  {
    return mess->mess;
  }
  int GetUsage()
  {
    return count_usage;
  }
  bool3 GetMediaType(){
    if(mess->doc){
      return 1;
    }else if(mess->photo){
      return 0;
    }else if(mess->video){
      return -1;
    }
  }
  std::string GetMediaTypeString(){
    if(mess->doc){
      return "DOCUMENT";
    }else if(mess->photo){
      return "PHOTO";
    }else if(mess->video){
      return "VIDEO";
    }
  }
protected:
  int count_usage = 0;
  messageHistory *mess;
  bool mini = false;
};
class TdMedia
{
public:
  TdMedia()
  {
  }
  TdMedia(td_api::object_ptr<td_api::message> mess)
  {
  }
  void Show();

  bool3 GetMediaPh()
  {
    return mediaph;
  }
  void SetMediaPh(bool3 media)
  {
    this->mediaph = media;
  }
  std::string GetPath()
  {
    return path;
  }
  float procLoad();
  float procDown();
  messagemedia media;
  void SetPath(std::string path)
  {
    this->path = path;
  }

protected:
  std::string path;
  bool3 mediaph = -1;
};

struct TdFile
{

  int count_usage = 0;
  std::string path = "";
  std::fstream *file;
};
class Module
{
public:
  Module();
  Module(Tdlib *td);
  virtual void UpdatesModule(td_api::object_ptr<td_api::Object> &mod);
  void SetTd(Tdlib *td);

protected:
  Tdlib *td;
};
class HistoryChat : public Module
{
public:
  HistoryChat();
  HistoryChat(Tdlib *td);

  void UpdateChatHistory();
  void WaitHistory();
  messagemedia GetMessage(int index);
  void AddMessageHistory(messageHistory mess)
  {
    messages.push_back(std::move(mess));
  }
  int GetCount()
  {
    return messages.size();
  }
  void Test()
  {
    int in = messages.size();
  }

private:
  bool CheckMedia(td::tl_object_ptr<td::td_api::message> &mess);

private:
  bool get_his = false;
  td_api::int64 last_mess = 0;
  std::vector<messageHistory> messages;
};
class MenagerUDFile : public Module
{

public:
  MenagerUDFile();
  MenagerUDFile(Tdlib *td);

private:
  void procLogMiniPhoto(std::fstream &file);
  int SearchVPhoto(std::string path, bool commpress, bool3 mediaph);
  int SearchId(td_api::int64 id);
  void GetRometeSize(int id);
  int SearchLoc_id(int loc_id);
  void GetProcFile(int id);
 
public:
  void UpdatesModule(td_api::object_ptr<td_api::Object> &mod) override;
  void LoadPhoto(std::string path, bool commpress);
  void ShowLoadPhoto();
  void LoadVideo(std::string path);
  TdMedia* DownloadFile(messagemedia mess, bool mini);
  
  void SetHistoryChat(HistoryChat* hist);
  void Test(){
    TdMedia* p=downphoto[0];
  }
private:
  std::vector<messageHistory> loadbuffer;
  HistoryChat* hist;
  std::vector<TdFile> miniphoto;
  std::vector<TdMedia> loadphoto;
  std::vector<TdMedia*> downphoto;
};