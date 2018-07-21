#ifndef MONITORINGMANAGER_H
#define MONITORINGMANAGER_H

#include <string>
#include <vector>
#include <future>
#include <queue>

#include "node_api.h"

#include "common.h"

using std::unique_ptr;
using std::shared_ptr;

namespace monitoring {
  struct SessionGStatData {
    int gNumActive;
    int gNumWaiting;
    double gDownloadSpeed;
    double gUploadSpeed;
  };

  struct DownloadStatData {
    std::string gid;
    std::string fileName;
    int64_t completedLength;
    int64_t totalLength;
    double downloadSpeed;
    double uploadSpeed;
  };

  struct SessionData {
    std::string sessionID;
    SessionGStatData gStat;
    std::vector<DownloadStatData> dStat;
  };

  struct DownloadCallbackSignal {
    aria2::DownloadEvent evt;
    std::string gid;
    std::string fileName;
  };
}

void ExecuteMonitoring(napi_env env, void *data);

void CompleteMonitoring(napi_env env, napi_status status, void *data);

class MonitoringManager {
  public:
    static bool isStopMonitoring;

    static MonitoringManager* getInstance();

    friend void ExecuteMonitoring(napi_env env, void *data);
    friend void CompleteMonitoring(napi_env env, napi_status status, void *data);

    napi_value startMonitoring(napi_env &env, shared_ptr<napi_value[]> argv);
    napi_value stopMonitoring(napi_env &env);

    void listenAria2();
    void addDownloadEventSignal(aria2::DownloadEvent evt, std::string gid, std::string fileName);

  private:
    MonitoringManager() { 
      isStopMonitoring = true;
      futureObj = completeSignal.get_future();
    }
    ~MonitoringManager() {}

    std::promise<void> completeSignal;
    std::future<void> futureObj;
    std::queue<monitoring::DownloadCallbackSignal> completeSignalQueue;
    
    napi_env env;
    napi_ref eventHandler;
    napi_ref completeHandler;
    napi_async_work request;

    std::string ariaDataSerialized;
};

#endif