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

    v8::Local<v8::String> results = String::NewFromUtf8(v8::Isolate::GetCurrent(), "download end");


    Local<Value> argv[] = {
        Nan::Null(),
        results
    };

    callback->Call(2, argv);

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
}
