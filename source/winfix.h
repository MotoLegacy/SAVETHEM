#ifndef _WINFIX_H_
#define _WINFIX_H_

#ifdef __WIN32__
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 10000
#endif // __WIN32__

#endif // _WINFIX_H_