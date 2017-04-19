#ifndef __CIRCUS_LIST_H__
#define __CIRCUS_LIST_H__

struct _ListNodeBase {
  struct _ListNodeBase *prev;
  struct _ListNodeBase *next;
  _ListNodeBase() 
    : prev(NULL), next(NULL) {}
};

#endif // __CIRCUS_LIST_H__
