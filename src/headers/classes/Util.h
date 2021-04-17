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

    float hypo(float x1, float y1, float x2, float y2) {
      float a = y1 - y2;
      float b = x1 - x2;

      return sqrt(pow(a, 2) + pow(b, 2));
    }

} util;

#endif