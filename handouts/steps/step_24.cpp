// Synchronizing graphics and audio:   only move a particle when it plays a new
// note
// Includes bringing N down to 2 so it's easier to see and hear what's happening

#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"

using namespace al;
#define N 2

#define MAX_SONG_LENGTH 10

struct Agent {
  int whichAgentAmI;
  Color color;
  Vec3f position;
  bool justChangedNote;

  // To play my song
  gam::Sine<> sine;
  gam::Env<5> ampEnv;  // Amplitude envelope for notes
  gam::Accum<> tmr;    // an accumulator that will keep track of note time

  // My song
  int song_length;
  int current_note;
  float frequencies[MAX_SONG_LENGTH];  // Hertz
  float durations[MAX_SONG_LENGTH];    // seconds

  Agent() {
    using namespace rnd;

    justChangedNote = false;

    // Make up our amplitude envelope

    ampEnv.curve(uniformS(4.5));  // random curvature

// We want a short "ending" attack, so envelope will have 5 segments
//  0. Start at zero
//  1. Initial attack is quick strong
//  2. Gradually decay as long as necessary for overall duration (sustain
//  segment)
#define SUSTAIN_SEGMENT 1  // The segment from level 1 to level 2
    //  3. Quickly go up to a strong value just before ending
    //  4. Quickly end at zero
    float strong = 0.7 + uniformS(.25);
    float medium = 0.3 + uniformS(.25);
    float quick = 0.01 + uniform(.25);

    ampEnv.levels(0, strong + uniformS(.05), medium, strong + uniformS(.05), 0);
    ampEnv.lengths(quick * uniform(.9, 1.1), 0 /*sustain*/,
                   quick * uniform(.9, 1.1), quick * uniform(.9, 1.1));

    std::cout << "Initial envelope for " << whichAgentAmI << ":" << std::endl;
    printEnvelope();

    // Make up our song

    song_length = 1 + uniform<int>(MAX_SONG_LENGTH);
    std::cout << "Calling Agent constructor; song_length is " << song_length
              << std::endl;

    // Random starting note between 150 and 300 Hz
    frequencies[0] = uniform(150, 300);
    durations[0] = 0.5;

    float multiplier = uniform(1.01, 1.5);
    float offset = uniformS(25);

    for (int i = 1; i < song_length; i++) {
      frequencies[i] = frequencies[i - 1] * multiplier + offset;
      durations[i] = pow(2.0, uniform<int>(5) - 4);
    }

    // Initialize song player
    current_note = 0;
    sine.freq(frequencies[current_note]);
    tmr.period(durations[current_note]);
  }

  void printEnvelope() {
    for (int i = 0; i < ampEnv.size() - 1; i++) {
      std::cout << "level[" << i << "]=" << ampEnv.levels()[i] << ",length["
                << i << "]=" << ampEnv.lengths()[i] << ",";
    }
    int lasti = ampEnv.size() - 1;
    std::cout << "level[" << lasti << "]=" << ampEnv.lengths()[lasti]
              << std::endl;
  }

  void onAnimate(double dt) {
    if (justChangedNote) {
      position +=
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.1;
      justChangedNote = false;
    }
  }

  void onDraw(Graphics& g, const Viewpoint& v, Mesh& m) {
    g.pushMatrix();
    g.color(color);
    g.translate(position);
    g.draw(m);
    g.popMatrix();
  }

  float onSound() {
    if (tmr()) {
      // Current note just ended so move to next one
      current_note++;
      if (current_note >= song_length) current_note = 0;
      tmr.reset();
      tmr.period(durations[current_note]);

      sine.freq(frequencies[current_note]);

      ampEnv.totalLength(durations[current_note], SUSTAIN_SEGMENT);
      if (ampEnv.lengths()[SUSTAIN_SEGMENT] < 0.) {
        // We want a note shorter than the "quick" three segments allow, so
        // permanently
        // make them even quicker
        float* l = ampEnv.lengths();
        float oldMinDuration = l[0] + l[2] + l[3];
        float s /* slimming factor */ =
            durations[current_note] / oldMinDuration;
        s *= 0.99;  // A little bit smaller just to be safe
        ampEnv.lengths(l[0] * s, 0 /*sustain*/, l[2] * s, l[3] * s);

        std::cout << "Slimmed by " << s << " to get a shorter envelope for "
                  << whichAgentAmI << ":" << std::endl;
        printEnvelope();
      }
      ampEnv.reset();
      justChangedNote = true;
    }

    return sine() * ampEnv();
  }
};

struct AlloApp : App {
  Material material;
  Light light;
  Mesh m;
  Agent agents[N];

  AlloApp() {
    std::cout << "Created AlloApp" << std::endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.primitive(Graphics::TRIANGLES);
    m.scale(0.2);
    m.generateNormals();
    for (int i = 0; i < N; ++i) {
      agents[i].whichAgentAmI = i;
      agents[i].position = Vec3f(sin(i), i, -i * i / 7);
      agents[i].color = HSV((float)i / N, 1, 1);
    }
    initWindow();
    initAudio();
  }
  virtual void onAnimate(double dt) {
    for (int i = 0; i < N; ++i) {
      agents[i].onAnimate(dt);
    }
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();
    for (int i = 0; i < N; ++i) {
      agents[i].onDraw(g, v, m);
    }
  }

  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());
    while (io()) {
      float s = 0;
      for (int i = 0; i < N; ++i) {
        float distance = (nav().pos() - agents[i].position).mag();
        // constrain distance so no matter how we navigate each agent is not too
        // loud...
        if (distance < 0.5) distance = 0.5;
        // ... or too quiet
        if (distance > 7) distance = 7;
        s += agents[i].onSound() / distance;
      }
      io.out(0) = io.out(1) = s * 0.08;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      std::cout << "Spacebar pressed" << std::endl;
    }
  }
};

int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}
