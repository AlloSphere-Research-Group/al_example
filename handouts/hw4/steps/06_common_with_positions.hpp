// MAT201B: Homework 1, Problem 2
// Original authors: Karl Yerkes and Matt Wright (2012)
//
// Author: Karl Yerkes (2015)
//
// Step 06 - put positions into State definition
//
//

#ifndef __COMMON_STUFF__
#define __COMMON_STUFF__

// define stuff that's common to both the simulator and renderer: definition of
// State and N.
//
#define N (40)
struct State {
  int frame;
  Vec3f position[N];
};

#endif
