#include "circus_string.h"

class Circus_String::StringPrivateData {
public:
   
};

Circus_String::Circus_String() 
    : dptr(new StringPrivateData()) {
}

Circus_String::Circus_String(const char *latin) 
    : dptr(new StringPrivateData()) {
    
}

Circus_String::Circus_String(const Circus_String &origin) {
    : dptr(new StringPrivateData())
    if (this != &origin) {
        *(this->dptr) = *(origin.dptr);
    }
}

Circus_String::~Circus_String() {
    delete dptr;
}

