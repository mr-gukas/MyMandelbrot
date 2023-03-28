#include "mandelbrot.h"


inline void moveDown (picture_t* picture) {picture->yc -= 5. * delta;}
inline void moveUp   (picture_t* picture) {picture->yc += 5. * delta;}
inline void moveLeft (picture_t* picture) {picture->xc -= 5. * delta;}
inline void moveRight(picture_t* picture) {picture->xc += 5. * delta;}
inline void zoomIn   (picture_t* picture) {picture->scale *= .5;}
inline void zoomOut  (picture_t* picture) {picture->scale *= 1.2;}
inline void resetView(picture_t* picture) 
{
    picture->xc = x_centre;
    picture->yc = y_centre;
    picture->scale = 1.;
}

int pictureCtor(picture_t* picture, size_t width, size_t height)
{
    if (!picture) return 1;

    unsigned char* pixels = (unsigned char*) calloc(4 * width * height, sizeof(unsigned char));
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


int convert2clr(picture_t* picture, int iterX, int iterY, unsigned int n)
{
    if (picture == NULL) return 1;
    
    *(picture->pixels + 4 * iterY * picture->width + 4 * iterX) = (unsigned int) 100*n;
    *(picture->pixels + 4 * iterY * picture->width + 4 * iterX + 1) = (unsigned int) (25 * (n%2));
    *(picture->pixels + 4 * iterY * picture->width + 4 * iterX + 2) = (unsigned int) (10*(255-n));
    *(picture->pixels + 4 * iterY * picture->width + 4 * iterX + 3) = (unsigned int) 0xFF;

    return 0;
}

int mandelbrotFrac(picture_t* picture)
{
    if (!picture) return 1;

    for (int iterY = 0; iterY < picture->height; iterY++)
    {
        double y0 = picture->scale * (picture->y_max - iterY*dy + picture->yc);  

        for (int iterX = 0; iterX < picture->width; iterX++)
        {
            double x0 = picture->scale * (picture->x_min + iterX*dx + picture->xc);
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
            
            convert2clr(picture, iterX, iterY, n);
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
            case sf::Keyboard::Key::J:     moveDown (picture); break;
            case sf::Keyboard::Key::K:     moveUp   (picture); break;
            case sf::Keyboard::Key::H:     moveLeft (picture); break;
            case sf::Keyboard::Key::L:     moveRight(picture); break;
            case sf::Keyboard::Key::R:     resetView(picture); break;

            case sf::Keyboard::Key::Left:  zoomIn   (picture); break;
            case sf::Keyboard::Key::Right: zoomOut  (picture); break;
            
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





















