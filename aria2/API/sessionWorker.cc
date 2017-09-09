#include <node.h>
#include <aria2/aria2.h>
#include <nan.h>
#include <iostream>
#include <chrono>
#include <map>
#include "sessionWorker.h"

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void AriaSessionWorker::Execute () {
  if(isCreate) {
    sessionInit();
  }
  else if(sesMapNum == -99999) {
    sessionAllKill();
  }
  else {
    sessionKill();
  }

}

void AriaSessionWorker::HandleOKCallback () {
  Nan:: HandleScope scope;

  v8::Local<v8::Number> results = Number::New(v8::Isolate::GetCurrent(), sesMapNum);


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

void AriaSessionWorker::sessionInit() {
  aria2::libraryInit();

  aria2::SessionConfig config;

  config.downloadEventCallback = downloadEventCallback;
  config.keepRunning = true;
  session = aria2::sessionNew(aria2::KeyVals(), config);

  sessionMap.insert ( std::pair<int, aria2::Session*>(sesMapNum, session) );
}

void AriaSessionWorker::sessionKill() {
  std::map<int, aria2::Session*>::iterator it;

  it = sessionMap.find(sesMapNum);
  session = it->second;

  aria2::shutdown(session);
  aria2::libraryDeinit();

  sessionMap.erase(it);
  session = nullptr;
}

void AriaSessionWorker::sessionAllKill() {
  std::map<int, aria2::Session*>::iterator it;

  for(it = sessionMap.begin(); it != sessionMap.end(); it++){
    aria2::sessionFinal(it->second);
    aria2::shutdown(it->second);
  }

  aria2::libraryDeinit();
  sessionMap.clear();
}
