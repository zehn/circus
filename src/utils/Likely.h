#ifndef __CIRCUS_LIKELY_H__
#define __CIRCUS_LIKELY_H__

#if defined __GNUC__
#define LIKELY(v)    __builtin_expect ((v), 1)
#define UNLIKELY(v)  __builtin_expect ((v), 0)
#else
#define LIKELY(v)
#define UNLIKELY(v)
#endif

#endif // __CIRCUS_LIKELY_H__

