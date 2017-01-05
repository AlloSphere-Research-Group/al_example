// MAT201B: Homework 4, Problem 1
// Original authors: Karl Yerkes and Matt Wright (2012)
//
// Author: Karl Yerkes (2015)
//

#ifndef __COMMON_STUFF__
#define __COMMON_STUFF__

// define everything that the renderer needs to draw on each frame. the
// simulator must generate this infomation on each frame.
//
#define N (40)
struct State {
  int frame;
  Pose pose;
  Vec3f position[N];
  float radius[N];
  float hue[N];
};

#endif
