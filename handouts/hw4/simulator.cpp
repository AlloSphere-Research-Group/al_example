// MAT201B: Homework 4, Problem 1
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
// lines marked with XXX are vital to using Cuttlebone
//

#include "Cuttlebone/Cuttlebone.hpp"  // XXX
#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"
#include "alloutil/al_Simulator.hpp"  // XXX

using namespace al;
using namespace std;

#include "common.hpp"  // XXX

void printFactsAboutState(int size) {
  cout << "==================================================" << endl;
  cout << "Your state type takes " << size << " bytes in memory." << endl;

  if (size > 1024.0f * 1024.0f * 1024.0f) {
    cout << "That is " << size / (1024.0f * 1024.0f * 1024.0f) << " gigabytes."
         << endl;
    cout << "That's too big." << endl;
  } else if (size > 1024.0f * 1024.0f * 10) {
    cout << "That is " << size / (1024.0f * 1024.0f) << " megabytes." << endl;
    cout << "That's a very big state." << endl;
  } else if (size > 1024.0f * 1024.0f) {
    cout << "That is " << size / (1024.0f * 1024.0f) << " megabytes." << endl;
  } else if (size > 1024.0f) {
    cout << "That is " << size / 1024.0f << " kilobytes." << endl;
  } else {
  }

  // Standard 1Gb Ethernet effective bandwidth
  //
  float gbe = 1.18e+8f;

  cout << "On a 1Gb Ethernet LAN (like the MAT network), ";
  if (gbe / size > 60.0f)
    cout << "you will use \n"
         << 100.0f * (size * 60.0f) / gbe
         << "\% of the effective bandwidth with a framerate of 60 Hz." << endl;
  else
    cout << "your framerate will be *network limited* to " << gbe / size
         << " Hz." << endl;

  cout << "On a 10Gb Ethernet LAN (like the AlloSphere network), ";
  if (10 * gbe / size > 60.0f)
    cout << "you will use \n"
         << 100.0f * (size * 60.0f) / (10 * gbe)
         << "\% of the effective bandwidth with a framerate of 60 Hz." << endl;
  else {
    cout << "your framerate will be *network limited* to " << 10 * gbe / size
         << " Hz." << endl;
    cout << "Your state is very large." << endl;
  }
  cout << "==================================================" << endl;
}

// simulation constants
//
#define G (7.0)
#define TIME_STEP (0.01)
#define MAX_FORCE (5.0)
#define DRAG (0.003)

// smoothing class
//
class SmoothAmplitude {
  float _target, _actual, _amount;

 public:
  SmoothAmplitude(float a = 0.1) : _target(0), _actual(0), _amount(a) {}
  void target(float t) { _target = t; }
  float operator()(float signal) {
    // change the actual amplitude a litte bit toward the target amplitude
    //
    _actual += (_target - _actual) * _amount;
    return _actual * signal;
  }
};

// encapsulate particles properties in a class named Particle
//
struct Particle {
  Vec3f position, velocity;
  float radius, mass, collision, hue;
  gam::Sine<> sine;
  SmoothAmplitude smoother;

  Particle() : collision(0), hue(0) {}

  void setRadius(double r) {
    radius = r;
    mass = 4.18879020479 * r * r * r;
    sine.freq(1000 - 1000 * r);
  }

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

struct AlloApp : App, InterfaceServerClient {  // XXX
  Material material;
  Light light;
  Mesh m;

  Particle particle[N];

  bool slow, pause, reverse;

  cuttlebone::Maker<State> maker;  // XXX
  State state;                     // XXX

  AlloApp()
      : maker(Simulator::defaultBroadcastIP()),                        // XXX
        InterfaceServerClient(Simulator::defaultInterfaceServerIP()),  // XXX
        slow(false),
        pause(false),
        reverse(false) {
    // print facts about the state
    //
    printFactsAboutState(sizeof(State));

    state.frame = 0;  // XXX

    addSphere(m);
    m.generateNormals();

    // put initial (random) settings into a function
    //
    reset();

    initWindow();
    initAudio();

    // set interface server nav/lens to App's nav/lens
    //
    InterfaceServerClient::setNav(nav());    // XXX
    InterfaceServerClient::setLens(lens());  // XXX
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
    // handle messages from interface server
    //
    while (InterfaceServerClient::oscRecv().recv())
      ;  // XXX

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
    for (int i = 0; i < N; i++) {
      state.position[i] = particle[i].position;  // XXX
      state.radius[i] = particle[i].radius;      // XXX
      state.hue[i] = particle[i].hue;            // XXX
    }
    state.pose = nav();
    maker.set(state);  // XXX
    state.frame++;     // XXX
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

// this is the main function, the entry point of our program. execution
// begins here.
//
int main() {
  AlloApp app;
  app.maker.start();  // XXX
  app.start();
}
