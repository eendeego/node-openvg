#ifndef ARG_CHECKS_H_
#define ARG_CHECKS_H_

#if defined(ARG_CHECKS)

#define BASE_MESSAGE "Invalid arguments: Expected "
#define THROW(fn, args) return ThrowException(Exception::TypeError(String::New(BASE_MESSAGE #fn "(" args ")")))

#define CheckArgs0(fn) \
do {\
  if (!(args.Length() == 0)) {\
    THROW(fn, );\
  }\
} while(0)

#define CheckArgs1(fn, arg0, type0) \
do {\
  if (!(args.Length() == 1 &&\
        args[0]->Is ## type0())) {\
    THROW(fn, #arg0);\
  }\
} while(0)

#define CheckArgs2(fn, arg0, type0, arg1, type1) \
do {\
  if (!(args.Length() == 2 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1())) {\
    THROW(fn, #arg0 "," #arg1);\
  }\
} while(0)

#define CheckArgs3(fn, arg0, type0, arg1, type1, arg2, type2) \
do {\
  if (!(args.Length() == 3 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2);\
  }\
} while(0)

#define CheckArgs4(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3) \
do {\
  if (!(args.Length() == 4 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3);\
  }\
} while(0)

#define CheckArgs5(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4) \
do {\
  if (!(args.Length() == 5 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4);\
  }\
} while(0)

#define CheckArgs6(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5) \
do {\
  if (!(args.Length() == 6 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4() &&\
        args[5]->Is ## type5())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5);\
  }\
} while(0)

#define CheckArgs7(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6) \
do {\
  if (!(args.Length() == 7 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4() &&\
        args[5]->Is ## type5() &&\
        args[6]->Is ## type6())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6);\
  }\
} while(0)

#define CheckArgs8(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7) \
do {\
  if (!(args.Length() == 8 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4() &&\
        args[5]->Is ## type5() &&\
        args[6]->Is ## type6() &&\
        args[7]->Is ## type7())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7);\
  }\
} while(0)

#define CheckArgs9(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8) \
do {\
  if (!(args.Length() == 9 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4() &&\
        args[5]->Is ## type5() &&\
        args[6]->Is ## type6() &&\
        args[7]->Is ## type7() &&\
        args[8]->Is ## type8())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7 "," #arg8);\
  }\
} while(0)

#define CheckArgs10(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9) \
do {\
  if (!(args.Length() == 10 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4() &&\
        args[5]->Is ## type5() &&\
        args[6]->Is ## type6() &&\
        args[7]->Is ## type7() &&\
        args[8]->Is ## type8() &&\
        args[9]->Is ## type9())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7 "," #arg8 "," #arg9);\
  }\
} while(0)

#define CheckArgs11(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9, arg10, type10) \
do {\
  if (!(args.Length() == 11 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4() &&\
        args[5]->Is ## type5() &&\
        args[6]->Is ## type6() &&\
        args[7]->Is ## type7() &&\
        args[8]->Is ## type8() &&\
        args[9]->Is ## type9() &&\
        args[10]->Is ## type10())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7 "," #arg8 "," #arg9 "," #arg10);\
  }\
} while(0)

#define CheckArgs12(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9, arg10, type10, arg11, type11) \
do {\
  if (!(args.Length() == 12 &&\
        args[0]->Is ## type0() &&\
        args[1]->Is ## type1() &&\
        args[2]->Is ## type2() &&\
        args[3]->Is ## type3() &&\
        args[4]->Is ## type4() &&\
        args[5]->Is ## type5() &&\
        args[6]->Is ## type6() &&\
        args[7]->Is ## type7() &&\
        args[8]->Is ## type8() &&\
        args[9]->Is ## type9() &&\
        args[10]->Is ## type10() &&\
        args[11]->Is ## type11())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7 "," #arg8 "," #arg9 "," #arg10 "," #arg11);\
  }\
} while(0)

#else

#define CheckArgs0(fn)
#define CheckArgs1(fn, arg0, type0)
#define CheckArgs2(fn, arg0, type0, arg1, type1)
#define CheckArgs3(fn, arg0, type0, arg1, type1, arg2, type2)
#define CheckArgs4(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3)
#define CheckArgs5(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4)
#define CheckArgs6(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5)
#define CheckArgs7(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6)
#define CheckArgs8(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7)
#define CheckArgs9(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8)
#define CheckArgs10(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9)
#define CheckArgs11(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9, arg10, type10)
#define CheckArgs12(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9, arg10, type10, arg11, type11)

#endif

#endif
