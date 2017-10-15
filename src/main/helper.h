#pragma once

template <class T>
void DeleteVect(std::vector<T *> &theArray) {
  std::vector<T *>().swap(theArray);
}

template <class T>
void DeleteList(std::list<T *> &theList) {
  std::list<T *>().swap(theList);
}

template <class T1, class T2>
void DeleteMap(std::map<T1, T2 *> &container) {
  for (typename std::map<T1, T2 *>::iterator iter = container.begin();
       iter != container.end(); ++iter)
    delete (*iter).second;
  container.clear();
}

template <class T>
inline void PushTypeOnVect(std::vector<uint8_t> &theVector, T unit) {
  theVector.insert(theVector.end(), ((uint8_t *)&unit),
                   ((uint8_t *)(&unit)) + sizeof(T));
}

template <class T>
inline void PushTypeOnVectBE(std::vector<uint8_t> &theVector, T unit) {
  for (uint32_t i = 0; i < sizeof(T); i++)
    theVector.push_back(*(((uint8_t *)(&unit)) - i + sizeof(T) - 1));
}

inline void PushBackStringOnVector(std::vector<uint8_t> &theVector,
                                   std::string &str) {
  theVector.insert(theVector.end(), str.data(), str.data() + str.length());
}

template <class Tstring>
inline Tstring FormatString(const Tstring fmt, ...) {
  // TODO: either fix up this function or rewrite usages of it (probably
  // preferable)
  return fmt;
}
