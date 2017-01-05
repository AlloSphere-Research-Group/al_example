// MAT201B
// Fall 2015
// Author(s): Karl Yerkes
//
// Shows how to:
// - use faceToward, move, and step methods of the Nav class
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;

#define N 3

struct AlloApp : App {
  Material material;
  Light light;
  Mesh body;
  Nav agent[N];

  AlloApp() {
    nav().pos(0, 0, 20);
    light.pos(0, 0, 20);

    addSphere(body);
    body.scale(1, 1, 3);
    body.translate(0.5, 0, 2);
    addSphere(body);
    body.translate(-1, 0, 0);
    addSphere(body);
    body.scale(0.3);

    body.generateNormals();

    for (int i = 0; i < N; ++i) {
      agent[i].pos(
          Vec3f(2 * sin(M_PI * 2 * i / N), 2 * cos(M_PI * 2 * i / N), 0));
      agent[i].move(0, 0, 2);
    }

    initWindow();
  }

  virtual void onAnimate(double dt) {
    agent[0].faceToward(agent[N - 1], 0.1);
    for (int i = 1; i < N; ++i) {
      agent[i].step(0.01);
      agent[i].faceToward(agent[i - 1], 0.1);
    }
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();
    for (int i = 0; i < N; ++i) {
      g.pushMatrix();
      g.color(HSV((float)i / N, 1, 1));
      g.translate(agent[i].pos());
      g.rotate(agent[i].quat());
      g.draw(body);
      g.popMatrix();
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main() { AlloApp().start(); }
