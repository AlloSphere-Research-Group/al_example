// MAT 201B Homework 2, Problem 1
// By Karl Yerkes and Matt Wright (2012)
// Now with FileList Tim Wood (2015)
//
// This code loads an image, treats each pixel as a particle
// (represented by a point in a Mesh), draws each particle using
// a point sprite, and on each frame adds random noise to the
// position of each particle, which is reset on any keypress.

#include "allocore/io/al_App.hpp"
#include <cassert>  // gets you assert()

using namespace al;
using namespace std;

// for sorting the FileList
bool sort(FilePath a, FilePath b){ return a.filepath() < b.filepath();}

struct AlloApp : App {
  Texture textureSprite;
  Mesh mesh;
  vector<Vec3f> homePosition;  // note the use of std::vector<>
  double time;
  FileList files;

  AlloApp() {

    SearchPaths searchPaths;
    searchPaths.addSearchPath("handouts");
    searchPaths.addSearchPath("student");

    // matches .png .jpg files and adds them to a FileList
    files = searchPaths.glob("(.*)\\.(png|jpg|JPG)");
    cout << files.count() << endl;
    files.sort(sort); // sort files by filepath
    files.print();

    // load currently selected file 
    loadImage(files().filepath());
    
    // we're going to render each vertex in the mesh as a so called "point
    // sprite" because it's prettier than rendering with simple points and it's
    // cheaper than rendering with quads or circles. we make a 16x16 texture and
    // paint a nice greyscale gradient dot in there.
    //
    textureSprite =
        Texture(16, 16, Graphics::LUMINANCE_ALPHA, Graphics::UBYTE, true);
    Array& sprite(textureSprite.array());

    struct {
      uint8_t l, a;
    } lum;

    for (size_t row = 0; row < sprite.height(); ++row) {
      float y = float(row) / (sprite.height() - 1) * 2 - 1;
      for (size_t col = 0; col < sprite.width(); ++col) {
        float x = float(col) / (sprite.width() - 1) * 2 - 1;
        lum.l = 256 * (1 - al::clip(hypot(x, y)));
        lum.a = lum.l;
        sprite.write(&lum.l, col, row);
      }
    }

    // move the viewer right, up, and back so the image fits in the screen.
    //
    nav().pos(1.0, .5, 5);

    initWindow();
    initAudio();

    // here's a variable to help keep track of time.
    //
    time = 0;
  }

  void loadImage(std::string imageFullPath){
    mesh.reset();
    homePosition.clear();

    cout << "Full path to your image file is " << imageFullPath << endl;

    // try to load the given image file
    //
    Image image;
    if (!image.load(imageFullPath)) {
      cerr << "FAIL: your image " << imageFullPath << " did not load." << endl;
      exit(1);
    }

    // make a local alias to the 2D array inside the image object.
    //
    Array& array(image.array());

    cout << "array has " << (int)array.components() << " components" << endl;
    cout << "array has type " << allo_type_name(array.type()) << endl;

    // this will dump lots of information on the array
    //
    array.print();

    // this will terminate the program with an error unless the condition is
    // true.
    //
    assert(array.type() == AlloUInt8Ty);

    // this variable will hold each pixel value. its type is "RGB pixel where
    // each of R, G, and B is an unsigned 8-bit integer".
    //
    Image::RGBPix<uint8_t> pixel;

    // read the color value of each of the image's pixels. For each image
    // pixel, add a vertex to our mesh with color same as the image pixel.
    //
    for (size_t row = 0; row < array.height(); ++row) {
      for (size_t col = 0; col < array.width(); ++col) {
        // read the current pixel value from the image
        //
        array.read(&pixel.r, col, row);

        // interpret the column position as the x coordinate and the row
        // position as the y coordinate; normalize both by dividing by the
        // actual height of the array (so y values go from 0 to 1 regardless
        // of the resolution of the image).
        //
        Vec3f position(float(col) / array.height(), float(row) / array.height(),
                       0);

        // remember the original "home" position of each pixel so we can reset
        // when the user presses a key. the std::vector class has a method
        // "push_back" that adds a new value (in this case a vec3f) to the end
        // (the "back") of the vector.
        //
        homePosition.push_back(position);

        // push the pixel's position onto our mesh. add a color (below).
        //
        mesh.vertex(position);

        // image pixels are stored as 8-bit unsigned integers (from 0
        // to 255), but mesh colors are floats (from 0.0 to 1.0), so
        // we scale the r, g, and b values by 1/256.
        //
        mesh.color(Color(pixel.r / 256.0, pixel.g / 256.0, pixel.b / 256.0));
      }
    }

    mesh.primitive(Graphics::POINTS);

  }

  virtual void onAnimate(double dt) {
    // accumulate time steps (dt) into our time variable.
    time += dt;

    ////////////////////////////////////////////////////////////
    // put your code here. animate the verticies of the mesh. //
    ////////////////////////////////////////////////////////////

    // delete this code or comment it out once you start your homework.
    //
    for (int i = 0; i < mesh.vertices().size(); i++)
      mesh.vertices()[i] +=
          0.01f * Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS());
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    ///////////////////////////////////////////////////////////
    // put your code here. detect key presses and take action.
    ///////////////////////////////////////////////////////////

    // delete this code or comment it out once you start your homework.
    //
    for (int i = 0; i < mesh.vertices().size(); i++)
      mesh.vertices()[i] = homePosition[i];

    // select next or previous file and reload image
    if(k.key() == '.'){
      files.next();
      loadImage(files().filepath());
    } else if(k.key() == ','){
      files.prev();
      loadImage(files().filepath());
    }
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    // some settings that make stuff look nice
    //
    g.depthTesting(false);
    g.blending(true);
    g.blendModeTrans();
    g.pointSize(10);

    // you can use raw OpenGL calls, if you want.
    //
    glEnable(GL_POINT_SPRITE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

    // draw our mesh with texture sprites enabled and our texture "bound".
    //
    textureSprite.bind();
    g.draw(mesh);
    textureSprite.unbind();

    glDisable(GL_POINT_SPRITE);
  }

  virtual void onSound(AudioIOData& io) {
    // don't forget to add the line below once you start using Gamma
    //
    // gam::Sync::master().spu(audioIO().fps());
    while (io()) {
      float s = 0;
      io.out(0) = s;
      io.out(1) = s;
    }
  }
};

int main() { AlloApp().start(); }
