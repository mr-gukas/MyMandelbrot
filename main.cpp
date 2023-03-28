#include "mandelbrot.h"

int main()
{
    picture_t picture = {};
    pictureCtor(&picture, 800, 600);
    
    mandel_t mandel = {};
    mandelCtor(&mandel);

    while (mandel.window.isOpen())
    {

        sf::Event event;
        while (mandel.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                mandel.window.close();

            cameraMotion(&picture, event);
        }

        mandelbrotFrac(&picture);
        
        image_t image = {};
        imageCtor(&image, &picture); 

        setFps(&mandel);

        mandel.window.clear();
        image.texture.update(image.fractal);
        mandel.window.draw (image.sprite);
        mandel.window.draw(mandel.fpsText);
        mandel.window.display();
    }
    
    pictureDtor(&picture);

    return 0;
}

