#include "header/tdclass.h"

void TdCloud::GetUpdate()
{
  while (true)
  {
    auto response = client_manager_->receive(0);
    if (response.object)
    {
      process_response(std::move(response));
    }
  }
}
void TdCloud::Login()
{
  while (true)
  {

    if (need_restart_)
    {
      restart();
    }
    else if (!are_authorized_)
    {
      process_response(client_manager_->receive(10));
    }
    else
    {
      std::thread th(&TdCloud::GetUpdate, this);
      th.detach();
      break;
    }
  }
}

void TdCloud::CreateChannel()
{
  auto channel = td_api::make_object<td_api::createNewSupergroupChat>();
  channel->title_ = channel_td;
  channel->is_channel_ = true;
  send_query(std::move(channel), [this](td_api::object_ptr<td_api::Object> object)
             {
                 if (object->get_id() == td_api::error::ID)
                 {
                   channel_b = 0;
                   auto errr = td::move_tl_object_as<td_api::error>(object);
                   std::cout << "ERROR: " << errr->message_ << "\n";
                   return;
                 }
                 auto channel = td::move_tl_object_as<td_api::chat>(object);
                 if (channel->id_ != 0)
                 {
                   channel_b = 1;
                   std::cout << "CHANNEL CREATE: " << channel->title_ << " ID: " << channel->id_ << "\n";
                   chat_chennal = channel->id_;
                   std::cout << "CHANNEL INIT GOOD\n";
                 }
                 else
                 {
                   channel_b = 0;
                   chat_chennal = 0;
                 } });
}

void TdCloud::GetNameChannel()
{
  auto channel = td_api::make_object<td_api::getChat>();
  channel->chat_id_ = chat_chennal;

  send_query(std::move(channel), [this](td_api::object_ptr<td_api::Object> object)
             {
                 if (object->get_id() == td_api::error::ID)
                 {
                   channel_b=0;
                   auto errr = td::move_tl_object_as<td_api::error>(object);
                   std::cout << "ERROR: " << errr->message_ << "\n";
                   return;
                 }
                 auto channel = td::move_tl_object_as<td_api::chat>(object);
                 std::cout << "NAME: " << channel->title_ << "\n";
                 name_chennal =  channel->title_;
                   if (name_chennal != channel_td)
                  {
                    CreateChannel();
                  }else{
                    channel_b=1;
                    std::cout<<"CHANNEL INIT GOOD\n";
                  } });
}
void TdCloud::ChannelInit()
{
  auto chat = td_api::make_object<td_api::searchChatsOnServer>();
  chat->query_ = channel_td;
  chat->limit_ = 1;

  send_query(std::move(chat), [this](td_api::object_ptr<td_api::Object> object)
             {
                if (object->get_id() == td_api::error::ID)
                {
                  channel_b=0;
                  std::cout << "ERROR: " << object->get_id() << "\n";
                  return;
                }
                auto chats = td::move_tl_object_as<td_api::chats>(object);
               
                if(chats->total_count_!=0){
                chat_chennal=chats->chat_ids_[0];
                std::cout<<"ID CHANNAL: "<<chat_chennal<<"\n";
                GetNameChannel();
                }else{
                  std::cout<<"ERROR Search\n";
                  CreateChannel();
                } });
  while (channel_b == -1)
  {
  }
}


TdMedia* TdCloud::DownloadManager(int loc_id,bool mini){

  messagemedia mes;
  mes= hist.GetMessage(loc_id);
  return this->file.DownloadFile(mes,mini);
 
}