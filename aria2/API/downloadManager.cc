#include "downloadManager.h"
#include "downloadWorker.h"

#include "common.h"

DownloadManager* DownloadManager::instance;

DownloadManager* DownloadManager::getInstance() {
  if(!instance) {
    instance = new DownloadManager();
  }

  return instance;
}

napi_value DownloadManager::addDownload(napi_env &env, napi_value *&argv) {
  int sesId;
  size_t result;

  napi_ref callback;

  char * buffer = new char[1000];

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[0], buffer, 1000, &result));
  NAPI_CALL(env, napi_get_value_int32(env, argv[1], &sesId));
  NAPI_CALL(env, napi_create_reference(env, argv[2], 1, &callback));

  AriaDownloadWorker* worker = new AriaDownloadWorker(env);

  std::string uri(buffer);

  worker->download(uri, sesId, callback);

  delete buffer;

  return nullptr;
}