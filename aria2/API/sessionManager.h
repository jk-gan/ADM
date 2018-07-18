#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <map>
#include <thread>
#include <future>

#include <aria2/aria2.h>
#include <node_api.h>

#include <sessionContainer.h>

using std::shared_ptr;
using std::unique_ptr;

class SessionManager {
  public:
    static SessionManager* getInstance();

    std::map<std::string, aria2::Session*> getSessionMap();
    std::map<std::string, shared_ptr<SessionContainer>> getSessionContainers();
    aria2::Session* getSession(std::string sesId);
    
    void addSession(std::string sessionId, SessionContainer sesContainer);
    void addSessionRunWorker(std::string sessionId, std::thread runWorker);
    void addExitSignal(std::string sessionId, std::promise<void> exitSignal);

    void clearAllSession();
    void clearSession(std::string sesId);

    int ariaInit();
    int ariaDeInit();

    napi_value createSession(napi_env &env, shared_ptr<napi_value[]> argv);
    napi_value killAllSession(napi_env &env);
    napi_value killSession(napi_env &env, shared_ptr<napi_value[]> argv);
    napi_value pauseAllSession(napi_env &env, shared_ptr<napi_value[]> argv);
    napi_value pauseSession(napi_env &env, shared_ptr<napi_value[]> argv);

  private:
    SessionManager() {}
    ~SessionManager() {}

    static std::map<std::string, shared_ptr<SessionContainer>> sessionContainer;
};

#endif