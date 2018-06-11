#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <map>

#include <node_api.h>

#include "common.h"

// Utilities for some common function
class Util {
  public:
    static napi_value getArguments(napi_env env, napi_callback_info args, size_t argc, napi_value *&argv, std::vector<napi_valuetype> argumentsType);

  private:
    Util() {}
};

#endif