#ifndef _CLOCKFIX_H_
#define _CLOCKFIX_H_

// Non-POSIX platforms either can't agree on what
// CLOCKS_PER_SEC should be or in the case of Windows
// have broken it years ago and stated they won't fix..
// sigh.. I'll just manually set it to POSIX.
#ifdef __WIN32__
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 10000
#endif
#ifdef PLATFORM_WEB
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 10000000
#endif

#endif // _CLOCKFIX_H_