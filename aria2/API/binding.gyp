{
  "targets": [
    {
      "target_name": "main",
      "include_dirs": [ "/usr/local/include/node/", "/home/plwai/ADM/aria2/API" ],
      "conditions": [
        ['OS=="linux"', {
          "ldflags": [
            "-Wl,-rpath,/usr/local/lib64/",
          ],
          "libraries": ["-laria2"],
        }]
      ],
      "cflags": [ "-Wall -O2 -g -std=c++11" ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      "sources": [
                    "main.cc",
                    "sessionManager.cc",
                    "sessionWorker.cc",
                    "downloadManager.cc",
                    "downloadWorker.cc",
                    "monitoringManager.cc",
                    "util.cc",
                    "json.hpp"
                 ]
    }
  ]
}
