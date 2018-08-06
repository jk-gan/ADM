#include <iostream>
#include <map>
#include <chrono>
#include <exception>
#include <thread>
#include <memory>

#include <aria2/aria2.h>

#include "json.hpp"

#include "monitoringManager.h"
#include "sessionManager.h"

using json = nlohmann::json;

namespace monitoring {
    void to_json(json& j, const SessionData& p) {
        for(auto download : p.dStat) {
          j.push_back(
            {
              {"gid", download.gid},
              {"fileName", download.fileName},
              {"uri", download.uri},
              {"completedLength", download.completedLength},
              {"totalLength", download.totalLength},
              {"downloadSpeed", download.downloadSpeed},
              {"uploadSpeed", download.uploadSpeed},
              {"sessionId", p.sessionID}
            }
          );
        }
    }

    void to_json(json& j, const DownloadCallbackSignal& p) {
      std::string eventStr;

      switch (p.evt) {
        case aria2::EVENT_ON_DOWNLOAD_COMPLETE:
          eventStr = "COMPLETE";
          break;
        case aria2::EVENT_ON_DOWNLOAD_ERROR:
          eventStr = "ERROR";
          break;
        default:
          eventStr = "UNHANDLED";
      }

      j = {
        {"event", eventStr},
        {"gid", p.gid},
        {"filename", p.fileName}
      };
    }
}

bool MonitoringManager::isStopMonitoring;

MonitoringManager* MonitoringManager::getInstance() {
  static MonitoringManager instance;

  return &instance;
}

napi_value MonitoringManager::startMonitoring(napi_env &env, shared_ptr<napi_value[]> argv) {
  napi_ref listenerCallback, completeCallback;

  napi_status status;
  napi_value isSuccess;

  if(!isStopMonitoring) {
    status = napi_create_int32(env, 0, &isSuccess);

    if (status != napi_ok)
    {
      napi_throw_error(env, NULL, "Unable to create return value");
    }

    return isSuccess;
  }
  
  try {
    NAPI_CALL(env, napi_create_reference(env, argv.get()[0], 1, &listenerCallback));
    NAPI_CALL(env, napi_create_reference(env, argv.get()[1], 1, &completeCallback));

    this->eventHandler = listenerCallback;
    this->completeHandler = completeCallback;
    this->env = env;

    isStopMonitoring = false;
    
    napi_value resource_name;
    NAPI_CALL(env, napi_create_string_utf8(env, "MonitoringResource", NAPI_AUTO_LENGTH, &resource_name));

    NAPI_CALL(env, napi_create_async_work(env, NULL, resource_name, 
      ExecuteMonitoring, CompleteMonitoring, this, &request));

    NAPI_CALL(env, napi_queue_async_work(env, request));
    
    status = napi_create_int32(env, 5, &isSuccess);
  }
  catch(std::exception ex) {
    status = napi_create_int32(env, -1, &isSuccess);
  }

  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return isSuccess;
}

napi_value MonitoringManager::stopMonitoring(napi_env &env) {
  napi_status status;
  napi_value isSuccess;

  if(isStopMonitoring) {
    status = napi_create_int32(env, 0, &isSuccess);

    if (status != napi_ok)
    {
      napi_throw_error(env, NULL, "Unable to create return value");
    }

    return isSuccess;
  }

  try {
    isStopMonitoring = true;

    this->futureObj.wait_for(std::chrono::seconds(10));

    status = napi_create_int32(env, 1, &isSuccess);
  }
  catch(std::exception ex) {
    status = napi_create_int32(env, -1, &isSuccess);
  }

  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return isSuccess;
}

void ExecuteMonitoring(napi_env env, void *data) {
  MonitoringManager* monitoringMgr = static_cast<MonitoringManager *>(data);

  monitoringMgr->listenAria2();
}

