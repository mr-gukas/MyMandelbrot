#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream> // for std::ostringstream
#include <stdlib.h>

const float dx       = 4.f / 800;
const float dy       = 3.f / 600;
const float x_centre = -.5;
const float y_centre = 0.;
const float r_max    = 4.f;
const size_t n_max   = 256;

const unsigned char palette[16][3] =
{
    {60,   30,  15},
    {25,    7,  26},
    {9,     1,  47},
    {4,     4,  73},
    {0,     7, 100},
    {12,   44, 138},
    {24,   82, 177},
    {57,  125, 209},
    {134, 181, 229},
    {211, 236, 248},
    {241, 233, 191},
    {248, 201,  95},
    {255, 170,   0},
    {204, 128,   0},
    {153,  87,   0},
    {106,  52,   3},
};

struct picture_t
{
    float x_max =   2.;
    float x_min =  -2.;
    float y_min = -1.5;
    float y_max =  1.5;

    size_t width;
    size_t height;
    
    float shift = .1;
    float scale = 1.;
    
    float xc = -.5;
    float yc =  .0;

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

