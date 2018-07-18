#include "sessionManager.h"
#include "sessionWorker.h"
#include "common.h"

std::map<std::string, shared_ptr<SessionContainer>> SessionManager::sessionContainer;

SessionManager* SessionManager::getInstance() {
  static SessionManager instance;

  return &instance;
}

int SessionManager::ariaInit() {
  return aria2::libraryInit();
}

int SessionManager::ariaDeInit() {
  return aria2::libraryDeinit();
}

napi_value SessionManager::createSession(napi_env &env, shared_ptr<napi_value[]> argv) {
  napi_ref callback;
  size_t result;
  char * sessionBuffer = new char[100];

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[0], sessionBuffer, 100, &result));
  NAPI_CALL(env, napi_create_reference(env, argv.get()[1], 1, &callback));

  std::string sesId(sessionBuffer);

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  worker->createSession(sesId, callback);

  delete sessionBuffer;

  return nullptr;
}

napi_value SessionManager::killAllSession(napi_env &env) {
  shared_ptr<AriaSessionWorker> worker = std::make_shared<AriaSessionWorker>(AriaSessionWorker(env));

  worker.get()->killAllSession();

  return nullptr;
}

napi_value SessionManager::killSession(napi_env &env, shared_ptr<napi_value[]> argv) {
  napi_ref callback;
  size_t result;
  char * sessionBuffer = new char[100];

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[0], sessionBuffer, 100, &result));
  NAPI_CALL(env, napi_create_reference(env, argv.get()[1], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  std::string sesId(sessionBuffer);

  worker->killSession(sesId, callback);

  return nullptr;
}

napi_value SessionManager::pauseAllSession(napi_env &env, shared_ptr<napi_value[]> argv) {
  napi_ref callback;

  NAPI_CALL(env, napi_create_reference(env, argv.get()[0], 1, &callback));

 AriaSessionWorker* worker = new AriaSessionWorker(env);

  worker->pauseAllSession(callback);

  return nullptr;
}

napi_value SessionManager::pauseSession(napi_env &env, shared_ptr<napi_value[]> argv) {
  napi_ref callback;
  size_t result;
  char * sessionBuffer = new char[100];

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[0], sessionBuffer, 100, &result));
  NAPI_CALL(env, napi_create_reference(env, argv.get()[1], 1, &callback));

  AriaSessionWorker* worker = new AriaSessionWorker(env);

  std::string sesId(sessionBuffer);

  worker->pauseSession(sesId, callback);

  return nullptr;
}

void SessionManager::addSession(std::string sessionId, SessionContainer sesContainer) {
  sessionContainer.insert(std::make_pair(sessionId, std::make_shared<SessionContainer>(std::move(sesContainer))));
}

void SessionManager::addSessionRunWorker(std::string sessionId, std::thread runWorker) {
  sessionContainer[sessionId].get()->sessionWorker = move(runWorker);
}

void SessionManager::addExitSignal(std::string sessionId, std::promise<void> exitSignal) {
  sessionContainer[sessionId].get()->exitSignal = move(exitSignal);
}

std::map<std::string, aria2::Session*> SessionManager::getSessionMap() {
  std::map<std::string, aria2::Session*> tempSessionMap;

  for(auto sessionObj : sessionContainer) {
    tempSessionMap[sessionObj.first] = sessionObj.second->session;
  }

  return tempSessionMap;
}

std::map<std::string, shared_ptr<SessionContainer>> SessionManager::getSessionContainers() {
  return sessionContainer;
}

aria2::Session* SessionManager::getSession(std::string sesId) {
  return sessionContainer.find(sesId) != sessionContainer.end() ? sessionContainer[sesId]->session : nullptr;
}

void SessionManager::clearAllSession() {
  sessionContainer.clear();
}

void SessionManager::clearSession(std::string sesId) {
  std::map<std::string, shared_ptr<SessionContainer>>::iterator it;

  it = sessionContainer.find(sesId);

  sessionContainer.erase(it);
}