#ifndef ARG_CHECKS_H_
#define ARG_CHECKS_H_

#include "nan.h"

#if defined(ARG_CHECKS)

#define BASE_MESSAGE "Invalid arguments: Expected "
#define THROW(fn, info) NanThrowTypeError(BASE_MESSAGE #fn "(" info ")")

#define CheckArgs0(fn) \
do {\
  if (!(info.Length() == 0)) {\
    THROW(fn, );\
  }\
} while(0)

#define CheckArgs1(fn, arg0, type0) \
do {\
  if (!(info.Length() == 1 &&\
        info[0]->Is ## type0())) {\
    THROW(fn, #arg0);\
  }\
} while(0)

#define CheckArgs2(fn, arg0, type0, arg1, type1) \
do {\
  if (!(info.Length() == 2 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1())) {\
    THROW(fn, #arg0 "," #arg1);\
  }\
} while(0)

#define CheckArgs3(fn, arg0, type0, arg1, type1, arg2, type2) \
do {\
  if (!(info.Length() == 3 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2);\
  }\
} while(0)

#define CheckArgs4(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3) \
do {\
  if (!(info.Length() == 4 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3);\
  }\
} while(0)

#define CheckArgs5(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4) \
do {\
  if (!(info.Length() == 5 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4);\
  }\
} while(0)

#define CheckArgs6(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5) \
do {\
  if (!(info.Length() == 6 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4() &&\
        info[5]->Is ## type5())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5);\
  }\
} while(0)

#define CheckArgs7(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6) \
do {\
  if (!(info.Length() == 7 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4() &&\
        info[5]->Is ## type5() &&\
        info[6]->Is ## type6())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6);\
  }\
} while(0)

#define CheckArgs8(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7) \
do {\
  if (!(info.Length() == 8 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4() &&\
        info[5]->Is ## type5() &&\
        info[6]->Is ## type6() &&\
        info[7]->Is ## type7())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7);\
  }\
} while(0)

#define CheckArgs9(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8) \
do {\
  if (!(info.Length() == 9 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4() &&\
        info[5]->Is ## type5() &&\
        info[6]->Is ## type6() &&\
        info[7]->Is ## type7() &&\
        info[8]->Is ## type8())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7 "," #arg8);\
  }\
} while(0)

#define CheckArgs10(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9) \
do {\
  if (!(info.Length() == 10 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4() &&\
        info[5]->Is ## type5() &&\
        info[6]->Is ## type6() &&\
        info[7]->Is ## type7() &&\
        info[8]->Is ## type8() &&\
        info[9]->Is ## type9())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7 "," #arg8 "," #arg9);\
  }\
} while(0)

#define CheckArgs11(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9, arg10, type10) \
do {\
  if (!(info.Length() == 11 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4() &&\
        info[5]->Is ## type5() &&\
        info[6]->Is ## type6() &&\
        info[7]->Is ## type7() &&\
        info[8]->Is ## type8() &&\
        info[9]->Is ## type9() &&\
        info[10]->Is ## type10())) {\
    THROW(fn, #arg0 "," #arg1 "," #arg2 "," #arg3 "," #arg4 "," #arg5 "," #arg6 "," #arg7 "," #arg8 "," #arg9 "," #arg10);\
  }\
} while(0)

#define CheckArgs12(fn, arg0, type0, arg1, type1, arg2, type2, arg3, type3, arg4, type4, arg5, type5, arg6, type6, arg7, type7, arg8, type8, arg9, type9, arg10, type10, arg11, type11) \
do {\
  if (!(info.Length() == 12 &&\
        info[0]->Is ## type0() &&\
        info[1]->Is ## type1() &&\
        info[2]->Is ## type2() &&\
        info[3]->Is ## type3() &&\
        info[4]->Is ## type4() &&\
        info[5]->Is ## type5() &&\
        info[6]->Is ## type6() &&\
        info[7]->Is ## type7() &&\
        info[8]->Is ## type8() &&\
        info[9]->Is ## type9() &&\
        info[10]->Is ## type10() &&\
        info[11]->Is ## type11())) {\
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
