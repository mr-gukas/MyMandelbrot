#include "mandelbrot.h"


inline void moveDown (picture_t* picture) {picture->yc -= 5. * delta;}
inline void moveUp   (picture_t* picture) {picture->yc += 5. * delta;}
inline void moveLeft (picture_t* picture) {picture->xc -= 5. * delta;}
inline void moveRight(picture_t* picture) {picture->xc += 5. * delta;}
inline void zoomIn   (picture_t* picture)
{
    picture->scale *= .5;
    picture->shift *= .5;
}

inline void zoomOut  (picture_t* picture)
{
    picture->scale *= 2.0;
    picture->shift *= 2.0;
}

inline void resetView(picture_t* picture) 
{
    picture->xc = x_centre;
    picture->yc = y_centre;
    picture->scale = 1.;
}

int pictureCtor(picture_t* picture, size_t width, size_t height)
{
    if (!picture) return 1;

    unsigned int* pixels = (unsigned int*) calloc(4 * width * height, sizeof(unsigned int));
    if (pixels == NULL) return 1;

    picture->pixels = pixels;
    picture->width  = width;
    picture->height = height;
    
    return 0;
}

int pictureDtor(picture_t* picture)
{
    if (!picture) return 1;

    free(picture->pixels);

    return 0;
}


int convert2clr(picture_t* picture, size_t pxlnum, int color)
{
    if (picture == NULL) return 1;

    if (color == n_max)
        picture->pixels[pxlnum] = 0xFF000000;
    else
    {
        color %= 16;
        picture->pixels[pxlnum] = palette[color][0]       |
                                  palette[color][1] << 8  |
                                  palette[color][2] << 16 |
                                              0xFF  << 24;
    }

    return 0;
}

int mandelbrotFrac(picture_t* picture)
{
    if (!picture) return 1;

    for (int iterY = 0; iterY < picture->height; iterY++)
    {
        double y0 = picture->scale * (picture->y_max - iterY*dy) + picture->yc;  

        for (int iterX = 0; iterX < picture->width; iterX++)
        {
            double x0 = picture->scale * (picture->x_min + iterX*dx) + picture->xc;
            int n = 0;

            double x = 0.;
            double y = 0.;

            for (; n < n_max; ++n)
            {
                double x2 = x*x;
                double y2 = y*y;
                double xy = x*y;

                if (x2+y2 > r_max)
                    break;

                x = x2 - y2 + x0;
                y = xy + xy + y0;
            }
            
            convert2clr(picture, picture->width * iterY + iterX, n);
        }
    }

    return 0;
}

int cameraMotion(picture_t* picture, sf::Event &event)
{
    if (!picture) return 1;

    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::Key::S:     moveDown (picture); break;
            case sf::Keyboard::Key::W:     moveUp   (picture); break;
            case sf::Keyboard::Key::A:     moveLeft (picture); break;
            case sf::Keyboard::Key::D:     moveRight(picture); break;
            case sf::Keyboard::Key::R:     resetView(picture); break;

            case sf::Keyboard::Key::J:  zoomIn   (picture); break;
            case sf::Keyboard::Key::K:  zoomOut  (picture); break;
            
            default: break;
        }

    }

    return 0;
}

int mandelCtor(mandel_t* mandel)
{
    if (!mandel) return 1;

    mandel->window.create(sf::VideoMode(800, 600), "Mandelbrot");

    if (!mandel->font.loadFromFile("../src/font.ttf")) {
        return 1; 
    }
    
    mandel->fpsText.setFont(mandel->font);
    mandel->fpsText.setString("");
    mandel->fpsText.setCharacterSize(10);
    mandel->fpsText.setFillColor(sf::Color::White);
    mandel->fpsText.setPosition(sf::Vector2f(0, 0));

    return 0;
}

int setFps(mandel_t* mandel)
{
    if (!mandel) return 1;

    sf::Time elapsed = mandel->clock.restart();

    float fps = 1.0f / elapsed.asSeconds();

    std::ostringstream ss;
    ss << "FPS: " << fps;

    mandel->fpsText.setString(ss.str());

    return 0;
}

int imageCtor(image_t* image, picture_t* picture)
{
    if (!picture) return 1;

    image->fractal.create(800, 600, (unsigned char*)picture->pixels);
    image->texture.create(800, 600);
    image->texture.update(image->fractal);
    image->sprite.setTexture(image->texture);

    return 0;
}





















