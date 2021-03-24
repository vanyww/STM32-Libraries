#pragma once

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#ifndef __MIN
#define __MIN(value1, value2)                 \
   ({                                         \
       __auto_type _value1 = (value1);        \
       __auto_type _value2 = (value2);        \
       _value1 < _value2 ? _value1 : _value2; \
    })
#endif

#ifndef __MAX
#define __MAX(value1, value2)                 \
   ({                                         \
       __auto_type _value1 = (value1);        \
       __auto_type _value2 = (value2);        \
       _value1 > _value2 ? _value1 : _value2; \
    })
#endif
