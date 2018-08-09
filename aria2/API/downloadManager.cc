#include "downloadManager.h"
#include "downloadWorker.h"
#include "sessionManager.h"

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

// Used when the download is under aria2 control (running, pausing)
// IMPORTANT: this function remove download in aria2 session but not the physical disk.
napi_value DownloadManager::removeDownloadWithGid(napi_env &env, shared_ptr<napi_value[]> argv) {
  napi_status status;
  napi_value returnValue;
  size_t result;

  unique_ptr<char[]> gidBuffer(new char[100]);
  unique_ptr<char[]> sessionBuffer(new char[100]);

  bool force;
  int removeResult;

  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[0], sessionBuffer.get(), 100, &result));
  NAPI_CALL(env, napi_get_value_string_utf8(env, argv.get()[1], gidBuffer.get(), 100, &result));
  NAPI_CALL(env, napi_get_value_bool(env, argv.get()[2], &force));

  std::string sesId(sessionBuffer.get());
  std::string gidHex(gidBuffer.get());

  aria2::Session* session = SessionManager::getInstance()->getSession(sesId);
  aria2::A2Gid gid = aria2::hexToGid(gidHex);

  removeResult = aria2::removeDownload(session, gid, force);

  status = napi_create_int32(env, removeResult, &returnValue);

  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return returnValue;
}