#if !defined(AFX_BASE64_CONVERT_H)
#define AFX_BASE64_CONVERT_H

char * base64_encode( const unsigned char * originChar, char * base64, int originlength );
int base64_decode( const char * base64, unsigned char * originChar );

#endif // !defined(AFX_BASE64_CONVERT_H)
