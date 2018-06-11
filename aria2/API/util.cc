#include "util.h"

#include <vector>

// Static function for getArguments of functions
napi_value Util::getArguments(napi_env env, napi_callback_info args, size_t argc, napi_value *&argv, std::vector<napi_valuetype> argumentsType) {
  size_t localArgc = argc;
  napi_value _this;
  void *data; 
  
  NAPI_CALL(env, napi_get_cb_info(env, args, &localArgc, argv, &_this, &data));
  NAPI_ASSERT(env, localArgc >= 2, "Not enough arguments, expected 2.");

  short counter = 0;

  for(const auto typeItem: argumentsType) {
    napi_valuetype t;

    NAPI_CALL(env, napi_typeof(env, argv[counter], &t));
    NAPI_ASSERT(env, t == typeItem, "Wrong argument");

    counter++;
  }

  return nullptr;
}