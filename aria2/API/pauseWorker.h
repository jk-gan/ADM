#ifndef PAUSEWORKER_H
#define PAUSEWORKER_H

void pauseAll();

extern aria2::Session* session;

class AriaPauseWorker : public Nan::AsyncWorker {
  public:
      AriaPauseWorker(Nan::Callback *callback)
      : Nan::AsyncWorker(callback){}

      ~AriaPauseWorker() {}

      void Execute ();

      void HandleOKCallback ();
  private:
};

#endif
