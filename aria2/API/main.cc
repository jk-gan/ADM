#include <node.h>
#include <chrono>
#include <iostream>
#include <map>
#include <nan.h>
#include <aria2/aria2.h>
#include "downloadWorker.h"
#include "pauseWorker.h"
#include "sessionWorker.h"

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

std::map<int, aria2::Session*> sessionMap;

NAN_METHOD(createSession) {
  int sesMapNum = info[0]->Uint32Value();
  Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

  Nan::AsyncQueueWorker(new AriaSessionWorker(callback, true, sesMapNum));
}

NAN_METHOD(killAllSession) {
  Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
  int sesMapNum = -99999;

  Nan::AsyncQueueWorker(new AriaSessionWorker(callback, false, sesMapNum));
}

NAN_METHOD(addUrl) {
  int sesMapNum = info[0]->Uint32Value();

  v8::String::Utf8Value* params;
  params = new v8::String::Utf8Value(info[1]->ToString());
  std::string uri = std::string(**params);
  delete params;

  params = new v8::String::Utf8Value(info[2]->ToString());
  std::string uri2 = std::string(**params);
  delete params;

  std::vector<std::string> uris = {uri, uri2};

  Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());

  Nan::AsyncQueueWorker(new AriaDownloadWorker(callback, uris, sesMapNum));
}

NAN_METHOD(pause) {
  int sesMapNum = info[0]->Uint32Value();
  Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

  Nan::AsyncQueueWorker(new AriaPauseWorker(callback, sesMapNum));
}

NAN_METHOD(stop) {
  int sesMapNum = info[0]->Uint32Value();
  Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

  Nan::AsyncQueueWorker(new AriaSessionWorker(callback, false, sesMapNum));
}

NAN_MODULE_INIT(init) {
  NAN_EXPORT(target, createSession);
  NAN_EXPORT(target, killAllSession);
  NAN_EXPORT(target, stop);
  NAN_EXPORT(target, addUrl);
  NAN_EXPORT(target, pause);
}

NODE_MODULE(addon, init)
