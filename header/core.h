#pragma once
#include "../td/td/telegram/Client.h"
#include "../td/td/generate/auto/td/telegram/td_api.h"
#include "../td/td/generate/auto/td/telegram/td_api.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <thread>
//#include <unistd.h>
// Simple single-threaded example of TDLib usage.
// Real world programs should use separate thread for the user input.
// Example includes user authentication, receiving updates, getting chat list and sending text messages.

// overloaded
#ifdef TDCLOUD_EXPORTS
#define TDCLOUD_API __declspec(dllexport)
#else
#define TDCLOUD_API __declspec(dllimport)
#endif
namespace detail
{
  template <class... Fs>
  struct overload;

  template <class F>
  struct overload<F> : public F
  {
    explicit overload(F f) : F(f)
    {
    }
  };
  template <class F, class... Fs>
  struct overload<F, Fs...>
      : public overload<F>, public overload<Fs...>
  {
    overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...)
    {
    }
    using overload<F>::operator();
    using overload<Fs...>::operator();
  };
} // namespace detail

template <class... F>
auto overloaded(F... f)
{
  return detail::overload<F...>(f...);
}

namespace td_api = td::td_api;

 class Tdlib
{
public:
  Tdlib();
  ~Tdlib(); 
 
  void send_query(td_api::object_ptr<td_api::Function> f, std::function<void(td_api::object_ptr<td_api::Object>)> handler);
  td_api::int53 GetChannelId();
protected:
  td_api::int53 chat_chennal = 0;

protected:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_;
  std::int32_t client_id_{0};

  td_api::object_ptr<TDCLOUD_API td_api::AuthorizationState> authorization_state_;
  bool are_authorized_{false};
  bool need_restart_{false};
  std::uint64_t current_query_id_{0};
  std::uint64_t authentication_query_id_{0};

  std::map<std::uint64_t, std::function<void(Object)>> handlers_;

  std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;

  std::map<std::int64_t, std::string> chat_title_;
  
protected:
  void restart();

  void process_response(td::ClientManager::Response response);

  std::string get_user_name(std::int64_t user_id) const;
  std::string get_chat_title(std::int64_t chat_id) const;
  virtual void MyUpdate(td_api::object_ptr<td_api::Object>& update);
  virtual void process_update(td_api::object_ptr<td_api::Object> update);
  
  auto create_authentication_query_handler();
  virtual void on_authorization_state_update();

  void check_authentication_error(Object object);

  std::uint64_t next_query_id();
};