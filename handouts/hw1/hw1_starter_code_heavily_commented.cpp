// this include provides access to the class named App, which our class (a.k.a
// struct) will inherit from.
//
#include "allocore/io/al_App.hpp"

// this include provides access to the Sine<> class in Gamma as well as a bunch
// of other cool oscillators.
//
#include "Gamma/Oscillator.h"

// this tells the compiler to search in the 'al' namespace if it cannot resolve
// a particular symbol, then also look in the 'std' namespace. this means we
// can say App and Material instead of saying al::App and al::Material and cout
// instead of std::cout.
//
using namespace al;
using namespace std;

// this tells the c preprocessor to search for the symbol N in our code and
// replace every instance of N with 20. this saves 1 int of memory in our
// program and it means we can use N all over our program but we only have to
// change its value in this one place near the top of the file. simulation
// parameters are commonly handled this way.
//
#define N 20

// this next line reads: "there shall be a class (a.k.a struct) called AlloApp
// that inherits from App". this means that AlloApp is an App plus some other
// stuff that we the programmers shall say next. we then proceed to extend
// AlloApp beyond its parent, App.
//
struct AlloApp : App {
  // we need these 2 objects to make our spheres shaded with color. these hide
  // a lot of complicated stuff behind a pretty curtain. we'll get behind the
  // curtain soon enough. for now, leave these here and call material(), then
  // light() at the top of onDraw().
  //
  Material material;
  Light light;

  // a Mesh is a container for verticies, normals, colors, texture coordinates,
  // and some other stuff. in this code, it'll just hold verticies and normals.
  //
  Mesh m;

  // makes an array of N sine oscillators. thank you, Gamma.
  //
  gam::Sine<> sine[N];

  // Vec3f contains 3 floating point numbers that may be accessed in many ways
  // that are convienient for simulations and graphics. many operators and
  // methods are defined for Vec: +, -, +=, -=, ==, !=, [], elems(), mag(),
  // normalize(), norm(), clip(), wrap(), lerp(). for this code, position[i] is
  // a Vec3f that represents the 3d cartsian coordinate of the i-th sphere. for
  // example, the y coordinate of the 7th sphere is spelled "position[7].y".
  // however, c++ arrays start at 0, so when we speak of the 7th element of an
  // array, we count like up like this (use your fingers): 0th, 1st, 2nd, 3rd,
  // 4th, 5th, 6th, 7th. so, the 7th element in c++ is the 8th element on human
  // fingers.
  //
  Vec3f position[N];

  // this is the constructor of our AlloApp class. after our AlloApp is placed
  // into memory, this constructor is called to handle any extra setup that we
  // want to happen once.
  //
  AlloApp() {
    // this prints to the console, so we know that something happened. often,
    // when a program does not work the way we intended, we use the console to
    // print useful information about what the program is doing.
    //
    cout << "Created AlloApp" << endl;

    // this can be read "move the viewer position back 10 units."
    //
    nav().pos(0, 0, 10);

    // this can be read "move the light position back 10 units."
    //
    light.pos(0, 0, 10);

    // this uses math to generate verticies in the shape of a sphere, then it
    // adds those verticies to our mesh. find addSphere in the doxygen if you
    // want more detailed spheres.
    //
    addSphere(m);

    // set our mesh to render using the OpenGL primitive called "triangles".
    // this tells the GPU to fill in triangles with color or shading or both.
    // however, recently AlloSystem changed a little to make TRIANGLES the
    // primitive for meshes that are made with addSphere(), so we can comment
    // this out.
    //
    // m.primitive(Graphics::TRIANGLES);

    // this scales (multiplies) each vertex by the value. the CPU executes
    // these multiplications, not the GPU.
    //
    m.scale(0.2);

    // this uses math to figure out the normals for our mesh. this is critical
    // to lighting/shading. also, we must do this *after* all the verticies are
    // added and scaled.
    //
    m.generateNormals();

    // this will count from 0 to N - 1 (inclusive) and do some stuff for each i
    //
    for (int i = 0; i < N; ++i) {
      // set the initial frequency of the i-th sine according to some arbitrary
      // pattern that karl liked.
      //
      sine[i].freq(220.0 + i * i);

      // set the initial position of the i-th sphere according to some
      // arbitrary pattern that karl liked. we have sin, cos, tan, abs, and a
      // mess of other common math functions.
      //
      position[i] = Vec3f(sin(i), i, -i * i / 7);
    }

    // make a window cabable of rendering OpenGL stuff. this function takes a
    // bunch of optional arguments. look it up in the doxygen if you want to,
    // but the defaults are pretty good.
    //
    initWindow();

    // make sound happen. this will open the default sound device and setup a
    // thread that will be called each time ....
    //
    initAudio();
  }

