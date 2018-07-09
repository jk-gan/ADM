#include "sessionManager.h"
#include "sessionWorker.h"
#include "common.h"

SessionManager* SessionManager::instance;
std::map<std::string, aria2::Session*> SessionManager::sessionMap;
std::vector<std::thread> SessionManager::sessionRunWorker;

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

void SessionManager::addSession(std::pair<std::string, aria2::Session*> session) {
  sessionMap.insert(session);
}

void SessionManager::addSessionRunWorker(std::thread runWorker) {
  sessionRunWorker.push_back(move(runWorker));
}

std::map<std::string, aria2::Session *> SessionManager::getSessionMap() {
  return sessionMap;
}

aria2::Session* SessionManager::getSession(std::string sesId) {
  return sessionMap.find(sesId) != sessionMap.end() ? sessionMap[sesId] : nullptr;
}

std::vector<std::thread>* SessionManager::getRunWorker() {
  return &sessionRunWorker;
}

void SessionManager::clearAllSession() {
  sessionMap.clear();
}

void SessionManager::clearSession(std::string sesId) {
  std::map<std::string, aria2::Session*>::iterator it;

  it = sessionMap.find(sesId);

  sessionMap.erase(it);
}