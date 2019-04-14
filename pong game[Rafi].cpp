#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
#define nl endl
enum directions
{
    STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4,
    UPRIGHT = 5, DOWNRIGHT = 6
};

class Ball
{
    int ball_x, ball_y;
    int init_x, init_y;
    directions ball_dir;

public:
    Ball(int x, int y)
    {
        init_x = x, init_y = y;
        ball_x = x, ball_y = y;
        ball_dir = STOP;
    }

    void ball_reset()
    {
        ball_dir = STOP;
        ball_x = init_x;
        ball_y = init_y;
    }

    void move_to(directions d)
    {
        ball_dir = d;
    }

    void ball_rand_dir()
    {
        ball_dir = (directions) ((rand() % 6) + 1);
    }

    directions cur_dir(){
        return ball_dir;
    }

    int cur_pos_x()
    {
        return ball_x;
    }

    int cur_pos_y()
    {
        return ball_y;
    }

    void move()
    {
        if(ball_dir == LEFT) ball_y--;
        else if(ball_dir == UPLEFT) ball_x--, ball_y--;
        else if(ball_dir == RIGHT) ball_y++;
        else if(ball_dir == UPRIGHT) ball_x--, ball_y++;
        else if(ball_dir == DOWNLEFT) ball_x++, ball_y--;
        else if(ball_dir == DOWNRIGHT) ball_x++, ball_y++;
    }


    void display()
    {
        cout << "(" << this -> ball_x << "," << this -> ball_y << ")" << " for dir " << this -> ball_dir << endl;
    }
};

class Paddle
{
    int pad_x, pad_y;
    int init_x, init_y;

public:
    Paddle(int x, int y)
    {
        init_x = x, init_y = y;
        pad_x = x, pad_y = y;
    }

    void paddle_reset()
    {
        pad_x = init_x, pad_y = init_y;
    }

    int cur_pos_x(){
        return pad_x;
    }

    int cur_pos_y(){
        return pad_y;
    }

    void move_up(){
        pad_x--;
    }

    void move_down(){
        pad_x++;
    }


};

class Controller
{
    Ball *b;
    Paddle *p1, *p2;
    int w, h;
    int score1, score2;
    char pd_up1, pd_dwn1, pd_up2, pd_dwn2;
    bool quit;

public:
    Controller(int hh, int ww)
    {
        w = ww, h = hh;
        quit = false;
        score1 = score2 = 0;
        pd_up1 = 'w', pd_up2 = 'i';
        pd_dwn1 = 's', pd_dwn2 = 'k';

        b = new Ball(h / 2, w / 2);
        p1 = new Paddle((h / 2) - 3, 3);
        p2 = new Paddle((h / 2) - 3, w);
    }

    ~Controller()
    {
        delete b, p1, p2;
    }

    void update_score(Paddle *player)
    {
        if(p1 == player) score1++;
        else score2++;

        b -> ball_reset();
        p1 -> paddle_reset();
        p2 -> paddle_reset();
    }

    void draw()
    {
        system("cls");
        /// upper wall
        for(int i = 1; i <= w + 2; i++)
            cout << "\xDB";
        cout << nl;

        for(int i = 2; i <= h - 1; i++) /// row
        {
            for(int j = 1; j <= w + 2; j++) /// col
            {
                int xball = b -> cur_pos_x();
                int yball = b -> cur_pos_y();

                int xp1 = p1 -> cur_pos_x();
                int yp1 = p1 -> cur_pos_y();

                int xp2 = p2 -> cur_pos_x();
                int yp2 = p2 -> cur_pos_y();

                if(j == 1 || j == w + 2) cout << "\xDB";
                else if(xball == i && yball == j) cout << "0";
                else if(yp1 == j && (xp1 == i || xp1 + 1 == i || xp1 + 2 == i || xp1 + 3 == i)) cout << "\xB2";
                else if(yp2 == j && (xp2 == i || xp2 + 1 == i || xp2 + 2 == i || xp2 + 3 == i)) cout << "\xB2";
                else cout << ' ';
            }

            cout << nl;
        }

        /// lower wall
        for(int i = 1; i <= w + 2; i++)
            cout << "\xDB";
        cout << nl << nl;
        cout << "Player1: " << score1 << "\tPlayer2: " << score2;

    }

    void input()
    {

        b -> move();
        int xball = b -> cur_pos_x();
        int yball = b -> cur_pos_y();

        int xp1 = p1 -> cur_pos_x();
        int yp1 = p1 -> cur_pos_y();

        int xp2 = p2 -> cur_pos_x();
        int yp2 = p2 -> cur_pos_y();

        if(_kbhit())
        {
            char inp = _getch();

            if(b -> cur_dir() == STOP) b -> ball_rand_dir();

            if(inp == pd_up1 && xp1 > 1) p1 -> move_up();
            else if(inp == pd_up2 && xp2 > 1) p2 -> move_up();

            else if(inp == pd_dwn1 && xp1 + 4 < h) p1 -> move_down();
            else if(inp == pd_dwn2 && xp2 + 4 < h) p2 -> move_down();

            else if(inp == 'q') quit = true;
        }
    }


    void logic()
    {
        int xball = b -> cur_pos_x();
        int yball = b -> cur_pos_y();

        int xp1 = p1 -> cur_pos_x();
        int yp1 = p1 -> cur_pos_y();

        int xp2 = p2 -> cur_pos_x();
        int yp2 = p2 -> cur_pos_y();

        /// left paddle
        for(int i = 0; i <= 3; i++)
        {
            if(xp1 + i == xball and yp1 + 1 == yball)
            {
                b -> move_to((directions)((rand() % 3) + 4));
            }
        }

        /// right paddle
        for(int i = 0; i <= 3; i++)
        {
            if(xp2 + i == xball and yp2 - 1 == yball)
            {
                b -> move_to((directions)((rand() % 3) + 1));
            }
        }

        /// top wall
        if(xball == 2)
            b -> move_to(b -> cur_dir() == UPLEFT ? DOWNLEFT : DOWNRIGHT);

        /// bottom wall
        if(xball == h - 1)
            b -> move_to(b -> cur_dir() == DOWNLEFT ? UPLEFT : UPRIGHT);

        /// left wall
        if(yball == 2)
            update_score(p2);

        /// right wall
        if(yball == w + 1)
            update_score(p1);

    }




    void run()
    {
        while(!quit)
        {
            draw();
            input();
            logic();
            Sleep(50);
        }
    }

};

int main()
{
    Controller c(25, 60);
    c.run();


}
