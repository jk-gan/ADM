#include <aria2/aria2.h>
#include <iostream>
#include <chrono>
#include <map>
#include "downloadWorker.h"

void AriaDownloadWorker::download(string uri, int sesId, napi_ref callback) {
  napi_value resource_name;

  this->uris.insert(uri);
  this->sesId = sesId;
  this->callback = callback;

  NAPI_CALL(env, napi_create_string_utf8(env, "DownloadResource", NAPI_AUTO_LENGTH, &resource_name));

  NAPI_CALL(env, napi_create_async_work(env, NULL, resource_name, 
    ExecuteDownload, CompleteDownload, this, &request));

  NAPI_CALL(env, napi_queue_async_work(env, request));

  return nullptr;
}

void ExecuteSessionInit(napi_env env, void *data) {
  AriaSessionWorker* worker = static_cast<AriaSessionWorker *>(data);

  aria2::session session = SessionManager::getSession(worker->sesId);

  int rv;
  if (worker->uris.size() < 2) {
    std::cerr << "Usage: libaria2ex URI [URI...]\n"
              << "\n"
              << "  Download given URIs in parallel in the current directory."
              << std::endl;
    exit(EXIT_SUCCESS);
  }

  // Add download item to session
  for (int i = 1; i < worker->uris.size(); ++i) {
    aria2::KeyVals options;
    rv = aria2::addUri(session, nullptr, worker->uris, options);
    if (rv < 0) {
      std::cerr << "Failed to add download " << worker->uris[0] << std::endl;
    }
  }
  auto start = std::chrono::steady_clock::now();

  for (;;) {

    if(!session) {
      break;
    }
    rv = aria2::run(session, aria2::RUN_ONCE);
    //aria2::GlobalStat gstat = aria2::getGlobalStat(session);

    // the application can call aria2 API to add URI or query progress
    // here
    auto now = std::chrono::steady_clock::now();
    auto count =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count();

    // Print progress information once per 500ms
    if (count >= 5000) {
      //int pauseDownload(Session *session, A2Gid gid, bool force = false)
      start = now;
      aria2::GlobalStat gstat = aria2::getGlobalStat(session);
      std::cerr << "Overall #Active:" << gstat.numActive
                << " #waiting:" << gstat.numWaiting
                << " D:" << gstat.downloadSpeed / 1024 << "KiB/s"
                << " U:" << gstat.uploadSpeed / 1024 << "KiB/s " << std::endl;
      std::vector<aria2::A2Gid> gids = aria2::getActiveDownload(session);
      for (const auto& gid : gids) {
        aria2::DownloadHandle* dh = aria2::getDownloadHandle(session, gid);
        if (dh) {
          std::cerr << "    [" << aria2::gidToHex(gid) << "] "
                    << dh->getCompletedLength() << "/" << dh->getTotalLength()
                    << "(" << (dh->getTotalLength() > 0
                                   ? (100 * dh->getCompletedLength() /
                                      dh->getTotalLength())
                                   : 0)
                    << "%)"
                    << " D:" << dh->getDownloadSpeed() / 1024
                    << "KiB/s, U:" << dh->getUploadSpeed() / 1024 << "KiB/s"
                    << std::endl;
          aria2::deleteDownloadHandle(dh);
        }
      }
    }
  }
}

void CompleteDownload(napi_env env, napi_status status, void *data) {
  AriaSessionWorker* worker = static_cast<AriaSessionWorker *>(data);

  napi_value argv[2];

  NAPI_CALL_RETURN_VOID(env, napi_get_null(env, &argv[0]));
  NAPI_CALL_RETURN_VOID(env, napi_create_int32(env, worker->sesId, &argv[1]));

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