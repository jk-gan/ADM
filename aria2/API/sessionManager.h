#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <map>
#include <thread>

#include <aria2/aria2.h>
#include <node_api.h>

class SessionManager {
  public:
    static SessionManager* getInstance();

    std::map<std::string, aria2::Session *> getSessionMap();
    aria2::Session* getSession(std::string sesId);
    std::vector<std::thread>* getRunWorker();
    
    void addSession(std::pair<std::string, aria2::Session*> session);
    void addSessionRunWorker(std::thread runWorker);

    void clearAllSession();
    void clearSession(std::string sesId);

    int ariaInit();
    int ariaDeInit();

    napi_value createSession(napi_env &env, napi_value *&argv);
    napi_value killAllSession(napi_env &env, napi_value *&argv);
    napi_value killSession(napi_env &env, napi_value *&argv);
    napi_value pauseAllSession(napi_env &env, napi_value *&argv);
    napi_value pauseSession(napi_env &env, napi_value *&argv);

  private:
    SessionManager() {}
    ~SessionManager() {}

    static SessionManager* instance;
    static std::map<std::string, aria2::Session*> sessionMap;
    static std::vector<std::thread> sessionRunWorker;
};

#endif