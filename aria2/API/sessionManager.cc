#include "sessionManager.h"
#include "sessionWorker.h"
#include "common.h"

SessionManager* SessionManager::instance;
std::map<std::string, SessionContainer*> SessionManager::sessionContainer;

SessionManager* SessionManager::getInstance() {
  if(!instance) {
    instance = new SessionManager();
  }

  return instance;
}

int SessionManager::ariaInit() {
  return aria2::libraryInit();
}

int SessionManager::ariaDeInit() {
  return aria2::libraryDeinit();
}

napi_value SessionManager::createSession(napi_env &env, napi_value *&argv) {
  napi_ref callback;
  size_t result;
  char * sessionBuffer = new char[100];

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[0], sessionBuffer, 100, &result));
  NAPI_CALL(env, napi_create_reference(env, argv[1], 1, &callback));

  std::string sesId(sessionBuffer);

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  worker->createSession(sesId, callback);

  delete sessionBuffer;

  return nullptr;
}

napi_value SessionManager::killAllSession(napi_env &env, napi_value *&argv) {
  napi_ref callback;
std::cerr << "test";
  NAPI_CALL(env, napi_create_reference(env, argv[0], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  worker->killAllSession(callback);

  return nullptr;
}

napi_value SessionManager::killSession(napi_env &env, napi_value *&argv) {
  napi_ref callback;
  size_t result;
  char * sessionBuffer = new char[100];

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[0], sessionBuffer, 100, &result));
  NAPI_CALL(env, napi_create_reference(env, argv[1], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  std::string sesId(sessionBuffer);

  worker->killSession(sesId, callback);

  return nullptr;
}

napi_value SessionManager::pauseAllSession(napi_env &env, napi_value *&argv) {
  napi_ref callback;

  NAPI_CALL(env, napi_create_reference(env, argv[0], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  worker->pauseAllSession(callback);

  return nullptr;
}

napi_value SessionManager::pauseSession(napi_env &env, napi_value *&argv) {
  napi_ref callback;
  size_t result;
  char * sessionBuffer = new char[100];

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[0], sessionBuffer, 100, &result));
  NAPI_CALL(env, napi_create_reference(env, argv[1], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  std::string sesId(sessionBuffer);

  worker->pauseSession(sesId, callback);

  return nullptr;
}

void SessionManager::addSession(std::pair<std::string, SessionContainer*> session) {
  sessionContainer.insert(session);
}

void SessionManager::addSessionRunWorker(std::string sessionId, std::thread runWorker) {
  sessionContainer[sessionId]->sessionWorker = move(runWorker);
}

void SessionManager::addExitSignal(std::string sessionId, std::promise<void> exitSignal) {
  sessionContainer[sessionId]->exitSignal = move(exitSignal);
}

std::map<std::string, aria2::Session *> SessionManager::getSessionMap() {
  std::map<std::string, aria2::Session*> tempSessionMap;

  for(auto sessionObj : sessionContainer) {
    tempSessionMap[sessionObj.first] = sessionObj.second->session;
  }

  return tempSessionMap;
}

std::map<std::string, SessionContainer *> SessionManager::getSessionContainers() {
  return sessionContainer;
}

aria2::Session* SessionManager::getSession(std::string sesId) {
  return sessionContainer.find(sesId) != sessionContainer.end() ? sessionContainer[sesId]->session : nullptr;
}

void SessionManager::clearAllSession() {
  sessionContainer.clear();
}

void SessionManager::clearSession(std::string sesId) {
  std::map<std::string, SessionContainer*>::iterator it;

  it = sessionContainer.find(sesId);

  sessionContainer.erase(it);
}