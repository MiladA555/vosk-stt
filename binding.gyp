{
  "targets": [
    {
      "target_name": "vosk_stt",
      "sources": [
        "src/vosk_wrapper.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "third_party/include"
      ],
      "libraries": [
        "-L<!(pwd)/third_party/lib",
        "-lvosk",
        "-lkaldi-util"
      ],
      "defines": [
        "NAPI_CPP_EXCEPTIONS"
      ],
      "cflags_cc": [ "-fexceptions" ],
      "conditions": [
        ["OS=='linux'", {
          "libraries": [
            "-Wl,-rpath,'$$ORIGIN/../third_party/lib'"
          ]
        }]
      ]
    }
  ]
}
