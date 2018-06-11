#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <map>

#include <aria2/aria2.h>
#include <node_api.h>

class SessionManager {
  public:
    static SessionManager* getInstance();

    std::map<int, aria2::Session *> getSession();
    
    void addSession(std::pair<int, aria2::Session*> session);

    napi_value createSession(napi_env &env, napi_value *&argv);
    napi_value killAllSession(napi_env &env, napi_value *&argv);
    napi_value killSession(napi_env &env, napi_value *&argv);

  private:
    SessionManager() {}
    ~SessionManager() {}

    static SessionManager* instance;
    static std::map<int, aria2::Session*> sessionMap;
};

#endif