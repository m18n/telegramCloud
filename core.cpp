#include"header/core.h"
  Tdlib::Tdlib()
  {
    td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
    client_manager_ = std::make_unique<td::ClientManager>();
    client_id_ = client_manager_->create_client_id();
    send_query(td_api::make_object<td_api::getOption>("use_quick_ack"), [this](Object object)
               { std::cout << to_string(object) << std::endl; });
  }
  Tdlib::~Tdlib() {

  }
void Tdlib::send_query(td_api::object_ptr<td_api::Function> f, std::function<void(td_api::object_ptr<td_api::Object>)> handler)
{
    auto query_id = next_query_id();
    if (handler)
    {
        handlers_.emplace(query_id, std::move(handler));
    }

    client_manager_->send(client_id_, query_id, std::move(f));
}
td_api::int53 Tdlib::GetChannelId()
{
    return chat_chennal;
}
void Tdlib::restart()
{
    client_manager_.reset();
   // *this = Tdlib();
}

void Tdlib::process_response(td::ClientManager::Response response)
{
    if (!response.object)
    {
        return;
    }
    // std::cout << response.request_id << " " << to_string(response.object) << std::endl;
    if (response.request_id == 0)
    {
        return process_update(std::move(response.object));
    }
    auto it = handlers_.find(response.request_id);
    if (it != handlers_.end())
    {
        it->second(std::move(response.object));
        handlers_.erase(it);
    }
}

std::string Tdlib::get_user_name(std::int64_t user_id) const
{
    auto it = users_.find(user_id);
    if (it == users_.end())
    {
        return "unknown user";
    }
    return it->second->first_name_ + " " + it->second->last_name_;
}

std::string Tdlib::get_chat_title(std::int64_t chat_id) const
{
    auto it = chat_title_.find(chat_id);
    if (it == chat_title_.end())
    {
        return "unknown chat";
    }
    return it->second;
}
void Tdlib::MyUpdate(td_api::object_ptr<td_api::Object>& update){

}
void Tdlib::process_update(td_api::object_ptr<td_api::Object> update)
{
    td_api::downcast_call(
        *update, overloaded(
                     [this](td_api::updateAuthorizationState &update_authorization_state)
                     {
                         authorization_state_ = std::move(update_authorization_state.authorization_state_);
                         on_authorization_state_update();
                     },

                     [this](td_api::updateNewChat &update_new_chat)
                     {
                         chat_title_[update_new_chat.chat_->id_] = update_new_chat.chat_->title_;
                     },
                     [this](td_api::updateChatTitle &update_chat_title)
                     {
                         chat_title_[update_chat_title.chat_id_] = update_chat_title.title_;
                     },
                     [this](td_api::updateUser &update_user)
                     {
                         auto user_id = update_user.user_->id_;
                         users_[user_id] = std::move(update_user.user_);
                     },
                     [this](td_api::updateNewMessage &update_new_message)
                     {
                         auto chat_id = update_new_message.message_->chat_id_;
                         std::string sender_name;
                         td_api::downcast_call(*update_new_message.message_->sender_id_,
                                               overloaded(
                                                   [this, &sender_name](td_api::messageSenderUser &user)
                                                   {
                                                       sender_name = get_user_name(user.user_id_);
                                                   },
                                                   [this, &sender_name](td_api::messageSenderChat &chat)
                                                   {
                                                       sender_name = get_chat_title(chat.chat_id_);
                                                   }));
                         std::string text;
                         if (update_new_message.message_->content_->get_id() == td_api::messageText::ID)
                         {
                             text = static_cast<td_api::messageText &>(*update_new_message.message_->content_).text_->text_;
                         }
                         std::cout << "Got message: [chat_id:" << chat_id << "] [from:" << sender_name << "] [" << text
                                   << "]" << std::endl;
                     },
                     [](auto &update) {}));
    MyUpdate(update);
}

auto Tdlib::create_authentication_query_handler()
{
    return [this, id = authentication_query_id_](Object object)
    {
        if (id == authentication_query_id_)
        {
            check_authentication_error(std::move(object));
        }
    };
}
void Tdlib::on_authorization_state_update()
{
    authentication_query_id_++;
    td_api::downcast_call(
        *authorization_state_,
        overloaded(
            [this](td_api::authorizationStateReady &)
            {
                are_authorized_ = true;
                std::cout << "Got authorization" << std::endl;
            },
            [this](td_api::authorizationStateLoggingOut &)
            {
                are_authorized_ = false;

                std::cout << "Logging out" << std::endl;
            },
            [this](td_api::authorizationStateClosing &)
            { std::cout << "Closing" << std::endl; },
            [this](td_api::authorizationStateClosed &)
            {
                are_authorized_ = false;
                need_restart_ = true;
                std::cout << "Terminated" << std::endl;
            },
            [this](td_api::authorizationStateWaitCode &)
            {
                std::cout << "Enter authentication code: " << std::flush;
                std::string code;
                std::cin >> code;
                send_query(td_api::make_object<td_api::checkAuthenticationCode>(code),
                           create_authentication_query_handler());
            },
            [this](td_api::authorizationStateWaitRegistration &)
            {
                std::string first_name;
                std::string last_name;
                std::cout << "Enter your first name: " << std::flush;
                std::cin >> first_name;
                std::cout << "Enter your last name: " << std::flush;
                std::cin >> last_name;
                send_query(td_api::make_object<td_api::registerUser>(first_name, last_name),
                           create_authentication_query_handler());
            },
            [this](td_api::authorizationStateWaitPassword &)
            {
                std::cout << "Enter authentication password: " << std::flush;
                std::string password;
                std::getline(std::cin, password);
                send_query(td_api::make_object<td_api::checkAuthenticationPassword>(password),
                           create_authentication_query_handler());
            },
            [this](td_api::authorizationStateWaitOtherDeviceConfirmation &state)
            {
                std::cout << "Confirm this login link on another device: " << state.link_ << std::endl;
            },
            [this](td_api::authorizationStateWaitPhoneNumber &)
            {
                std::cout << "Enter phone number: " << std::flush;
                std::string phone_number;
                std::cin >> phone_number;
                send_query(td_api::make_object<td_api::setAuthenticationPhoneNumber>(phone_number, nullptr),
                           create_authentication_query_handler());
            },
            [this](td_api::authorizationStateWaitEncryptionKey &)
            {
               
                    send_query(td_api::make_object<td_api::checkDatabaseEncryptionKey>(std::move("")),
                               create_authentication_query_handler());
                
            },
            [this](td_api::authorizationStateWaitTdlibParameters &)
            {
                auto parameters = td_api::make_object<td_api::tdlibParameters>();
                parameters->database_directory_ = "tdlib";
                parameters->use_message_database_ = true;
                parameters->use_secret_chats_ = true;
                parameters->api_id_ = 94575;
                parameters->api_hash_ = "a3406de8d171bb422bb6ddf3bbd800e2";
                parameters->system_language_code_ = "en";
                parameters->device_model_ = "Desktop";
                parameters->application_version_ = "1.0";
                parameters->enable_storage_optimizer_ = true;
                send_query(td_api::make_object<td_api::setTdlibParameters>(std::move(parameters)),
                           create_authentication_query_handler());
            }));
}

void Tdlib::check_authentication_error(Object object)
{
    if (object->get_id() == td_api::error::ID)
    {
        auto error = td::move_tl_object_as<td_api::error>(object);
        std::cout << "Error: " << to_string(error) << std::flush;
        on_authorization_state_update();
    }
}

std::uint64_t Tdlib::next_query_id()
{
    return ++current_query_id_;
}