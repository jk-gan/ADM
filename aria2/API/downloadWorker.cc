#include <node.h>
#include <aria2/aria2.h>
#include <nan.h>
#include <iostream>
#include <chrono>
#include "downloadWorker.h"

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void AriaDownloadWorker::Execute () {
    download(uris);
}

void AriaDownloadWorker::HandleOKCallback () {
    Nan:: HandleScope scope;

    v8::Local<v8::String> results = String::NewFromUtf8(v8::Isolate::GetCurrent(), "yeah");


    Local<Value> argv[] = {
        Nan::Null(),
        results
    };

    callback->Call(2, argv);

}

int downloadEventCallback(aria2::Session* session, aria2::DownloadEvent event,
                          aria2::A2Gid gid, void* userData)
{
  switch (event) {
  case aria2::EVENT_ON_DOWNLOAD_COMPLETE:
    std::cerr << "COMPLETE";
    break;
  case aria2::EVENT_ON_DOWNLOAD_ERROR:
    std::cerr << "ERROR";
    break;
  default:
    return 0;
  }
  std::cerr << " [" << aria2::gidToHex(gid) << "] ";
  aria2::DownloadHandle* dh = aria2::getDownloadHandle(session, gid);
  if (!dh)
    return 0;
  if (dh->getNumFiles() > 0) {
    aria2::FileData f = dh->getFile(1);
    // Path may be empty if the file name has not been determined yet.
    if (f.path.empty()) {
      if (!f.uris.empty()) {
        std::cerr << f.uris[0].uri;
      }
    }
    else {
      std::cerr << f.path;
    }
  }
  aria2::deleteDownloadHandle(dh);
  std::cerr << std::endl;
  return 0;
}

void download(std::vector<std::string> uris) {
  int rv;
  if (uris.size() < 2) {
    std::cerr << "Usage: libaria2ex URI [URI...]\n"
              << "\n"
              << "  Download given URIs in parallel in the current directory."
              << std::endl;
    exit(EXIT_SUCCESS);
  }
  aria2::libraryInit();
  // session is actually singleton: 1 session per process

  // Create default configuration. The libaria2 takes care of signal
  // handling.
  aria2::SessionConfig config;
  // Add event callback
  config.downloadEventCallback = downloadEventCallback;
  config.keepRunning = true;
  session = aria2::sessionNew(aria2::KeyVals(), config);
  // Add download item to session
  for (int i = 1; i < uris.size(); ++i) {
    aria2::KeyVals options;
    rv = aria2::addUri(session, nullptr, uris, options);
    if (rv < 0) {
      std::cerr << "Failed to add download " << uris[0] << std::endl;
    }
  }
  auto start = std::chrono::steady_clock::now();

  for (;;) {
    rv = aria2::run(session, aria2::RUN_ONCE);
    aria2::GlobalStat gstat = aria2::getGlobalStat(session);
    auto num = gstat.numActive;
    if(num == 0) {
      aria2::shutdown(session);
      break;
    }

    if (rv != 1) {
      break;
    }
    // the application can call aria2 API to add URI or query progress
    // here
    auto now = std::chrono::steady_clock::now();
    auto count =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count();

    // Print progress information once per 500ms
    if (count >= 2000) {
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
  rv = aria2::sessionFinal(session);

  aria2::libraryDeinit();
}
