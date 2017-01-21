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

      // We have the results, and we're back in the event loop.
      void HandleOKCallback ();
  private:
};

#endif
