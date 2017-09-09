#ifndef SESSIONWORKER_H
#define SESSIONWORKER_H

int downloadEventCallback(aria2::Session*, aria2::DownloadEvent, aria2::A2Gid, void*);

extern std::map<int, aria2::Session*> sessionMap;

class AriaSessionWorker : public Nan::AsyncWorker {
  public:
      AriaSessionWorker(Nan::Callback *callback, bool isCreate, int sesMapNum)
      : Nan::AsyncWorker(callback), isCreate(isCreate), sesMapNum(sesMapNum){}

      ~AriaSessionWorker() {}

      void Execute ();

      void HandleOKCallback ();

      void sessionInit();

      void sessionKill();

      void sessionAllKill();

  private:
    bool isCreate;
    int sesMapNum;
    aria2::Session* session;
};

#endif
