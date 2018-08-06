#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <aria2/aria2.h>
#include <node_api.h>

#include "downloadManager.h"

using std::shared_ptr;

void ExecuteDownload(napi_env env, void *data);

void CompleteDownload(napi_env env, napi_status status, void *data);

class AriaDownloadWorker {
  public:
    friend void ExecuteDownload(napi_env env, void *data);
    friend void CompleteDownload(napi_env env, napi_status status, void *data);

    AriaDownloadWorker(napi_env env)
    : env(env) {}

    ~AriaDownloadWorker() {}

    napi_value download(std::string uri, std::string sesId, std::string fileName, napi_ref callback, DownloadOption option);

  private:
    DownloadOption option;

    std::string sesId;
    std::string downloadStatSerialized;
    std::string fileName;

    std::vector<std::string> uris;

    aria2::Session* session;

    napi_ref callback;
    napi_async_work request;
    napi_env env;
};


#endif
