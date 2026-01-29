//---------------------------------------------------------------------------

#ifndef MainUnit1_SocketsH
#define MainUnit1_SocketsH
//---------------------------------------------------------------------------
#endif

extern int NoActivityTime[];

// 4 Digi Servers of 16 channels each
extern char RcvStr[][];
extern int RcvIndex[];

template <typename TSock>
extern void LogRxTextLogged(TSock* sock, const char* text);
