#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

void download(std::vector<std::string>);

extern aria2::Session* session;

class AriaDownloadWorker : public Nan::AsyncWorker {
  public:
      AriaDownloadWorker(Nan::Callback *callback, std::vector<std::string> uris)
      : Nan::AsyncWorker(callback), uris(uris) {}

      ~AriaDownloadWorker() {}

      void Execute ();

      void HandleOKCallback ();

  private:
      std::vector<std::string> uris;
};


#endif
