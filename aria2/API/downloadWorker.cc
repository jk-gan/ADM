#include <aria2/aria2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>

#include "json.hpp"

#include "monitoringManager.h"
#include "downloadWorker.h"
#include "sessionManager.h"
#include "common.h"

using json = nlohmann::json;

namespace monitoring {
    void to_json(json& j, const DownloadStatData& p) {
        j.push_back(
          {
            {"gid", p.gid},
            {"fileName", p.fileName},
            {"completedLength", p.completedLength},
            {"totalLength", p.totalLength},
            {"downloadSpeed", p.downloadSpeed},
            {"uploadSpeed", p.uploadSpeed},
          }
        );
    }
}

napi_value AriaDownloadWorker::download(std::string uri, std::string sesId, napi_ref callback, DownloadOption option) {
  napi_value resource_name;

  this->uris.push_back(uri);
  this->sesId = sesId;
  this->callback = callback;
  this->option = option;

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

  json downloadStatJson;

  // Add download item to session
  for (int i = 1; i < (int)worker->uris.size(); ++i) {
    monitoring::DownloadStatData dStateData;
    shared_ptr<aria2::A2Gid[]> gidPtr(new aria2::A2Gid[1]);
    aria2::KeyVals options;
    std::string fileName = "";

    // Set options
    if(worker->option == RESUME) {
      options.push_back(std::pair<std::string, std::string>("continue", "true"));
    }

    options.push_back(std::pair<std::string, std::string>("auto-save-interval", "1"));
    
    rv = aria2::addUri(session, gidPtr.get(), worker->uris, options);

    if (rv < 0) {
      std::cerr << "Failed to add download " << worker->uris[0] << std::endl;
    }
  
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    aria2::DownloadHandle* dh = aria2::getDownloadHandle(session, gidPtr[0]);

    if (dh) {
      aria2::FileData f = dh->getFile(1);

      if (f.path.empty()) {
        if (!f.uris.empty()) {
          fileName = f.uris[0].uri;
        }
      }
      else {
        fileName = f.path;
      }

      dStateData = {
        aria2::gidToHex(gidPtr[0]),
        fileName,
        dh->getCompletedLength(),
        dh->getTotalLength(),
        dh->getDownloadSpeed(),
        dh->getUploadSpeed()
      };

      aria2::deleteDownloadHandle(dh);
      
      downloadStatJson = dStateData;
    }
  }

  worker->downloadStatSerialized = downloadStatJson.dump();
}

void CompleteDownload(napi_env env, napi_status status, void *data) {
  AriaDownloadWorker* worker = static_cast<AriaDownloadWorker *>(data);

  napi_value argv[2];

  // Throw error
  if(worker->downloadStatSerialized == "null") {
    std::string errorMsg = "Error: URL is not correct!";
    NAPI_CALL_RETURN_VOID(env, napi_create_string_utf8(env, errorMsg.c_str(), errorMsg.length(), &argv[0]));
  }
  else {
    NAPI_CALL_RETURN_VOID(env, napi_get_null(env, &argv[0]));
  }
  
  NAPI_CALL_RETURN_VOID(env, napi_create_string_utf8(env, worker->downloadStatSerialized.c_str(), worker->downloadStatSerialized.length() , &argv[1]));

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