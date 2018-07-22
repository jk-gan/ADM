#include <aria2/aria2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>

#include "downloadWorker.h"
#include "sessionManager.h"
#include "common.h"

napi_value AriaDownloadWorker::download(std::string uri, std::string sesId, napi_ref callback) {
  napi_value resource_name;

  this->uris.push_back(uri);
  this->sesId = sesId;
  this->callback = callback;

  NAPI_CALL(env, napi_create_string_utf8(env, "DownloadResource", NAPI_AUTO_LENGTH, &resource_name));

  NAPI_CALL(env, napi_create_async_work(env, NULL, resource_name, 
    ExecuteDownload, CompleteDownload, this, &request));

  NAPI_CALL(env, napi_queue_async_work(env, request));

  return nullptr;
}

void ExecuteDownload(napi_env env, void *data) {
  AriaDownloadWorker* worker = static_cast<AriaDownloadWorker *>(data);

  aria2::Session* session = SessionManager::getInstance()->getSession(worker->sesId);

  if(session == nullptr) {
    std::cerr << "ERROR: session id not found." << std::endl;
    return;
  }

  worker->uris.push_back(worker->uris[0]);

  int rv;
  if (worker->uris.size() < 2) {
    std::cerr << "Usage: libaria2ex URI [URI...]\n"
              << "\n"
              << "  Download given URIs in parallel in the current directory."
              << std::endl;
  }

  // Add download item to session
  for (int i = 1; i < (int)worker->uris.size(); ++i) {
    aria2::KeyVals options;
    rv = aria2::addUri(session, nullptr, worker->uris, options);
    if (rv < 0) {
      std::cerr << "Failed to add download " << worker->uris[0] << std::endl;
    }
  }
}

void CompleteDownload(napi_env env, napi_status status, void *data) {
  AriaDownloadWorker* worker = static_cast<AriaDownloadWorker *>(data);

  napi_value argv[2];

  NAPI_CALL_RETURN_VOID(env, napi_get_null(env, &argv[0]));
  NAPI_CALL_RETURN_VOID(env, napi_create_string_utf8(env, worker->sesId.c_str(), worker->sesId.length() , &argv[1]));

  napi_value localCallback;
  NAPI_CALL_RETURN_VOID(env, napi_get_reference_value(env, worker->callback, &localCallback));

  napi_value global;
  NAPI_CALL_RETURN_VOID(env, napi_get_global(env, &global));

  napi_value result;
  NAPI_CALL_RETURN_VOID(env, napi_call_function(env, global, localCallback, 2, argv, &result));

  NAPI_CALL_RETURN_VOID(env, napi_delete_reference(env, worker->callback));
  NAPI_CALL_RETURN_VOID(env, napi_delete_async_work(env, worker->request));

  delete worker;
}