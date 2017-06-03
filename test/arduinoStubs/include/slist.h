#ifndef SLIST_H
#define SLIST_H

template<typename T>
class SList {
public:
  SList() { }

protected:

  static void _add(T* self) { }

  static void _remove(T* self) { }

  static T* _s_first;
  T* _next;
};


#endif //SLIST_H
