// MAT201B: Homework 1, Problem 2
// Original authors: Karl Yerkes and Matt Wright (2012)
//
// Author: Karl Yerkes (2015)
//
// Step 02 - make a new Cuttlebone app that's just a renderer: no sound, no
// simulation, just graphics given most recent state. this program should be
// run concurrently with the program in step 01.
//
// note that N and the the definition of State must be the same in both the
// Cuttlebone server/simulator and the client/renderer. the next step will
// formalize this. this app does not draw anything yet.
//

#include "allocore/io/al_App.hpp"
#include "Cuttlebone/Cuttlebone.hpp"

using namespace al;
using namespace std;

// define a Cuttlebone state
//
struct State {
  int frame;
};

// we had to keep this. it must agree with the simulator
//
#define N (40)

// we remove an Gamma/sound stuff
//
struct Particle {
  Vec3f position, velocity;
  float radius, mass, collision, hue;

  Particle() : collision(0), hue(0) {}

  // particle has it's own onDraw and onSound methods
  //
  void onDraw(Graphics& g, Mesh& m) {
    g.pushMatrix();
    g.color(HSV(hue, 1, 1));
    g.translate(position);
    g.scale(radius);
    g.draw(m);
    g.popMatrix();
  }
};

struct AlloApp : App {
  Material material;
  Light light;
  Mesh m;

  Particle particle[N];

  cuttlebone::Taker<State> taker;
  State state;

  AlloApp() {
    addSphere(m);
    m.generateNormals();
    initWindow();
    nav().pos(0, 0, 30);
  }

  virtual void onAnimate(double dt) {
    taker.get(state);
    cout << state.frame << endl;
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();

    // defer drawing to particle itself
    //
    for (int i = 0; i < N; ++i) particle[i].onDraw(g, m);
  }
};

// XXX this is the main function, the entry point of our program. execution
// begins here.
//
int main() {
  AlloApp app;
  app.taker.start();
  app.start();
}
