#include "mandelbrot.h"


inline void moveDown (picture_t* picture) {picture->yc -= 5. * picture->shift;}
inline void moveUp   (picture_t* picture) {picture->yc += 5. * picture->shift;}
inline void moveLeft (picture_t* picture) {picture->xc -= 5. * picture->shift;}
inline void moveRight(picture_t* picture) {picture->xc += 5. * picture->shift;}
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
    picture->shift = .1;
}

int pictureCtor(picture_t* picture, size_t width, size_t height)
{
    if (!picture) return 1;

    unsigned int* pixels = (unsigned int*) calloc(width * height, sizeof(unsigned int));
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
    
    __m256  _3210  = _mm256_set_ps(7, 6, 5, 4, 3, 2, 1, 0);
    __m256 r2_max  = _mm256_set1_ps(r_max);
    __m256     DX  = _mm256_set1_ps(dx);
    __m256 _3210dx = _mm256_mul_ps(_3210, DX);
    
    for (int iterY = 0; iterY < picture->height; iterY++)
    {
        float      y0 = picture->scale * (picture->y_max - iterY*dy) + picture->yc;  
        __m256 y0_arr = _mm256_set1_ps(y0);
        float      x0 =  picture->scale * (picture->x_min) + picture->xc;

        for (int iterX = 0; iterX < picture->width; iterX += 8, x0 += 8 * dx * picture->scale)
        {
            __m256 x0_arr = _mm256_add_ps(_mm256_set1_ps(x0), _mm256_mul_ps(_mm256_set1_ps(picture->scale), _3210dx));  

            __m256i N = _mm256_setzero_si256();
            int     n = 0;

            __m256 x = x0_arr;
            __m256 y = y0_arr;

            for (; n < n_max; ++n)
            {
                __m256 x2 = _mm256_mul_ps(x, x);
                __m256 y2 = _mm256_mul_ps(y, y);
                __m256 r2 = _mm256_add_ps(x2, y2);

                __m256 cmp = _mm256_cmp_ps(r2, r2_max, _CMP_LE_OQ);
                int    mask = _mm256_movemask_ps(cmp);

                if (!mask) break;

                N = _mm256_sub_epi32(N, _mm256_castps_si256(cmp));

                __m256 xy = _mm256_mul_ps(x, y);

                x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0_arr);
                y = _mm256_add_ps(_mm256_add_ps(xy, xy), y0_arr);
            }
            
            int *colors = (int *) &N;

            for (int pxlInd = 0; pxlInd < 8; ++pxlInd)
            {      
                convert2clr(picture, picture->width * iterY + iterX + pxlInd, (int) colors[pxlInd]);
            }

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
            case sf::Keyboard::Key::S: moveDown (picture); break;
            case sf::Keyboard::Key::W: moveUp   (picture); break;
            case sf::Keyboard::Key::A: moveLeft (picture); break;
            case sf::Keyboard::Key::D: moveRight(picture); break;
            case sf::Keyboard::Key::R: resetView(picture); break;

            case sf::Keyboard::Key::J: zoomIn   (picture); break;
            case sf::Keyboard::Key::K: zoomOut  (picture); break;
            
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





















