#ifndef _CIRCUS_STRING_H_
#define _CIRCUS_STRING_H_

namespace Circus {

class String {
public:
    String();
    String(const char *latin);
    String(const String &origin);
    ~String();
  
    String& operator = (const String &origin);
    
private:
    class StringPrivateData;
    StringPrivateData *dptr;
};

} // namespace Circus

#endif // _CIRCUS_STRING_H_

