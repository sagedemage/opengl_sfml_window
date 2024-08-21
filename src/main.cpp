/* Standard libraries */
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

/* Local header files */
#include "opengl_driver/opengl_driver.hpp"

constexpr unsigned int LEVEL_WIDTH = 750;
constexpr unsigned int LEVEL_HEIGHT = 500;

int main() {
    const std::string music_file_path = "assets/music/emergency.ogg";
    const int volume = 50;

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    std::string debug_msg;

    // window
    sf::RenderWindow window(sf::VideoMode(LEVEL_WIDTH, LEVEL_HEIGHT),
                            "OpenGL SFML Demo", sf::Style::Default, settings);

    // Vsync
    window.setVerticalSyncEnabled(true);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        const char* glew_error_msg_char_p =
            reinterpret_cast<const char*>(glewGetErrorString(glew_error));
        std::string glew_error_msg =
            static_cast<std::string>(glew_error_msg_char_p);

        debug_msg = "Error initializing GLEW! " + glew_error_msg;
        std::cerr << debug_msg << std::endl;
        return -1;
    }

    OpenGLDriver opengl_driver = OpenGLDriver();

    // Initialize OpenGL
    opengl_driver.InitGL();

    glEnable(GL_TEXTURE_2D);

    // Activate the window
    window.setActive(true);

    // Set framerate to 60 frames per second
    window.setFramerateLimit(60);

    // Set and play music
    sf::Music music;
    if (!music.openFromFile(music_file_path)) {
        return -1;
    }

    music.setVolume(volume);

    music.setLoop(true);
    music.play();

    GLsizei viewport_width = 0;
    GLsizei viewport_height = 0;

    bool quit = false;
    while (!quit) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    quit = true;
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        quit = true;
                        window.close();
                    }
                    break;
                case sf::Event::Resized:
                    // adjust the viewport when the window is resized
                    viewport_width = static_cast<GLsizei>(event.size.width);
                    viewport_height = static_cast<GLsizei>(event.size.height);
                    glViewport(0, 0, viewport_width, viewport_height);
                    break;
                default:
                    break;
            }
        }

        // Hold Keybindings
        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            // move player left
            player.move(-speed, 0.F);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            // move player right
            player.move(speed, 0.F);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            // move player up
            player.move(0.F, -speed);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            // move player down
            player.move(0.F, speed);
        }*/

        // set background color of the window
        // window.clear(sf::Color(84, 193, 255, 255));
        // window.draw(player);

        // Render the scene
        opengl_driver.Render();

        // end the current frame
        window.display();
    }

    // deactivate the window's context
    window.setActive(false);

    window.close();

    return 0;
}
