#include "VG/openvg.h"
#include "VG/vgu.h"
#include <v8.h>

#include "openvg.h"
#include "egl.h"

#include <stdio.h>

using namespace node;
using namespace v8;

extern "C" void
init(Handle<Object> target)
{
  NODE_SET_METHOD(target, "init", openvg::Init);
  NODE_SET_METHOD(target, "finish", openvg::Finish);
}

// init sets the system to its initial state
void vgInit() {
  egl::Init();

  printf("vgInit done!\n");
}

void vgFinish() {
  egl::Finish();
}

Handle<Value> openvg::Init(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Init()")));
  }

  vgInit();

  printf("openvg::Init done!\n");
  return Undefined();
}

Handle<Value> openvg::Finish(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Finish()")));
  }

  vgFinish();

  return Undefined();
}
