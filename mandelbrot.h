#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream> // for std::ostringstream
#include <stdlib.h>

const double delta = 0.1;
const double dx    = 4.f / 800;
const double dy    = 4.f / 600;
const double x_centre = .0;
const double y_centre = 0.;

const double r_max = 100.f;
const size_t n_max = 256;

struct picture_t
{
    double x_max = 2.;
    double x_min = -2.;
    double y_min = -2.;
    double y_max = 2.;

    size_t width;
    size_t height;
    
    double scale = 1.;
    double x0_init;
    double y0_init;
    
    double xc = .0;
    double yc = 0.0;

    unsigned int* pixels;
};

struct mandel_t
{
    sf::RenderWindow window;
    sf::Font         font;
    sf::Text         fpsText;
    sf::Clock        clock;
};

struct image_t
{
    sf::Image   fractal;
    sf::Texture texture; 
    sf::Sprite  sprite;
};

int mandelCtor(mandel_t* mandel);
int pictureCtor(picture_t* picture, size_t width, size_t height);
int pictureDtor(picture_t* picture);
int mandelbrotFrac(picture_t* picture);
int convert2clr(picture_t* picture, size_t pxlnum, int color);
int cameraMotion(picture_t* picture, sf::Event &event);
int setFps(mandel_t* mandel);
int imageCtor(image_t* image, picture_t* picture);

