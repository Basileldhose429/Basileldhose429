#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GRAVITY = 0.5;
const float JUMP = -10.0;
const float PIPE_SPEED = 5.0;
const int PIPE_SPACING = 200;
const int PIPE_WIDTH = 80;
const int PIPE_HEIGHT = 400;
const int BIRD_RADIUS = 20;

// Function to check collision between bird and pipes
bool isColliding(const sf::CircleShape& bird, const sf::RectangleShape& pipe) {
    return bird.getGlobalBounds().intersects(pipe.getGlobalBounds());
}

int main() {
    // Initialize the game window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flappy Bird");

    // Seed random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Bird
    sf::CircleShape bird(BIRD_RADIUS);
    bird.setFillColor(sf::Color::Yellow);
    bird.setPosition(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2);

    // Pipes
    std::vector<sf::RectangleShape> pipes;
    float pipeX = WINDOW_WIDTH;
    for (int i = 0; i < 3; ++i) {
        float pipeHeight = std::rand() % (PIPE_HEIGHT / 2) + PIPE_HEIGHT / 4;
        
        sf::RectangleShape topPipe(sf::Vector2f(PIPE_WIDTH, pipeHeight));
        topPipe.setFillColor(sf::Color::Green);
        topPipe.setPosition(pipeX, 0);
        pipes.push_back(topPipe);

        sf::RectangleShape bottomPipe(sf::Vector2f(PIPE_WIDTH, PIPE_HEIGHT - pipeHeight));
        bottomPipe.setFillColor(sf::Color::Green);
        bottomPipe.setPosition(pipeX, pipeHeight + PIPE_SPACING);
        pipes.push_back(bottomPipe);

        pipeX += PIPE_WIDTH * 3;
    }

    // Game variables
    float birdVelocity = 0;
    bool isGameOver = false;
    int score = 0;

    // Main game loop
    while (window.isOpen()) {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                birdVelocity = JUMP;
            }
        }

        // Update game state
        if (!isGameOver) {
            birdVelocity += GRAVITY;
            bird.move(0, birdVelocity);

            for (auto& pipe : pipes) {
                pipe.move(-PIPE_SPEED, 0);
            }

            // Check for collisions
            for (const auto& pipe : pipes) {
                if (isColliding(bird, pipe)) {
                    isGameOver = true;
                }
            }

            // Check if bird goes out of bounds
            if (bird.getPosition().y > WINDOW_HEIGHT || bird.getPosition().y < 0) {
                isGameOver = true;
            }

            // Reset pipes and update score
            for (size_t i = 0; i < pipes.size(); i += 2) {
                if (pipes[i].getPosition().x + PIPE_WIDTH < 0) {
                    float pipeHeight = std::rand() % (PIPE_HEIGHT / 2) + PIPE_HEIGHT / 4;

                    pipes[i].setSize(sf::Vector2f(PIPE_WIDTH, pipeHeight));
                    pipes[i].setPosition(WINDOW_WIDTH, 0);

                    pipes[i + 1].setSize(sf::Vector2f(PIPE_WIDTH, PIPE_HEIGHT - pipeHeight));
                    pipes[i + 1].setPosition(WINDOW_WIDTH, pipeHeight + PIPE_SPACING);

                    score++;
                }
            }
        }

        // Render
        window.clear();
        window.draw(bird);
        for (const auto& pipe : pipes) {
            window.draw(pipe);
        }
        window.display();
    }

    return 0;
}
