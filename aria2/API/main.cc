#include <node_api.h>
#include <vector>

//#include "resourceManager.h"
//#include "downloadManager.h"
#include "sessionManager.h"

#include "common.h"
#include "util.h"

SessionManager* sessionManager = SessionManager::getInstance();
//ResourceManager resourceManager = ResourceManager::GetInstance();
//DownloadManager downloadManager = DownloadManager::GetInstance();

//MonitoringWorker monitoringWorker = monitoringWorker::GetInstance();



/**  
 * Create session for new download. 
 * Each session can only start one download at the same time.
 * Unique session ID should be given through function param.
 *
 * Function param:
 * [1] : unique interger : int
 * [2] : callback function : function
**/
napi_value createSession(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  napi_value *argv = new napi_value[2];

  std::vector<napi_valuetype> argTypes = {
    napi_number,
    napi_function
  };

  Util::getArguments(env, args, argc, argv, argTypes);
  sessionManager->createSession(env, argv);

  delete argv;

  return nullptr;
}

/**
 * Kill session is used to stop all downloads.
 *
 * Function param:
 * [1] : callback function : function
**/
napi_value killAllSession(napi_env env, napi_callback_info args) {
  size_t argc = 1;
  napi_value argv[1];

  std::vector<napi_valuetype> argTypes = {
    napi_function
  };

  Util.GetArguments(env, args, argc, argv, argTypes);
  sessionManager.killAllSession(env, argv);

  return nullptr;
}

/**
 * Kill session is used to stop a download.
 *
 * Function param:
 * [1] : session ID : int
 * [2] : callback function : function
**/
napi_value killSession(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  napi_value argv[2];

  std::vector<napi_valuetype> argTypes = {
    napi_number,
    napi_function
  };

  Util.GetArguments(env, args, argc, argv, argTypes);
  sessionManager.killSession(env, argv);

  return nullptr;
}

/**
 * Function param:
 * [1] : callback function : function
**/
/* napi_value pauseAllSession(napi_env env, napi_callback_info args) {
  size_t argc = 1;
  napi_value argv[1];

  std::vector<napi_valuetype> argTypes = {
    napi_function
  };

  Util.GetArguments(env, args, argc, argv, argTypes);
  sessionManager.pauseAllSession(env, argv);

  return nullptr;
} */

/**
 * Function param:
 * [1] : session ID : int
 * [2] : callback function : function
**/
/* napi_value pauseSession(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  napi_value argv[2];

  std::vector<napi_valuetype> argTypes = {
    napi_number,
    napi_function
  };

  Util.GetArguments(env, args, argc, argv, argTypes);
  sessionManager.pauseSession(env, argv);

  return nullptr;
} */

/**
 * Function param:
 * [1] : uri : string
 * [2] : session ID : int
 * [3] : callback function : function
**/
/* napi_value addDownload(napi_env env, napi_callback_info args) {
  size_t argc = 3;
  napi_value argv[3];

  std::vector<napi_valuetype> argTypes = {
    napi_string,
    napi_number,
    napi_function
  };

  Util.GetArguments(env, args, argc, argv, argTypes);
  downloadManager.addDownload(env, argv);

  return nullptr;
} */

/**
 * Function param:
 * [1] : callback function : function
**/
/* napi_value deleteAllDownload(napi_env env, napi_callback_info args) {
  size_t argc = 1;
  napi_value argv[1];

  std::vector<napi_valuetype> argTypes = {
    napi_function
  };

  Util.GetArguments(env, args, argc, argv, argTypes);
  downloadManager.deleteAllDownload(env, argv);

  return nullptr;
} */

/**
 * Function param:
 * [1] : session ID : int
 * [2] : callback function : function
**/
/* napi_value deleteDownload(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  napi_value argv[2];

  std::vector<napi_valuetype> argTypes = {
    napi_number,
    napi_function
  };

  Util.GetArguments(env, args, argc, argv, argTypes);
  downloadManager.deleteAllDownload(env, argv);

  return nullptr;
}

// To be implement
napi_value startMonitoring(napi_env env, napi_callback_info args) {
  return nullptr;
}

// To be implement
napi_value endMonitoring(napi_env env, napi_callback_info args) {
  return nullptr;
} */

// Init functions and objects
napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor properties[] = {
      DECLARE_NAPI_PROPERTY("createSession", createSession),
      DECLARE_NAPI_PROPERTY("killAllSession", killAllSession),
      DECLARE_NAPI_PROPERTY("killSession", killSession),
      //DECLARE_NAPI_PROPERTY("pauseAllSession", pauseAllSession),
      //DECLARE_NAPI_PROPERTY("pauseSession", pauseSession),
      //DECLARE_NAPI_PROPERTY("addDownload", addDownload),
      //DECLARE_NAPI_PROPERTY("deleteAllDownload", deleteAllDownload),
      //DECLARE_NAPI_PROPERTY("deleteDownload", deleteDownload),
      //DECLARE_NAPI_PROPERTY("startMonitoring", startMonitoring),
      //DECLARE_NAPI_PROPERTY("endMonitoring", endMonitoring)
    };

  NAPI_CALL(env, napi_define_properties(
        env, exports, sizeof(properties) / sizeof(*properties), properties));

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);