void CompleteMonitoring(napi_env env, napi_status status, void *data) {
  napi_value argv[1];
  MonitoringManager* monitoringMgr = static_cast<MonitoringManager *>(data);

  napi_value global;
  NAPI_CALL_RETURN_VOID(env, napi_get_global(env, &global));

  napi_value localCallback;
  napi_value result;
  NAPI_CALL_RETURN_VOID(env, napi_get_reference_value(env, monitoringMgr->eventHandler, &localCallback));

  if(monitoringMgr->ariaDataSerialized != "null") {
    size_t str_len = monitoringMgr->ariaDataSerialized.length();
    NAPI_CALL_RETURN_VOID(env, napi_create_string_utf8(env, monitoringMgr->ariaDataSerialized.c_str(), str_len , &argv[0]));

    // Call the deligate function from nodejs
    NAPI_CALL_RETURN_VOID(env, napi_call_function(env, global, localCallback, 1, argv, &result));
  }

  // Check complete event signal
  if(monitoringMgr->completeSignalQueue.size() > 0) {
    napi_value completeArgv[1];

    json completeDataJson = monitoringMgr->completeSignalQueue.front();
    std::string completeDataSerialized = completeDataJson.dump();
    size_t completeDataLength = completeDataSerialized.length();

    NAPI_CALL_RETURN_VOID(env, napi_create_string_utf8(env, completeDataSerialized.c_str(), completeDataLength , &completeArgv[0]));

    NAPI_CALL_RETURN_VOID(env, napi_get_reference_value(env, monitoringMgr->completeHandler, &localCallback));
    NAPI_CALL_RETURN_VOID(env, napi_call_function(env, global, localCallback, 1, completeArgv, &result));

    monitoringMgr->completeSignalQueue.pop();
  }

  if(MonitoringManager::isStopMonitoring) {
    NAPI_CALL_RETURN_VOID(env, napi_delete_reference(env, monitoringMgr->eventHandler));
    NAPI_CALL_RETURN_VOID(env, napi_delete_reference(env, monitoringMgr->completeHandler));
    NAPI_CALL_RETURN_VOID(env, napi_delete_async_work(env, monitoringMgr->request));
  }
  else {
    NAPI_CALL_RETURN_VOID(env, napi_queue_async_work(env, monitoringMgr->request));
  }
}

void MonitoringManager::addDownloadEventSignal(aria2::DownloadEvent evt, std::string gid, std::string fileName) {
  monitoring::DownloadCallbackSignal signal = {
    evt,
    gid,
    fileName
  };

  completeSignalQueue.push(signal);
}

void MonitoringManager::listenAria2() {
  std::map<std::string, aria2::Session*>::iterator it;
  SessionManager* sesMgr = SessionManager::getInstance();

  monitoring::SessionData sessionData;
  json sessionDataJson;
  json ariaDataJson;

  // Get latest session collection
  std::map<std::string, aria2::Session*> sessionMap = sesMgr->getSessionMap();

  int gNumActive = 0;
  int gNumWaiting = 0;
  int gDownloadSpeed = 0;
  int gUploadSpeed = 0;

  // Loop all the session
  for(it = sessionMap.begin(); it != sessionMap.end(); it++) {
    try {
      // Set Id
      sessionData.sessionID = it->first;

      // Get global stat
      aria2::GlobalStat gstat = aria2::getGlobalStat(it->second);

      gNumActive += gstat.numActive;
      gNumWaiting += gstat.numWaiting;
      gDownloadSpeed += gstat.downloadSpeed;
      gUploadSpeed += gstat.uploadSpeed;

      sessionData.gStat = {
        gstat.numActive,
        gstat.numWaiting, 
        gstat.downloadSpeed, 
        gstat.uploadSpeed
      };

      std::vector<aria2::A2Gid> gids = aria2::getActiveDownload(it->second);

      // Loop for all downloads
      for (const auto& gid : gids) {
        aria2::DownloadHandle* dh = aria2::getDownloadHandle(it->second, gid);

        std::string fileName = "";
        std::string uri = "";
        aria2::FileData f = dh->getFile(1);

        if (f.path.empty()) {
          if (!f.uris.empty()) {
            fileName = f.uris[0].uri;
            uri = fileName;
          }
        }
        else {
          fileName = f.path;
          uri = f.uris[0].uri;
        }

        if (dh) {
          sessionData.dStat.push_back({
            aria2::gidToHex(gid),
            fileName,
            uri,
            dh->getCompletedLength(),
            dh->getTotalLength(),
            dh->getDownloadSpeed(),
            dh->getUploadSpeed()
          });

          aria2::deleteDownloadHandle(dh);
        }
      }
    }
    catch(std::exception ex) {
      std::cout << ex.what();
    }
  }

  ariaDataJson["gStats"] = {
    {"gNumActive", gNumActive},
    {"gNumWaiting", gNumWaiting},
    {"gDownloadSpeed", gDownloadSpeed},
    {"gUploadSpeed", gUploadSpeed}
  };

  if(!sessionData.dStat.empty()) {
    // Push into json container
    ariaDataJson["Downloads"] = sessionData;
  }

  // Serialize json data
  ariaDataSerialized = ariaDataJson.dump();

  // Sleep to limit cycle
  std::this_thread::sleep_for(std::chrono::seconds(1));

  if(MonitoringManager::isStopMonitoring) {
    this->completeSignal.set_value();
  }
}