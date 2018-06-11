#include <node.h>
#include "sessionManager.h"
#include "sessionWorker.h"
#include "common.h"

SessionManager* SessionManager::instance;
std::map<int, aria2::Session*> SessionManager::sessionMap;

SessionManager* SessionManager::getInstance() {
  if(!instance) {
    instance = new SessionManager();
  }

  return instance;
}

napi_value SessionManager::createSession(napi_env &env, napi_value *&argv) {
  int sesId;
  napi_ref callback;

  NAPI_CALL(env, napi_get_value_int32(env, argv[0], &sesId));
  NAPI_CALL(env, napi_create_reference(env, argv[1], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  worker->createSession(sesId, callback);

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
  int sesId;
  napi_ref callback;

  NAPI_CALL(env, napi_get_value_int32(env, argv[0], &sesId));
  NAPI_CALL(env, napi_create_reference(env, argv[1], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  worker->killSession(sesId, callback);

  return nullptr;
}

void SessionManager::addSession(std::pair<int, aria2::Session*> session) {
  sessionMap.insert(session);
}

std::map<int, aria2::Session *> SessionManager::getSession() {
  return sessionMap;
}