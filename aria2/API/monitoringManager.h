#ifndef MONITORINGMANAGER_H
#define MONITORINGMANAGER_H

#include <string>
#include <vector>
#include <future>

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

  private:
    MonitoringManager() { 
      isStopMonitoring = true;
      futureObj = completeSignal.get_future();
    }
    ~MonitoringManager() {}

    std::promise<void> completeSignal;
    std::future<void> futureObj;
    
    napi_env env;
    napi_ref eventHandler;
    napi_async_work request;

    std::string ariaDataSerialized;
};

#endif