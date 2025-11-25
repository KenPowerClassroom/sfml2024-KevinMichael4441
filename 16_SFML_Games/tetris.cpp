#include <SFML/Graphics.hpp>
#include <time.h>

#include <memory>

using namespace sf;

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 320;
const int HEIGHT = 20;
const int WIDTH = 10;
const int MAX_BLOCKS = 4;
const int NO_OF_FIGURES = 7;

//Global variables

int moveInX; 
bool shouldRotate; 
int colorNum;
float timer;
float delay;
Sprite tile, background, frame;


std::vector<std::vector<int>> field(HEIGHT, std::vector<int>(WIDTH, 0));




struct Point
{int x,y;} actual[MAX_BLOCKS], placeHolder[MAX_BLOCKS];

int figures[NO_OF_FIGURES][MAX_BLOCKS] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

void initializeGame(Texture &t_tilesTexture, Texture &t_backgroundTexture, Texture &t_frameTexture)
{
    //srand(time(0));

    t_tilesTexture.loadFromFile("images/tetris/tiles.png");
    t_backgroundTexture.loadFromFile("images/tetris/background.png");
    t_frameTexture.loadFromFile("images/tetris/frame.png");

    tile.setTexture(t_tilesTexture);
    background.setTexture(t_backgroundTexture);
    frame.setTexture(t_frameTexture);

    moveInX = 0;
    shouldRotate = 0;
    colorNum = 1;
    timer = 0;
    delay = 0.3;
}

class Check
{
public:

    Check(Point t_actual[], int t_sizeofActual, std::vector<std::vector<int>> t_field)
    {
        m_field = t_field;

        m_actual.resize(t_sizeofActual);
        for (int index = 0; index < t_sizeofActual; index++)
        {
            m_actual[index] = t_actual[index];
        }
    }

    bool runningTheCheck()
    {
        for (int i = 0; i < MAX_BLOCKS; i++)
        {
            if (actual[i].x < 0 || actual[i].x >= WIDTH || actual[i].y >= HEIGHT)
            {
                return false;
            }
            else if (field[actual[i].y][actual[i].x])
            {
                return false;
            }
        }
            
        return  true;
    }


private:
    int const m_WIDTH = 10;
    int const m_HEIGHT = 20;

    std::vector<Point> m_actual;
    std::vector<std::vector<int>> m_field;

};

void updateActual()
{
    Check check(actual, MAX_BLOCKS, field);
    if (!check.runningTheCheck())
    {
        for (int i = 0; i < MAX_BLOCKS; i++)
        {
            actual[i] = placeHolder[i];
        }
    }   
}

void move()
{
    for (int i = 0; i < MAX_BLOCKS; i++) 
    { 
        placeHolder[i] = actual[i]; actual[i].x += moveInX; 
    }
    updateActual();
}

void rotate()
{
    if (shouldRotate)
    {
        Point point = actual[1]; //center of rotation
        for (int i = 0; i < MAX_BLOCKS; i++)
        {
            int deltaX = actual[i].y - point.y;
            int deltaY = actual[i].x - point.x;
            actual[i].x = point.x - deltaX;
            actual[i].y = point.y + deltaY;
        }
        updateActual();
    }
}

void tick()
{
    if (timer > delay)
    {
        for (int i = 0; i < MAX_BLOCKS; i++)
        {
            placeHolder[i] = actual[i]; actual[i].y += 1;
        }

        Check check(actual, MAX_BLOCKS, field);
        if (!check.runningTheCheck())
        {
            for (int i = 0; i < MAX_BLOCKS; i++)
            {
                field[placeHolder[i].y][placeHolder[i].x] = colorNum;
            }

            colorNum = 1 + rand() % 7;
            int randomNumber = rand() % 7;
            for (int i = 0; i < MAX_BLOCKS; i++)
            {
                actual[i].x = figures[randomNumber][i] % 2;
                actual[i].y = figures[randomNumber][i] / 2;
            }
        }

        timer = 0;
    }
}
void chceckLines()
{
    int indexFromTop= HEIGHT - 1;
        for (int i= HEIGHT -1;i>0;i--)
        {
            int count=0;
            for (int j=0;j< WIDTH;j++)
            {
                if (field[i][j]) count++;
                    field[indexFromTop][j]=field[i][j];
            }
            if (count< WIDTH) indexFromTop--;
        }
}
void draw(RenderWindow &t_window)
{
    t_window.clear(Color::White);
    t_window.draw(background);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (field[i][j] == 0) continue;
            tile.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
            tile.setPosition(j * 18, i * 18);
            tile.move(28, 31); //offset
            t_window.draw(tile);
        }
    }
    for (int i = 0; i < MAX_BLOCKS; i++)
    {
        tile.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
        tile.setPosition(actual[i].x * 18, actual[i].y * 18);
        tile.move(28, 31); //offset
        t_window.draw(tile);
    }

    t_window.draw(frame);
    t_window.display();
}


int tetris()
{
    srand(time(0));

    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "The Game!");

    
    Texture tilesTexture, backgroundTexture, frameTexture;
    initializeGame(tilesTexture, backgroundTexture, frameTexture);

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
              if (event.key.code==Keyboard::Up) shouldRotate =true;
              else if (event.key.code==Keyboard::Left) moveInX =-1;
              else if (event.key.code==Keyboard::Right) moveInX =1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

        //// <- Move -> ///
        move();
        
        //////Rotate//////
        rotate();

        ///////Tick//////
        tick();

        ///////check lines//////////
        chceckLines();

        moveInX = 0; shouldRotate = 0; delay = 0.3;

        /////////draw//////////
        draw(window);
        
    }

    return 0;
}
