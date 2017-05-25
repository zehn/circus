#ifndef _CIRCUS_STRING_H_
#define _CIRCUS_STRING_H_

class Circus_String {
public:
    Circus_String();
    Circus_String(const char *latin);
    Circus_String(const Circus_String &origin);
    ~Circus_String();
  
    Circus_String& operator = (const Circus_String &origin);
    
private:
    class StringPrivateData;
    StringPrivateData *dptr;
};

#endif // _CIRCUS_STRING_H_

