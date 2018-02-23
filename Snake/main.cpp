//
//  main.cpp
//  Snake
//
//  Created by Jay Tunkie Saunders on 2/17/18.
//  Copyright © 2018 Jay Tunkie Saunders. All rights reserved.
//
//  Current Bugs:
//  - Fruit spawns under snek tail
//  - Snek can run over itself when it is size 2
//  - Instructions menu doesn't work
//  - ...
//
//  Add-ons:
//  - Allow user to customize the board size and other features
//  - Unlock new snek skins
//  - ...

#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <vector>

using namespace std; // simplify code
bool gameOver; // initialize variables for head and fruit position
bool menu;
int width = 20; // removed constant 'const' type spec
int height = 20;
int x, y, fruitX, fruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

int x_prev; // declare tail variables
int y_prev;
vector<int> x_hist = {1};
vector<int> y_hist = {1};

void Instructions() // Gameplay instructions for the user
{
    //clear();
    system("clear");
    mvprintw(height / 2 - 4, width / 2, "Don't Step On Snek - Instructions");
    mvprintw(height / 2, width / 2, "You are a snek! Your head is O and you want to eat X.");
    mvprintw(height / 2 + 2, width / 2, "Everytime you eat X you grow!");
    mvprintw(height / 2 + 4, width / 2, "However, be careful since if you hit yourself or the walls you'll die!!!");
    mvprintw(height / 2 + 6, width / 2, "AWSD keys control make the snek move left, up, down, and right, respectively.");
    mvprintw(height / 2 + 8, width / 2, "Arrow keys also controls the snek.");
    mvprintw(height / 2 + 10, width / 2, "You can quit the game at any time by pressing q.");
}

void Customize() // Allow user to customize gameplay
{
    //clear();
    system("clear");
    cout << "Enter Desired Width: " << endl;
    cin >> width;
    
    cout << "Enter Desired Height: " << endl;
    cin >> height;
    
}

void Menu() // Main menu of the game
{
    clear();
    mvprintw(height / 2 - 4, width / 2, "Don't Step On Snek");
    mvprintw(height / 2 - 3, width / 2, "------------------");
    mvprintw(height / 2 - 2, width / 2, "------------------");
    mvprintw(height / 2, width / 2, "Press P To Play");
    mvprintw(height / 2 + 2, width / 2, "Press I For Instructions");
    mvprintw(height / 2 + 4, width / 2, "Press C To Customize");
    mvprintw(height / 2 + 6, width / 2, "Press Q To Quit");
    mvprintw(height / 2 + 10, width / 2, "Created By Tunkie");
    
    int u = getch();
    
    switch (u)
    {
        case 'p':
            menu = false;
            break;
        case 'P':
            menu = false;
            break;
        case 'i':
            Instructions();
            break;
        case 'I':
            Instructions();
            break;
        case 'c':
            Customize();
            break;
        case 'C':
            Customize();
            break;
        case 'q':
            menu = false;
            gameOver = true;
            break;
        case 'Q':
            menu = false;
            gameOver = true;
            break;
    }
    refresh();
}

void Setup()
{
    initscr(); // INVESTIGATE
    clear();
    noecho();
    cbreak();
    curs_set(0);
    
    srand(time(NULL)); // set random seed based on the time
    
    gameOver = false;
    menu = true;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = 1 + (rand() % (width - 2));
    fruitY = 1 + (rand() % (height - 2));
    score = 0;
}

void Draw()
{
    clear(); // INVESTIGATE
    
    for (int i = 0; i < width + 2; i++) // make the top border
    {
        mvprintw(0, i, "#");
    }
    
    for (int i = 0; i < width + 2; i++) // make the bottom border
    {
        mvprintw(width + 1, i, "#");
    }
    
    for (int i = 1; i < height + 1; i++) // make left border
    {
        mvprintw(i, 0, "#");
    }
    
    for (int i = 1; i < height + 1; i++) // make right border
    {
        mvprintw(i, width + 1, "#");
    }
    
    for (int i = 1; i < height + 1; i++) // make the map
    {
        for (int j = 1; j < width + 1; j++)
        {
            if (i == x && j == y) // print the head
            {
                mvprintw(i, j, "O");
            }
            if (i == fruitX && j == fruitY) // print the fruit
            {
                mvprintw(i, j, "X");
            }

        }
    }
    
    if (score >= 1) // print the tail of the snek
    {
        for (int i = 0; i < score; i++)
        {
            mvprintw(x_hist[i], y_hist[i], "o");
        }
    }
    mvprintw(height + 3, 0, "Score: %d", score);
    
    refresh();
}

void Input()
{
    keypad(stdscr, TRUE); // INVESTIGATE
    halfdelay(1);
    
    int c = getch();
    
    switch (c)
    {
        case 'a':
            dir = LEFT;
            break;
        case 'A':
            dir = LEFT;
            break;
        case KEY_LEFT: // left arrow key
            dir = LEFT;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'S':
            dir = DOWN;
            break;
        case KEY_DOWN: // down arrow key
            dir = DOWN;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'D':
            dir = RIGHT;
            break;
        case KEY_RIGHT: // right arrow key
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 'W':
            dir = UP;
            break;
        case KEY_UP: // up arrow key
            dir = UP;
            break;
        case 'q':
            gameOver = true;
            break;
        case 'Q':
            gameOver = true;
            break;
    }
}

void Logic()
{
    for (int i = 0; i < x_hist.size(); i++) // can't hit your tail
    {
        if (x == x_hist[i] && y == y_hist[i])
        {
            gameOver = true;
        }
    }
    
    x_prev = x; // record current position (soon to be previous)
    y_prev = y;
    
    switch (dir)
    {
        case LEFT:
            y--;
            break;
        case RIGHT:
            y++;
            break;
        case UP:
            x--;
            break;
        case DOWN:
            x++;
        default:
            break;
    }
    
    if (x == 0 || y == 0 || x == height + 1 || y == width + 1) // end game if the head touches the border
    {
        gameOver = true;
    }
    
    if (x == fruitX && y == fruitY) // move fruit when eaten
    {
        fruitX = 1 + rand() % (width - 1); // need to make it so it appears within the board area
        fruitY = 1 + rand() % (height - 1);
        score++;
    }
    
    x_hist.resize(score);
    y_hist.resize(score);
    
    for (int i = 0; i < score; i++)
    {
        x_hist[score - i - 1] = x_hist[score - i - 2];
        y_hist[score - i - 1] = y_hist[score - i - 2];
    }
    
    x_hist[0] = x_prev;
    y_hist[0] = y_prev;
}

void game_over_animation() // game over animation
{
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            if (i % 2 != 0 && j % 2 != 0)
            {
               mvprintw(i, j, "+");
            }
            else
            {
                mvprintw(i, j, " ");
            }
            halfdelay(10);
        }
    }
}

int main()
{
    Setup(); // setup game
    
    while(menu) // stay in menu until user wants to play
    {
        Menu();
    }
    
    while(!gameOver) // loop when it is not gameover
    {
        Draw();
        Input();
        Logic();
    }
    
    game_over_animation(); // You should make this display for a set time
    //getch(); // INVESTIGATE
    endwin();
    cout << "You scored " << score << " points!" << endl;
    return 0;
}
