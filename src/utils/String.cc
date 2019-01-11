#include "circus_string.h"

namespace Circus {

class String::StringPrivateData {
public:
   
};

String::String() 
    : dptr(new StringPrivateData()) {
}

String::String(const char *latin) 
    : dptr(new StringPrivateData()) {
    
}

String::String(const String &origin)
    : dptr(new StringPrivateData()) {
    if (this != &origin) {
        *(this->dptr) = *(origin.dptr);
    }
}

String::~String() {
    delete dptr;
}

} // namespace Circus