  // onAnimate() is called before onDraw(). onAnimate() is envoked with 1
  // argument, the time elapsed since the class call to onAnimate(). we use
  // onAnimate() to calculate each step of our simulation. we don't use
  // onDraw() for this because onDraw() may be called many times if we have
  // more than 1 window.
  //
  virtual void onAnimate(double dt) {
    // THIS IS WHERE YOU SHOULD PUT YOUR CODE. you can delete the code below
    // after you digest it. here's my suggested order of operations.
    //
    // 0) detect collisions (maybe handle collisions)
    // 1) zero all forces
    // 2) calulate all forces on each sphere (gravity + drag) this will require
    // the use of a nested for loop. that is a for loop within a for loop.
    // 3) update velocity and position of each sphere (euler's method)
    // 4) draw scene (this gets done by onDraw(), not onAnimate())

    // here's some stuff you might use..
    //   vector (Vec) operations
    //     .normalize()
    //     .mag()
    //     +=
    //     *
    //   scalar operations
    //     pow()
    //     abs()
    //

    // this will count from 0 to N - 1 (inclusive) and do some stuff for each i
    //
    for (int i = 0; i < N; ++i) {
      // the next line adds a small, randomized vector to the position of
      // sphere i. since this happens about 40 times a second, it will make the
      // sphere look like it's wiggling.
      //
      position[i] +=
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.01;

      // the next line makes the frequency of the i-th sine oscillator
      // proportional to the distance (from the origin) of the i-th sphere. the
      // formula used is some arbitrary choice made by karl.
      //
      sine[i].freq(220.0 + position[i].mag() * 5);
    }
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();
    for (int i = 0; i < N; ++i) {
      g.pushMatrix();
      g.color(HSV((float)i / N, 1, 1));
      g.translate(position[i]);
      g.draw(m);
      g.popMatrix();
    }
  }

  virtual void onSound(AudioIOData& io) {
    // this next line tells Gamma to notify all oscillators of the current
    // sample rate. leave it here, as the first line of onSound().
    //
    gam::Sync::master().spu(audioIO().fps());

    // read this as "while there are samples to process..." or "for each
    // sample..."
    //
    while (io()) {
      // for each sphere + sine oscillator pairing (of which there are N), we
      // take into account it's contribution to the amplitude of the current
      // sample. we start off with a single, silent value.
      //
      float s = 0;

      for (int i = 0; i < N; ++i) {
        // we make the amplitude of a sine oscillator inversely proportional to
        // its distance from the viewer. nav().pos() is a Vec and position[i]
        // is a Vec. Vec supports subtraction. nav().pos() - position[i] is a
        // Vec that is the difference between the viewer and the i-th sphere.
        // when we add .mag(), we get the scalar distance from the viewer to
        // the i-th sphere.
        //
        // accumulate the contribution of the i-th sine oscillator after
        // adjusting its amplitude.
        //
        s += sine[i]() / (nav().pos() - position[i]).mag();
      }

      // we just accumulated N potentially unity-gain sine oscillators into a
      // single value, so the amplitude could be as high as N, but we'll get
      // clipping if the amplitude is greater than 1. we divide by N to avoid
      // clipping.
      //
      io.out(0) = io.out(1) = s / N;
    }
  }

  // this method will be called when a key is pressed. right now, it does not
  // do anything useful. it is our hope that you may use it to control some
  // part of the simulation, making it "interactive".
  //
  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    // here's an example of how to handle a key event. a few keys are used for
    // stuff already. toggle fullscreen with ESC.  navigate with Left, Right,
    // Up, Down, a, q, w, e, d, c, x, z. toggle stereo rendering with TAB.
    //
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

// main is a special function. it is the entry point for a c++ program; main is
// the very first function called. we use it to start everything else.
//
int main(int argc, char* argv[]) {
  // create a AlloApp named app. this puts a AlloApp in memory and creates a
  // variable named app, then c++ calls the AlloApp constructor for app.
  //
  AlloApp app;

  // we call start() after our AlloApp is allocated and constructed (above). it
  // starts the sound, graphics, and animation threads. start() blocks until we
  // quit our AlloApp.
  //
  app.start();

  // the return value of main tells the shell whether our program was a success
  // (0) or a failure (non-0).
  //
  return 0;
}

// you may see a main function like the one above shortened to this:
//
// int main() { AlloApp().start(); }
//
// this is because...
// 1) main is special in C++ in that you don't have to return anything
// 2) we aren't using argc/argv, so we can omit them
// 3) we only use the app variable once to call start(), so AlloApp().start();
