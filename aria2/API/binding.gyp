{
  "targets": [
    {
      "target_name": "main",
      "conditions": [
        ['OS=="linux"', {
          "ldflags": [
            "-Wl,-rpath,/usr/local/lib64/",
          ],
          "libraries": ["-laria2"],
        }]
      ],
      "cflags": [ "-Wall -O2 -g -std=c++11" ],
      "sources": [ "main.cc" ]
    }
  ]
}
