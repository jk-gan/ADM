#include "downloadManager.h"

DownloadManager* DownloadManager::instance;

DownloadManager* DownloadManager::getInstance() {
  if(!instance) {
    instance = new DownloadManager();
  }

  return instance;
}

napi_value DownloadManager::addDownload(napi_env &env, napi_value *&argv) {
  int sesId;
  string uri;
  napi_ref callback;

  NAPI_CALL(env, napi_get_value_int32(env, argv[0], &uri));
  NAPI_CALL(env, napi_get_value_int32(env, argv[1], &sesId));
  NAPI_CALL(env, napi_create_reference(env, argv[2], 1, &callback));

  AriaDownloadWorker* worker = new AriaDownloadWorker(env);

  worker->download(uri, sesId, callback);

  return nullptr;
}