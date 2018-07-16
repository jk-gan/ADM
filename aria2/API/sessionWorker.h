#ifndef SESSIONWORKER_H
#define SESSIONWORKER_H

#include <iostream>

#include <aria2/aria2.h>
#include <node_api.h>

#include <sessionContainer.h>

int downloadEventCallback(aria2::Session*, aria2::DownloadEvent, aria2::A2Gid, void*);

void ExecuteSessionInit(napi_env env, void *data);
void ExecuteKillSession(napi_env env, void *data);
void ExecutePauseAllSession(napi_env env, void *data);
void ExecutePauseSession(napi_env env, void *data);

void CompleteSessionInit(napi_env env, napi_status status, void *data);
void CompleteKillSession(napi_env env, napi_status status, void *data);
void CompletePauseAllSession(napi_env env, napi_status status, void *data);
void CompletePauseSession(napi_env env, napi_status status, void *data);

class AriaSessionWorker {
  public:
    friend void ExecuteSessionInit(napi_env env, void *data);
    friend void ExecuteKillSession(napi_env env, void *data);
    friend void ExecutePauseAllSession(napi_env env, void *data);
    friend void ExecutePauseSession(napi_env env, void *data);

    friend void CompleteSessionInit(napi_env env, napi_status status, void *data);
    friend void CompleteKillSession(napi_env env, napi_status status, void *data);
    friend void CompletePauseAllSession(napi_env env, napi_status status, void *data);
    friend void CompletePauseSession(napi_env env, napi_status status, void *data);

    AriaSessionWorker(napi_env env)
    : env(env) {}

    ~AriaSessionWorker() {}

    napi_value createSession(std::string sesId, napi_ref callback);
    napi_value killAllSession();
    napi_value killSession(std::string sesId, napi_ref callback);
    napi_value pauseAllSession(napi_ref callback);
    napi_value pauseSession(std::string sesId, napi_ref callback);

  private:
    std::string sesId;

    aria2::Session* session;

    napi_ref callback;
    napi_async_work request;
    napi_env env;
};

#endif