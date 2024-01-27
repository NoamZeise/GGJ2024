#ifndef VEC_MATH_H
#define VEC_MATH_H

#include <glm/glm.hpp>

namespace gh {
    
    inline bool colliding(glm::vec2 l1, glm::vec2 l2) {
	return l1.x < l2.x + l2.y &&
	    l1.x + l1.y > l2.x;
    }
    
  inline bool colliding(glm::vec4 a, glm::vec4 b)
  {
    return  a.x < b.x + b.z &&
      a.x + a.z > b.x &&
      a.y < b.y + b.w &&
      a.y + a.w > b.y;
  }

  static glm::vec2 colliding(glm::vec2 u1, glm::vec2 u2, glm::vec2 v1, glm::vec2 v2) {
      float px = ((v2.x-v1.x)*(u1.y-v1.y) - (v2.y-v1.y)*(u1.x-v1.x)) / ((v2.y-v1.y)*(u2.x-u1.x) - (v2.x-v1.x)*(u2.y-u1.y));

      float py = ((u2.x-u1.x)*(u1.y-v1.y) - (u2.y-u1.y)*(u1.x-v1.x)) / ((v2.y-v1.y)*(u2.x-u1.x) - (v2.x-v1.x)*(u2.y-u1.y));

      if(px >= 0 && px <= 1 && py >= 0 && py <= 1) {
	  return glm::vec2(u1.x + (px * (u2.x-u1.x)),
			   u1.y + (px * (u2.y - u1.y)));
      }
      return glm::vec2(0);
  }
  

inline bool aInB(glm::vec4 a, glm::vec4 b)
{
return a.x > b.x && a.x < b.x + b.z &&
  a.y > b.y && a.y < b.y + b.z;
}

inline bool contains(glm::vec2 p, glm::vec4 r)
{
return r.x < p.x && p.x < r.x + r.z  && r.y < p.y && p.y < r.y + r.w;
}

 inline void addVec2ToRect(glm::vec2 v, glm::vec4 *r)
 {
     *r = glm::vec4(r->x + v.x, r->y + v.y, r->z, r->w);
 }

 inline glm::vec4 addVec2ToRect(glm::vec2 v, glm::vec4 r)
 {
     return glm::vec4(r.x + v.x, r.y + v.y, r.z, r.w);
 }


    inline glm::vec2 centre(glm::vec4 rect)
    {

	return glm::vec2(rect.x + rect.z/2, rect.y + rect.w/2);
    }

inline float max(float a, float b)
{
return a > b ? a : b;
}

inline float min(float a, float b)
{
return a < b ? a : b;
}
}

#endif /* VEC_MATH_H */
