#include <node.h>
#include <aria2/aria2.h>
#include <nan.h>
#include <iostream>
#include <chrono>
#include "pauseWorker.h"

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void pauseAll(){
  std::cout << "Pausing " << std::endl;
  std::vector<aria2::A2Gid> allGids = aria2::getActiveDownload(session);

  for(const auto& gid : allGids){
    aria2::pauseDownload(session, gid);
    std::cout << aria2::gidToHex(gid) << std::endl;
  }
}

void AriaPauseWorker::Execute () {
    pauseAll();
}

void AriaPauseWorker::HandleOKCallback () {
    Nan:: HandleScope scope;

    v8::Local<v8::String> results = String::NewFromUtf8(v8::Isolate::GetCurrent(), "paused");


    Local<Value> argv[] = {
        Nan::Null(),
        results
    };

    callback->Call(2, argv);

}
