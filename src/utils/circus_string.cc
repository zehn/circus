#include "circus_string.h"

class Circus_String::StringPrivateData {
public:
   
};

Circus_String::Circus_String() 
    : : dptr(new StringPrivateData()) {
}

Circus_String::Circus_String(const char *latin) 
    : dptr(new StringPrivateData()) {
    
}

Circus_String::Circus_String(const Circus_String &origin) {
    dptr = &origin;
}

Circus_String::~Circus_String() {
    delete dptr;
}

