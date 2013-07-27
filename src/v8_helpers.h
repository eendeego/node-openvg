#ifndef V8_HELPERS_H_
#define V8_HELPERS_H_

// V8_CALLBACK_STYLE_* defined in bindings.gyp
#ifdef V8_CALLBACK_STYLE_PRE_3_20
#define V8_METHOD(method) v8::Handle<v8::Value> method(const v8::Arguments& args)
#define V8_RETURN(result) return result
#else
#define V8_METHOD(method) void method(const v8::FunctionCallbackInfo<v8::Value>& args)
#define V8_RETURN(result) do { args.GetReturnValue().Set(result);return; } while(0)
#endif

#define V8_METHOD_DECL(method) static V8_METHOD(method)

#define V8_FUNCTION_DECL(method) V8_METHOD(method)
#define V8_FUNCTION(method) static V8_METHOD(method)

#define V8_THROW(exception) V8_RETURN(ThrowException(exception))

#endif
