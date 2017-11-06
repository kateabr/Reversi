#include "current.h"

Current enemy(Current cur) {
  if (cur == Current::User)
    return Current::Computer;
  return Current::User;
}
