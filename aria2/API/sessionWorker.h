#ifndef SESSIONWORKER_H
#define SESSIONWORKER_H

int downloadEventCallback(aria2::Session*, aria2::DownloadEvent, aria2::A2Gid, void*);
void sessionInit();
void sessionKill();

extern aria2::Session* session;

class AriaSessionWorker : public Nan::AsyncWorker {
  public:
      AriaSessionWorker(Nan::Callback *callback, bool isCreate)
      : Nan::AsyncWorker(callback), isCreate(isCreate){}

      ~AriaSessionWorker() {}

      void Execute ();

      void HandleOKCallback ();

  private:
    bool isCreate;
};

#endif
