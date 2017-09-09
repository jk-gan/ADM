#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H



extern std::map<int, aria2::Session*> sessionMap;

class AriaDownloadWorker : public Nan::AsyncWorker {
  public:
      AriaDownloadWorker(Nan::Callback *callback, std::vector<std::string> uris, int sesMapNum)
      : Nan::AsyncWorker(callback), uris(uris), sesMapNum(sesMapNum) {
          session = sessionMap[sesMapNum];
      }

      ~AriaDownloadWorker() {}

      void Execute ();

      void HandleOKCallback ();

      void download(std::vector<std::string>);

  private:
      std::vector<std::string> uris;
      aria2::Session* session;
      int sesMapNum;
};


#endif
