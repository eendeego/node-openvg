#ifndef V8_HELPERS_H_
#define V8_HELPERS_H_

#include <v8.h>
#include <node.h>

// SCOPE_DECL_* defined in bindings.gyp
#ifdef SCOPE_DECL_PRE_0_11
#define ISOLATE_DECL(isolate)
#define ISOLATE(isolate)
#define ISOLATE_INIT(isolate)
#define SCOPE(isolate) HandleScope scope
#else
#define ISOLATE_DECL(isolate) extern v8::Isolate* isolate;
#define ISOLATE(isolate) v8::Isolate* isolate;
#define ISOLATE_INIT(isolate) isolate = Isolate::GetCurrent();
#define SCOPE(isolate) HandleScope scope(isolate)
#endif

#ifdef V8_CALLBACK_STYLE_PRE_3_20

#define V8_METHOD(method) v8::Handle<v8::Value> method(const v8::Arguments& args)
#define V8_PERSISTENT(value) value->handle_
#define V8_RETURN(result) return result
#define V8_RETURN_UNDEFINED return Undefined()

#define HANDLE handle_
#define SET_TEMPLATE(template, isolate, constructor) template = Persistent<FunctionTemplate>::New(constructor)

#define NEW_INSTANCE(isolate, template) template->GetFunction()->NewInstance()

#else

#define V8_METHOD(method) void method(const v8::FunctionCallbackInfo<v8::Value>& args)
#define V8_PERSISTENT(value) value->persistent()
#define V8_RETURN(result) do { args.GetReturnValue().Set(result);return; } while(0)
#define V8_RETURN_UNDEFINED return

#define HANDLE handle()
#define SET_TEMPLATE(template, isolate, constructor) template.Reset(isolate, constructor)

#define NEW_INSTANCE(isolate, template) Local<FunctionTemplate>::New(isolate, template)->GetFunction()->NewInstance();

#endif

#define V8_METHOD_DECL(method) static V8_METHOD(method)

#define V8_FUNCTION_DECL(method) V8_METHOD(method)
#define V8_FUNCTION(method) static V8_METHOD(method)

#define V8_THROW(exception) V8_RETURN(ThrowException(exception))

#if NODE_MODULE_VERSION <= NODE_0_10_MODULE_VERSION
template<class C> class TypedArrayWrapper {
 private:
  Local<Object> array;
  Handle<Object> buffer;
  int byteOffset;
 public:
  NAN_INLINE TypedArrayWrapper(const Local<Value>& arg) :
    array(arg->ToObject()),
    buffer(array->Get(NanNew<String>("buffer"))->ToObject()),
    byteOffset(array->Get(NanNew<String>("byteOffset"))->Int32Value()) {
  }

  NAN_INLINE C* pointer(int offset = 0) {
    return (C*) &((char*) buffer->GetIndexedPropertiesExternalArrayData())[byteOffset + offset];
  }

  NAN_INLINE int length() {
    return array->Get(NanNew<String>("length"))->Uint32Value();
  }
};
#else
template<class C> class TypedArrayWrapper {
 private:
  Local<TypedArray> array;
 public:
  NAN_INLINE TypedArrayWrapper(const Local<Value>& arg) :
    array(Handle<TypedArray>::Cast(arg->ToObject())) {
  }

  NAN_INLINE C* pointer(int offset = 0) {
    return (C*) &((char*) array->BaseAddress())[offset];
  }

  NAN_INLINE int length() {
    return array->Length();
  }
};
#endif

#endif
