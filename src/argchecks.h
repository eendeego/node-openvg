#ifndef ARG_CHECKS_H_
#define ARG_CHECKS_H_

#include <node_api.h>

#define DeclareArgs(argc, args, count) \
size_t argc = count; \
napi_value args[count];

#if defined(ARG_CHECKS)

#define BASE_MESSAGE "Invalid arguments: Expected "
#define THROW(fn, args) NanThrowTypeError(BASE_MESSAGE #fn "(" args ")")

#define ASSERT_ARG_COUNT(env, info, fn, argc, args, count) \
NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));\
NAPI_ASSERT(env, argc == count, #fn ": Wrong number of arguments");

#define ASSERT_TYPE(env, fn, args, arg_index, type) \
do {\
  napi_valuetype valuetype;\
  NAPI_CALL(env, napi_typeof(env, args[arg_index - 1], &valuetype));\
  NAPI_ASSERT(env, valuetype == napi_##type,\
    #fn ": Wrong argument[" #arg_index "] type: " #type " expected.");\
} while(0)


#define CheckArgs0(env, info, fn, argc, args); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 0)

#define CheckArgs1(env, info, fn, argc, args, type1); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 1); \
ASSERT_TYPE(env, fn, args, 1, type1)

#define CheckArgs2(env, info, fn, argc, args, type1, type2); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 2); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2)

#define CheckArgs3(env, info, fn, argc, args, type1, type2, type3); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 3); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3)

#define CheckArgs4(env, info, fn, argc, args, type1, type2, type3, type4); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 4); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4)

#define CheckArgs5(env, info, fn, argc, args, type1, type2, type3, type4, type5); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 5); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5)

#define CheckArgs6(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 6); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6)

#define CheckArgs7(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6, type7); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 7); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6); \
ASSERT_TYPE(env, fn, args, 7, type7)

#define CheckArgs8(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6, type7, type8); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 8); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6); \
ASSERT_TYPE(env, fn, args, 7, type7); \
ASSERT_TYPE(env, fn, args, 8, type8)

#define CheckArgs9(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6, type7, type8, type9); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 9); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6); \
ASSERT_TYPE(env, fn, args, 7, type7); \
ASSERT_TYPE(env, fn, args, 8, type8); \
ASSERT_TYPE(env, fn, args, 9, type9)

#define CheckArgs10(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 10); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6); \
ASSERT_TYPE(env, fn, args, 7, type7); \
ASSERT_TYPE(env, fn, args, 8, type8); \
ASSERT_TYPE(env, fn, args, 9, type9); \
ASSERT_TYPE(env, fn, args, 10, type10)

#define CheckArgs11(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 11); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6); \
ASSERT_TYPE(env, fn, args, 7, type7); \
ASSERT_TYPE(env, fn, args, 8, type8); \
ASSERT_TYPE(env, fn, args, 9, type9); \
ASSERT_TYPE(env, fn, args, 10, type10); \
ASSERT_TYPE(env, fn, args, 11, type11)

#define CheckArgs12(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 12); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6); \
ASSERT_TYPE(env, fn, args, 7, type7); \
ASSERT_TYPE(env, fn, args, 8, type8); \
ASSERT_TYPE(env, fn, args, 9, type9); \
ASSERT_TYPE(env, fn, args, 10, type10); \
ASSERT_TYPE(env, fn, args, 11, type11); \
ASSERT_TYPE(env, fn, args, 12, type12)

#define CheckArgs17(env, info, fn, argc, args, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12, type13, type14, type15, type16, type17); \
ASSERT_ARG_COUNT(env, info, fn, argc, args, 17); \
ASSERT_TYPE(env, fn, args, 1, type1); \
ASSERT_TYPE(env, fn, args, 2, type2); \
ASSERT_TYPE(env, fn, args, 3, type3); \
ASSERT_TYPE(env, fn, args, 4, type4); \
ASSERT_TYPE(env, fn, args, 5, type5); \
ASSERT_TYPE(env, fn, args, 6, type6); \
ASSERT_TYPE(env, fn, args, 7, type7); \
ASSERT_TYPE(env, fn, args, 8, type8); \
ASSERT_TYPE(env, fn, args, 9, type9); \
ASSERT_TYPE(env, fn, args, 10, type10); \
ASSERT_TYPE(env, fn, args, 11, type11); \
ASSERT_TYPE(env, fn, args, 12, type12); \
ASSERT_TYPE(env, fn, args, 13, type13); \
ASSERT_TYPE(env, fn, args, 14, type14); \
ASSERT_TYPE(env, fn, args, 15, type15); \
ASSERT_TYPE(env, fn, args, 16, type16); \
ASSERT_TYPE(env, fn, args, 17, type17)

#else

#define CheckArgs0(env, info, fn, argc, args)
#define CheckArgs1(env, info, fn, argc, args, type0)
#define CheckArgs2(env, info, fn, argc, args, type0, type1)
#define CheckArgs3(env, info, fn, argc, args, type0, type1, type2)
#define CheckArgs4(env, info, fn, argc, args, type0, type1, type2, type3)
#define CheckArgs5(env, info, fn, argc, args, type0, type1, type2, type3, type4)
#define CheckArgs6(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5)
#define CheckArgs7(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5, type6)
#define CheckArgs8(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5, type6, type7)
#define CheckArgs9(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5, type6, type7, type8)
#define CheckArgs10(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9)
#define CheckArgs11(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10)
#define CheckArgs12(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11)
#define CheckArgs17(env, info, fn, argc, args, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12, type13, type14, type15, type16)

#endif

#endif
