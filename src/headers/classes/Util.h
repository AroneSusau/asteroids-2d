#ifndef H_UTIL
#define H_UTIL

#include <math.h>

static class util_t {
  public:
    
    float deg_to_rad(float deg) {
      return deg * M_PI / 180.0f;
    }

    float random(int min, int max) {
      
      if (max == 0)
        return min;
      
      return (rand() % max) + min;
    }

} util;

#endif