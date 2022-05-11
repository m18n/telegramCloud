#pragma once

#include "core.h"
#include <fstream>
#include<iostream>
#include <string.h>
typedef char bool3;
namespace td_api = td::td_api;
class TDCLOUD_API messageHistory
{
public:
  messageHistory(const messageHistory& x);
  //messageHistory& operator=(const messageHistory& x);
  messageHistory();
  ~messageHistory();
  messageHistory(messageHistory&& x) noexcept;
  td_api::object_ptr<td_api::message> mess = NULL;
  td_api::object_ptr<td_api::messageDocument> doc = NULL;
  td_api::object_ptr<td_api::messagePhoto> photo = NULL;
  td_api::object_ptr<td_api::messageVideo> video = NULL;
};
class TDCLOUD_API messagemedia
{
private:
    td_api::file* GetFile_id(int file_id);

public:
    messagemedia();
    ~messagemedia();
    bool Que(messagemedia& mes);
    void SetMessageHistory(messageHistory* mess);
    std::string GetPath();
    void Usage();
    int GetCountUsage();
    void SetSize(int file_id, int size);
    void SetRemoteSize(int file_id, int size);
    void SetPath(int file_id, std::string path);
    bool GetMini();
    void SetMini(bool mini);
  int32_t GetSize();
  int32_t GetRemSize();
  int GetFileID();
  void SetMessage(td_api::object_ptr<td_api::message> mess);
  void SetDocumentHistory(td_api::object_ptr<td_api::messageDocument> doc);
  void SetPhotoHistory(td_api::object_ptr<td_api::messagePhoto> photo);
  void SetVideoHistory(td_api::object_ptr<td_api::messageVideo> video);
  td_api::object_ptr<td_api::message>& GetMessage();
  int GetUsage();
  bool3 GetMediaType();
  std::string GetMediaTypeString();
protected:
  int count_usage = 0;
  messageHistory *mess;
  bool mini = false;
};
class TdMedia
{
public:
  TDCLOUD_API TdMedia();
  TDCLOUD_API TdMedia(td_api::object_ptr<td_api::message> mess);
  TDCLOUD_API ~TdMedia();
  TDCLOUD_API void Show();

  TDCLOUD_API bool3 GetMediaPh();
  TDCLOUD_API void SetMediaPh(bool3 media);
  TDCLOUD_API std::string GetPath();
  TDCLOUD_API float procLoad();
  TDCLOUD_API float procDown();
  messagemedia media;
  TDCLOUD_API void SetPath(std::string path);
protected:
  std::string path;
  bool3 mediaph = -1;
};

struct  TdFile
{
    TDCLOUD_API TdFile();
    TDCLOUD_API ~TdFile();
  int count_usage = 0; 
  std::string path = "";
  std::fstream *file;
};
class Module
{
public:
  TDCLOUD_API Module();
  TDCLOUD_API Module(Tdlib *td);
  TDCLOUD_API ~Module();
  TDCLOUD_API virtual void UpdatesModule(td_api::object_ptr<td_api::Object> &mod);
  TDCLOUD_API void SetTd(Tdlib *td);

protected:
  Tdlib *td;
};
class HistoryChat : public Module
{
public:
 TDCLOUD_API  HistoryChat();
 TDCLOUD_API HistoryChat(Tdlib *td);
 TDCLOUD_API ~HistoryChat();
 TDCLOUD_API void UpdateChatHistory();
 TDCLOUD_API void WaitHistory();
 TDCLOUD_API messagemedia GetMessage(int index);
 TDCLOUD_API void AddMessageHistory(messageHistory mess);
 TDCLOUD_API int GetCount();
 TDCLOUD_API void Test();

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
    TDCLOUD_API MenagerUDFile();
    TDCLOUD_API MenagerUDFile(Tdlib *td);
    TDCLOUD_API ~MenagerUDFile();
private:
  void procLogMiniPhoto(std::fstream &file);
  int SearchVPhoto(std::string path, bool commpress, bool3 mediaph);
  int SearchId(td_api::int64 id);
  void GetRometeSize(int id);
  int SearchLoc_id(int loc_id);
  void GetProcFile(int id);
 
public:
    TDCLOUD_API void UpdatesModule(td_api::object_ptr<td_api::Object> &mod) override;
    TDCLOUD_API void LoadPhoto(std::string path, bool commpress);
    TDCLOUD_API void ShowLoadPhoto();
    TDCLOUD_API void LoadVideo(std::string path);
    TDCLOUD_API TdMedia* DownloadFile(messagemedia mess, bool mini);
  
    TDCLOUD_API void SetHistoryChat(HistoryChat* hist);

private:
  std::vector<messageHistory> loadbuffer;
  HistoryChat* hist;
  std::vector<TdFile> miniphoto;
  std::vector<TdMedia> loadphoto;
  std::vector<TdMedia*> downphoto;
};