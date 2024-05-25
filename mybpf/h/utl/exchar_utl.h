

typedef UINT EXCHAR;

#define EXCHAR_IS_NORMAL_CHAR(exchar) ((exchar) < 256)
#define EXCHAR_IS_EXTEND_CHAR(exchar) ((exchar) > 256)

#define EXCHAR_GET_NORMAL_CHAR(exchar) ((UCHAR)((exchar) & 0xff))


#define EXCHAR_NORMAL_DELETE 127

#define EXCHAR_EXTEND_UNKNOWN 256
#define EXCHAR_EXTEND_TEMP    257

#define EXCHAR_EXTEND_UP    258
#define EXCHAR_EXTEND_DOWN  259
#define EXCHAR_EXTEND_LEFT  260
#define EXCHAR_EXTEND_RIGHT 261
#define EXCHAR_EXTEND_HOME  262   
#define EXCHAR_EXTEND_END   263   

HANDLE EXCHAR_Create();
void EXCHAR_Reset(IN HANDLE hExcharHandle);
VOID EXCHAR_Delete(IN HANDLE hExcharHandle);
EXCHAR EXCHAR_Parse(IN HANDLE hExcharHandle, IN UCHAR ucChar);


