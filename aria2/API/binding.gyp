{
  "targets": [
    {
      "target_name": "main",
      "include_dirs": [ "/usr/local/include/node/", "/home/plwai/ADM/aria2/API" , "<!(node -e \"require('nan')\")" ],
      "conditions": [
        ['OS=="linux"', {
          "ldflags": [
            "-Wl,-rpath,/usr/local/lib64/",
          ],
          "libraries": ["-laria2"],
        }]
      ],
      "cflags": [ "-Wall -O2 -g -std=c++11" ],
      "sources": [
                    "main.cc",
                    "sessionManager.cc",
                    "sessionWorker.cc",
                    "util.cc"
                 ]
    }
  ]
}
