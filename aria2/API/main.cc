#include <node.h>
#include <chrono>
#include <iostream>
#include <nan.h>
#include <aria2/aria2.h>
#include "downloadWorker.h"
#include "pauseWorker.h"

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

aria2::Session* session;

NAN_METHOD(addUrl) {
  v8::String::Utf8Value* params;
  params = new v8::String::Utf8Value(info[0]->ToString());
  std::string uri = std::string(**params);
  delete params;

  params = new v8::String::Utf8Value(info[1]->ToString());
  std::string uri2 = std::string(**params);
  delete params;

  std::vector<std::string> uris = {uri, uri2};

  Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());

  Nan::AsyncQueueWorker(new AriaDownloadWorker(callback, uris));
}

NAN_METHOD(pause) {
  Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());

  Nan::AsyncQueueWorker(new AriaPauseWorker(callback));
}

NAN_MODULE_INIT(init) {
  NAN_EXPORT(target, addUrl);
  NAN_EXPORT(target, pause);
}

NODE_MODULE(addon, init)
