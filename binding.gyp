{
  "targets": [
    {
      "target_name": "main",
      "include_dirs": [ "/usr/local/include/node/", "<(module_root_dir)/aria2/API" ],
      "conditions": [
        ['OS=="linux"', {
          "libraries": ["-laria2"],
        }]
      ],
      "cflags": [ "-Wall -O2 -g -std=c++11" ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      "sources": [
                    "aria2/API/main.cc",
                    "aria2/API/sessionManager.cc",
                    "aria2/API/sessionWorker.cc",
                    "aria2/API/downloadManager.cc",
                    "aria2/API/downloadWorker.cc",
                    "aria2/API/monitoringManager.cc",
                    "aria2/API/util.cc",
                    "aria2/API/json.hpp"
                 ]
    }
  ]
}
