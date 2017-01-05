// MAT201B: Homework 1, Problem 2
// Original authors: Karl Yerkes and Matt Wright (2012)
//
// Author: Karl Yerkes (2015)
//
// What keys do...
// - 'r' toggles "reverse time", making the time step negative
// - 'p' toggles "pause state", freezing the simulation
// - 'l' toggles "bullet time", slowing the simulation by a factor of 10
// - ' ' (space bar) resets each particle with random position, velocity, size
// and hue.
//
// Step 01 - add Cuttlebone "Maker" to the current app. we add a #include, a
// definition of "state", a cuttlebone::Maker, a call to maker.set(), and we
// call app.maker.start() in main.
//

#include "Cuttlebone/Cuttlebone.hpp"
#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"

using namespace al;
using namespace std;

// define a Cuttlebone state
//
struct State {
  int frame;
};

// XXX when defining numeric consants with the C-preprocessor (like those
// below), it's best to surround. the number with parenthesis, just in case
// they are used in math expressions:
//   http://goo.gl/Fl9kTj
//
#define N (40)
#define G (7.0)
#define TIME_STEP (0.01)
#define MAX_FORCE (5.0)
#define DRAG (0.003)

// XXX this is a class i made to help me smoothly change the amplitude of a
// signal over the course of many samples. set the "target" amplitude with the
// target() method.
//
class SmoothAmplitude {
  float _target, _actual, _amount;

 public:
  SmoothAmplitude(float a = 0.1) : _target(0), _actual(0), _amount(a) {}
  void target(float t) { _target = t; }
  float operator()(float signal) {
    // XXX change the actual amplitude a litte bit toward the target amplitude
    //
    _actual += (_target - _actual) * _amount;
    return _actual * signal;
  }
};

// XXX encapsulate particles properties in a class named Particle. the
// collision property is used differently than many apps. it's represents the
// *amount* that the particle is overlapping with other particles (see how it's
// used in onAnimate()).
//
struct Particle {
  Vec3f position, velocity;
  float radius, mass, collision, hue;
  gam::Sine<> sine;
  SmoothAmplitude smoother;

  Particle() : collision(0), hue(0) {}

  // XXX setting the radius also decides the mass and the frequency
  //
  void setRadius(double r) {
    radius = r;
    mass = 4.18879020479 * r * r * r;
    sine.freq(1000 - 1000 * r);
  }

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
  float onSound() { return smoother(sine()); }
};

struct AlloApp : App {
  Material material;
  Light light;
  Mesh m;

  Particle particle[N];

  bool slow, pause, reverse;

  cuttlebone::Maker<State> maker;
  State state;

  AlloApp() : slow(false), pause(false), reverse(false) {
    state.frame = 0;

    addSphere(m);
    m.generateNormals();

    // XXX put initial settings into a function
    //
    reset();

    initWindow();
    initAudio();
  }

  void reset() {
    for (int i = 0; i < N; ++i) {
      particle[i].position =
          Vec3f(rnd::uniformS(5.0), rnd::uniformS(5.0), rnd::uniformS(5.0));
      particle[i].velocity =
          Vec3f(rnd::uniformS(10.0), rnd::uniformS(10.0), rnd::uniformS(10.0));
      particle[i].setRadius(rnd::uniform(.1, 1.0));
      particle[i].hue = rnd::uniform();
    }

    nav().pos(0, 0, 30);
  }

  virtual void onAnimate(double dt) {
    if (pause == 1) return;

    // zero all forces. start fresh.
    //
    Vec3f force[N];
    for (int i = 0; i < N; ++i) force[i].zero();

    // zero collision amount
    //
    for (int i = 0; i < N; ++i) particle[i].collision = 0;

    // for each pair of particles, calculate the gravity beteen the two.
    //
    for (int a = 0; a < N; ++a)
      for (int b = a + 1; b < N; ++b) {
        // this is here to make the formulas below easier to read
        //
        Vec3f A = particle[a].position;
        Vec3f B = particle[b].position;
        float m_A = particle[a].mass;
        float m_B = particle[b].mass;

        // Newton's law of gravitation
        //
        Vec3f points_from_a_to_b = (B - A).normalize();
        float distance_squared = (B - A).magSqr();
        Vec3f force_of_b_on_a =
            points_from_a_to_b * G * m_A * m_B / distance_squared;

        // set maximum force
        //
        if (force_of_b_on_a.mag() > MAX_FORCE)
          force_of_b_on_a.normalize(MAX_FORCE);

        // record the force between A and B, applying the force both ways: A->B
        // and B->A. note the use of +=. these forces *accumulate*.
        //
        force[a] += force_of_b_on_a;
        force[b] += -force_of_b_on_a;

        // detect collision state
        //
        float collision =
            (particle[a].radius + particle[b].radius) - (B - A).mag();

        if (collision > 0) {
          particle[a].collision += collision;
          particle[b].collision += collision;

          // mix colors
          //
          particle[a].hue += (particle[b].hue - particle[a].hue) * 0.001;
          particle[b].hue += (particle[a].hue - particle[b].hue) * 0.001;
        }
      }

    // set the target amplitude of each particle based on the collision amount
    // of each particle. this should have the effect that particles emit sound
    // only while they overlap.
    //
    for (int a = 0; a < N; ++a)
      particle[a].smoother.target(particle[a].collision);

    // global gravity pulls all particles toward the origin (0, 0, 0). this
    // should keep particles somewhere around the origin. if we didn't have
    // this, they might float away.
    //
    for (int a = 0; a < N; ++a)
      force[a] += particle[a].position / particle[a].position.mag() * -G;

    // apply a simple model of "viscous drag" to improve the stability of the
    // system.
    //
    for (int a = 0; a < N; ++a) particle[a].velocity *= (1.0 - DRAG);

    // use (forward) Euler method to integrate velocity to get position and
    // acceleration to get velocity.
    //
    for (int i = 0; i < N; ++i) {
      // slow down simulation by reducing the time step by a factor of 10
      //
      float timeStep = (slow == 1) ? 0.1 * TIME_STEP : TIME_STEP;

      // reverse time step, making everything go backwards
      //
      if (reverse) timeStep *= -1;

      // Forward Euler
      //
      particle[i].position += particle[i].velocity * timeStep;
      particle[i].velocity += force[i] / particle[i].mass * timeStep;
    }

    // move the light around
    //
    light.pos(particle[0].position);

    // broadcast Cuttlebone state and increment frame count
    //
    maker.set(state);
    state.frame++;
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();

    // defer drawing to particle itself
    //
    for (int i = 0; i < N; ++i) particle[i].onDraw(g, m);
  }

  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());
    while (io()) {
      float s = 0;
      for (int i = 0; i < N; ++i) {
        float distance = (nav().pos() - particle[i].position).mag();
        if (distance < 0.5) distance = 0.5;
        if (distance > 7) distance = 7;

        // defer sounding to particle itself
        //
        s += particle[i].onSound() / distance;
      }
      io.out(0) = s / N;
      io.out(1) = s / N;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') reset();
    if (k.key() == 'p') pause = !pause;
    if (k.key() == 'l') slow = !slow;
    if (k.key() == 'r') reverse = !reverse;
  }
};

// XXX this is the main function, the entry point of our program. execution
// begins here.
//
int main() {
  AlloApp app;
  app.maker.start();
  app.start();
}
