#ifndef VOSK_WRAPPER_H
#define VOSK_WRAPPER_H

#include <napi.h>
#include "vosk_api.h"  // From Vosk API

// Model wrapper class
class Model : public Napi::ObjectWrap<Model> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Model(const Napi::CallbackInfo& info);
    
private:
    static Napi::FunctionReference constructor;
    VoskModel* model;
    
    Napi::Value Free(const Napi::CallbackInfo& info);
};

// Initialize the module
Napi::Object Init(Napi::Env env, Napi::Object exports);

#endif