#ifndef SESSION_CONTAINER
#define SESSION_CONTAINER

#include <thread>
#include <future>
#include <aria2/aria2.h>

class SessionContainer {
  public:
    SessionContainer(aria2::Session* session){
      this->session = session;
    }

    std::thread sessionWorker;
    std::promise<void> exitSignal;
    aria2::Session* session;
};

#endif