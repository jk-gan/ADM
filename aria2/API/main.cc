#include <node_api.h>
#include <vector>

//#include "resourceManager.h"
#include "downloadManager.h"
#include "sessionManager.h"
#include "monitoringManager.h"

#include "common.h"
#include "util.h"

using std::unique_ptr;
using std::shared_ptr;

SessionManager* sessionManager = SessionManager::getInstance();
DownloadManager* downloadManager = DownloadManager::getInstance();
//ResourceManager resourceManager = ResourceManager::GetInstance();

MonitoringManager* monitoringManager = MonitoringManager::getInstance();

/**  
 * Initialize Aria2
 *
**/
napi_value ariaInit(napi_env env, napi_callback_info args) {
  napi_value returnNum;

  int result = sessionManager->ariaInit();

  napi_status status = napi_create_int32(env, result, &returnNum);

  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return returnNum;
}

/**  
 * Destroy Aria2
 *
**/
napi_value ariaDeInit(napi_env env, napi_callback_info args) {
  napi_value returnNum;

  int result = sessionManager->ariaDeInit();

  napi_status status = napi_create_int32(env, result, &returnNum);

  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return returnNum;
}

/**  
 * Create session for new download. 
 * Each session can only start one download at the same time.
 * Unique session ID should be given through function param.
 *
 * Function param:
 * [1] : unique string : string
 * [2] : callback function : function
**/
napi_value createSession(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  shared_ptr<napi_value[]> argv(new napi_value[2]);

  std::vector<napi_valuetype> argTypes = {
    napi_string,
    napi_function
  };
  
  Util::getArguments(env, args, argc, argv, argTypes);
  sessionManager->createSession(env, argv);

  return nullptr;
}

/**
 * Kill session is used to stop all downloads.
 *
 * Function param:
 * [1] : callback function : function
**/
napi_value killAllSession(napi_env env, napi_callback_info args) {
  sessionManager->killAllSession(env);

  return nullptr;
}

/**
 * Kill session is used to stop a download.
 *
 * Function param:
 * [1] : session ID : string
 * [2] : callback function : function
**/
napi_value killSession(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  shared_ptr<napi_value[]> argv(new napi_value[2]);

  std::vector<napi_valuetype> argTypes = {
    napi_string,
    napi_function
  };

  Util::getArguments(env, args, argc, argv, argTypes);
  sessionManager->killSession(env, argv);

  return nullptr;
}

/**
 * Function param:
 * [1] : callback function : function
**/
napi_value pauseAllSession(napi_env env, napi_callback_info args) {
  size_t argc = 1;
  shared_ptr<napi_value[]> argv(new napi_value[1]);

  std::vector<napi_valuetype> argTypes = {
    napi_function
  };

  Util::getArguments(env, args, argc, argv, argTypes);
  sessionManager->pauseAllSession(env, argv);

  return nullptr;
}

/**
 * Function param:
 * [1] : session ID : string
 * [2] : callback function : function
**/
napi_value pauseSession(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  shared_ptr<napi_value[]> argv(new napi_value[2]);

  std::vector<napi_valuetype> argTypes = {
    napi_string,
    napi_function
  };

  Util::getArguments(env, args, argc, argv, argTypes);
  sessionManager->pauseSession(env, argv);

  return nullptr;
}

/**
 * Function param:
 * [1] : uri : string
 * [2] : session ID : string
 * [3] : filename : string optional
 * [4] : callback function : function
**/
 napi_value addDownload(napi_env env, napi_callback_info args) {
  size_t argc = 4;
  shared_ptr<napi_value[]> argv(new napi_value[4]);

  std::vector<napi_valuetype> argTypes = {
    napi_string,
    napi_string,
    napi_string,
    napi_function
  };

  Util::getArguments(env, args, argc, argv, argTypes);
  downloadManager->addDownload(env, argv);

  return nullptr;
} 

/**
 * Function param:
 * [1] : uri : string
 * [2] : session ID : string
 * [3] : filename : string optional
 * [4] : callback function : function
**/
napi_value resumeDownload(napi_env env, napi_callback_info args) {
  size_t argc = 4;
  shared_ptr<napi_value[]> argv(new napi_value[4]);

  std::vector<napi_valuetype> argTypes = {
    napi_string,
    napi_string,
    napi_string,
    napi_function
  };

  Util::getArguments(env, args, argc, argv, argTypes);
  downloadManager->addDownload(env, argv, RESUME);

  return nullptr;
}

napi_value startMonitoring(napi_env env, napi_callback_info args) {
  size_t argc = 2;
  shared_ptr<napi_value[]> argv(new napi_value[2]);

  std::vector<napi_valuetype> argTypes = {
    napi_function,
    napi_function
  };

  Util::getArguments(env, args, argc, argv, argTypes);

  napi_value result = monitoringManager->startMonitoring(env, argv);

  return result;
}


napi_value stopMonitoring(napi_env env, napi_callback_info args) {
  napi_value result = monitoringManager->stopMonitoring(env);

  return result;
} 

// Init functions and objects
napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor properties[] = {
      DECLARE_NAPI_PROPERTY("ariaInit", ariaInit),
      DECLARE_NAPI_PROPERTY("ariaDeInit", ariaDeInit),
      DECLARE_NAPI_PROPERTY("createSession", createSession),
      DECLARE_NAPI_PROPERTY("killAllSession", killAllSession),
      DECLARE_NAPI_PROPERTY("killSession", killSession),
      DECLARE_NAPI_PROPERTY("pauseAllSession", pauseAllSession),
      DECLARE_NAPI_PROPERTY("pauseSession", pauseSession),
      DECLARE_NAPI_PROPERTY("addDownload", addDownload),
      DECLARE_NAPI_PROPERTY("resumeDownload", resumeDownload),
      //DECLARE_NAPI_PROPERTY("deleteAllDownload", deleteAllDownload),
      //DECLARE_NAPI_PROPERTY("deleteDownload", deleteDownload),
      DECLARE_NAPI_PROPERTY("startMonitoring", startMonitoring),
      DECLARE_NAPI_PROPERTY("stopMonitoring", stopMonitoring)
    };

  NAPI_CALL(env, napi_define_properties(
        env, exports, sizeof(properties) / sizeof(*properties), properties));

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);