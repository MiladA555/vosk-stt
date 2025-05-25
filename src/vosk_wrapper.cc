#include <napi.h>
#include "vosk_api.h"

/******************** Model Wrapper ********************/
class Model : public Napi::ObjectWrap<Model> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "Model", {
            InstanceMethod("free", &Model::Free),
        });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set("Model", func);
        return exports;
    }

    Model(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Model>(info) {
        Napi::Env env = info.Env();
        
        if (info.Length() < 1) {
            Napi::TypeError::New(env, "Expected model path").ThrowAsJavaScriptException();
            return;
        }

        std::string model_path = info[0].As<Napi::String>().Utf8Value();
        model = vosk_model_new(model_path.c_str());
    }

    VoskModel* GetModel() { return model; }

private:
    static Napi::FunctionReference constructor;
    VoskModel* model;

    Napi::Value Free(const Napi::CallbackInfo& info) {
        if (model) {
            vosk_model_free(model);
            model = nullptr;
        }
        return info.Env().Undefined();
    }
};

Napi::FunctionReference Model::constructor;

/******************** Recognizer Wrapper ********************/
class Recognizer : public Napi::ObjectWrap<Recognizer> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "Recognizer", {
            InstanceMethod("acceptWaveform", &Recognizer::AcceptWaveform),
            InstanceMethod("result", &Recognizer::Result),
            InstanceMethod("partialResult", &Recognizer::PartialResult),
            InstanceMethod("finalResult", &Recognizer::FinalResult),
            InstanceMethod("free", &Recognizer::Free),
        });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set("Recognizer", func);
        return exports;
    }

    Recognizer(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Recognizer>(info) {
        Napi::Env env = info.Env();
        
        if (info.Length() < 2) {
            Napi::TypeError::New(env, "Expected model and sample rate").ThrowAsJavaScriptException();
            return;
        }

        Model* model = Napi::ObjectWrap<Model>::Unwrap(info[0].As<Napi::Object>());
        int sample_rate = info[1].As<Napi::Number>().Int32Value();
        
        recognizer = vosk_recognizer_new(model->GetModel(), sample_rate);
    }

private:
    static Napi::FunctionReference constructor;
    VoskRecognizer* recognizer;

    Napi::Value AcceptWaveform(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        
        if (info.Length() < 1) {
            Napi::TypeError::New(env, "Expected audio buffer").ThrowAsJavaScriptException();
            return env.Null();
        }

        Napi::Buffer<char> buffer = info[0].As<Napi::Buffer<char>>();
        int result = vosk_recognizer_accept_waveform(recognizer, buffer.Data(), buffer.Length());
        
        return Napi::Boolean::New(env, result);
    }

    Napi::Value Result(const Napi::CallbackInfo& info) {
        const char* result = vosk_recognizer_result(recognizer);
        return Napi::String::New(info.Env(), result);
    }

    Napi::Value PartialResult(const Napi::CallbackInfo& info) {
        const char* result = vosk_recognizer_partial_result(recognizer);
        return Napi::String::New(info.Env(), result);
    }

    Napi::Value FinalResult(const Napi::CallbackInfo& info) {
        const char* result = vosk_recognizer_final_result(recognizer);
        return Napi::String::New(info.Env(), result);
    }

    Napi::Value Free(const Napi::CallbackInfo& info) {
        if (recognizer) {
            vosk_recognizer_free(recognizer);
            recognizer = nullptr;
        }
        return info.Env().Undefined();
    }
};

Napi::FunctionReference Recognizer::constructor;

/******************** Module Init ********************/
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Model::Init(env, exports);
    Recognizer::Init(env, exports);
    return exports;
}

NODE_API_MODULE(vosk, Init)