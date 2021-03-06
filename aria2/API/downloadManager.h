#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <string>
#include <memory>

#include <aria2/aria2.h>
#include <node_api.h>

enum DownloadOption {
  NEW,
  RESUME
};

using std::shared_ptr;
using std::unique_ptr;

class DownloadManager {
  public:
    static DownloadManager* getInstance();
  
    napi_value addDownload(napi_env &env, shared_ptr<napi_value[]> argv, DownloadOption option = NEW);
    napi_value removeDownloadWithGid(napi_env &env, shared_ptr<napi_value[]> argv);

  private:
    DownloadManager() {}
    ~DownloadManager() {}
};

#endif