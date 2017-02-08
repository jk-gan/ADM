#ifndef PAUSEWORKER_H
#define PAUSEWORKER_H

extern std::map<int, aria2::Session*> sessionMap;

class AriaPauseWorker : public Nan::AsyncWorker {
  public:
      AriaPauseWorker(Nan::Callback *callback, int sesMapNum)
      : Nan::AsyncWorker(callback), sesMapNum(sesMapNum) {
        if(sesMapNum != -9999){
          std::map<int, aria2::Session*>::iterator it;

          it = sessionMap.find(sesMapNum);
          session = it->second;
        }
      }

      ~AriaPauseWorker() {}

      void Execute ();

      void HandleOKCallback ();

      void pauseAll();

  private:
      aria2::Session* session;
      int sesMapNum;
};

#endif
