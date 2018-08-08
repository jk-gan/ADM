#include "downloadManager.h"
#include "downloadWorker.h"

#include "common.h"

DownloadManager* DownloadManager::getInstance() {
  static DownloadManager instance;

  return &instance;
}

napi_value DownloadManager::addDownload(napi_env &env, shared_ptr<napi_value[]> argv, DownloadOption option) {
  size_t result;

  napi_ref callback;

  unique_ptr<char[]> buffer(new char[1000]);
  unique_ptr<char[]> fileNameBuffer(new char[1000]);
  unique_ptr<char[]> sessionBuffer(new char[100]);

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[0], buffer.get(), 1000, &result));
  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[1], sessionBuffer.get(), 100, &result));
  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[2], fileNameBuffer.get(), 1000, &result));
  NAPI_CALL(env, napi_create_reference(env, argv.get()[3], 1, &callback));

  AriaDownloadWorker* worker = new AriaDownloadWorker(env);

  std::string uri(buffer.get());
  std::string sesId(sessionBuffer.get());
  std::string fileName(fileNameBuffer.get());

  worker->download(uri, sesId, fileName, callback, option);

  return nullptr;
}