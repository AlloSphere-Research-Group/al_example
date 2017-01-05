// MAT201B: Homework 1, Problem 2
// Original authors: Karl Yerkes and Matt Wright (2012)
//
// Author: Karl Yerkes (2015)
//
// Step 08 - update our renderer to recieve positions and draw
//

#include "allocore/io/al_App.hpp"
#include "Cuttlebone/Cuttlebone.hpp"

using namespace al;
using namespace std;

#include "06_common_with_positions.hpp"

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
    // XXX they also have to have a radius to be drawn!
    for (int i = 0; i < N; i++) particle[i].radius = 0.5;
    nav().pos(0, 0, 30);
  }

  virtual void onAnimate(double dt) {
    taker.get(state);
    for (int i = 0; i < N; i++) particle[i].position = state.position[i];
    // cout << state.frame << endl;
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
