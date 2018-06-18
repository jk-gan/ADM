#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <string>

#include <aria2/aria2.h>
#include <node_api.h>

class DownloadManager {
  public:
    static DownloadManager* getInstance();
  
    napi_value addDownload(napi_env &env, napi_value *&argv);

  private:
    DownloadManager() {}
    ~DownloadManager() {}

    static DownloadManager* instance;
};

#endif