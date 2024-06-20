#include <SFML/Graphics.hpp>
#include <sstream>
#include <ctime>

sf::RenderWindow window(sf::VideoMode(700, 500), "Snake");
sf::Clock time1;
sf::Clock time2;
sf::Text text;
sf::RectangleShape snake(sf::Vector2f(20,20));
sf::RectangleShape bite(sf::Vector2f(20,20));
sf::Font font;
sf::Image icon;

bool start = false;
bool stop = false;
bool grow = false;
bool badbite = false;

std::stringstream ss;

int d;
int snakelength = 5;
int finallength = 5;
int bitex = 0;
int bitey;
int snakex[1000];
int snakey[1000];
int speed = 65;

void movefood() {
    badbite = true;
    while(badbite == true) {
        badbite = false;
        bitex = (rand()%35)*20+10;
        bitey = (rand()%25)*20+10;
        for(int b = 0; b<snakelength; b++) {
            if(bitex == snakex[b] && bitey == snakey[b]) {
                badbite = true;
                break;
            }
        }
    }
}

void reset() {
    grow = false;
    time1.restart();
    time2.restart();
    snakelength = 5;
    finallength = 5;
    d = 2;
    for(int x = 0; x<snakelength; x++) {
        snakex[x] = 10+20*x;
        snakey[x] = 250;
    }
    movefood();
}

void change() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if(d != 3) {
            d = 1;
        }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if(d != 4) {
            d = 2;
        }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if(d != 1) {
            d = 3;
        }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if(d != 2) {
            d = 4;
        }
    }

    time2.restart();
}

void eatfood() {
     grow = true;
     finallength = finallength + rand()%10+1;
     movefood();
}

void snakemove() {
    if(grow == false) {
        for(int a = 0; a<snakelength-1; a++) {
            snakex[a] = snakex[a+1];
            snakey[a] = snakey[a+1];
        }
        if(d == 1) {
            snakey[snakelength-1] = snakey[snakelength-1] - 20;
        } else if(d == 2) {
            snakex[snakelength-1] = snakex[snakelength-1] + 20;
        } else if(d == 3) {
            snakey[snakelength-1] = snakey[snakelength-1] + 20;
        } else if(d == 4) {
            snakex[snakelength-1] = snakex[snakelength-1] - 20;
        }
    } else {
        snakelength++;
        if(d == 1) {
            snakey[snakelength-1] = snakey[snakelength-2] - 20;
            snakex[snakelength-1] = snakex[snakelength-2];
        } else if(d == 2) {
            snakex[snakelength-1] = snakex[snakelength-2] + 20;
            snakey[snakelength-1] = snakey[snakelength-2];
        } else if(d == 3) {
            snakey[snakelength-1] = snakey[snakelength-2] + 20;
            snakex[snakelength-1] = snakex[snakelength-2];
        } else if(d == 4) {
            snakex[snakelength-1] = snakex[snakelength-2] - 20;
            snakey[snakelength-1] = snakey[snakelength-2];
        }
        if(snakelength == finallength) {
            grow = false;
        }
    }
    time1.restart();
}

void snakelife() {
    if(snakex[snakelength-1] == -10 || snakex[snakelength-1] == 710 || snakey[snakelength-1] == -10 || snakey[snakelength-1] == 510) {
        start = false;
        stop = true;
        ss.str("");
        ss << snakelength-5;
        text.setString("Your Score: " + ss.str() + "\nPress Space to Restart");
    }
    for(int z=0; z<snakelength-1; z++) {
        if(snakex[snakelength-1] == snakex[z] && snakey[snakelength-1] == snakey[z]) {
            start = false;
            stop = true;
            ss.str("");
            ss << snakelength-5;
            text.setString("Your Score: " + ss.str() + "\nPress Space to Restart");
        }
    }
}

void draw() {
    window.clear();

    if(start == true) {
        snake.setFillColor(sf::Color::Blue);
        bite.setPosition(bitex,bitey);
        window.draw(bite);
        for(int i = 0; i<snakelength; i++) {
            snake.setPosition(snakex[i],snakey[i]);
            if(i == (snakelength-1)) {
                snake.setFillColor(sf::Color::Red);
            }
            window.draw(snake);
        }
    } else {
        window.draw(text);
    }
    window.display();
}

int main() {

    window.setFramerateLimit(60);
    icon.loadFromFile("icon.png");
    window.setIcon(32,32,icon.getPixelsPtr());
    srand(time(0));
    snake.setFillColor(sf::Color::Blue);
    snake.setOrigin(10,10);
    bite.setFillColor(sf::Color::Green);
    bite.setOrigin(10,10);
    font.loadFromFile("font.ttf");
    text.setFillColor(sf::Color::Blue);
    text.setCharacterSize(38);
    text.setPosition(20,20);
    text.setFont(font);
    text.setString("Press Space to Start\nArrow Keys to Move");

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

        }
        if(time2.getElapsedTime().asMilliseconds() > 50) {
            change();
        }
        if(time1.getElapsedTime().asMilliseconds() > speed) {
            if(start == false) {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    start = true;
                    stop = false;
                    reset();
                }
            } else {
                snakemove();
                if(bitex == snakex[snakelength-1] && bitey == snakey[snakelength-1]) {
                    eatfood();
                }
                snakelife();
            }
            draw();
        }
    }
}
