#include "systemc.h"
#include <string.h>
#include <ncurses.h>

struct cell : sc_module {

    bool alive, firstloop;
    
    sc_in_clk clk;
    sc_in<bool> neighbors_alive[8];
    sc_out<bool> alive_out;
    sc_signal<bool> alive_signal;
    
    void update ()
    {
        if (firstloop)
        {
            firstloop = false;
        }
        else
        {
            int nalive = 0;
            for (int i = 0; i < 8; i++)
            {
                if (neighbors_alive[i].read ())
                    nalive++;
            }
            
            if (nalive < 2 || nalive > 3)
                alive = false; // death conditions
            else if (nalive == 3)
                alive = true; // birth conditions
            // otherwise, don't change anything
        }
        alive_out.write (alive);
    }
    
    void thread_main ()
    {
        while (true)
        {
            update ();
            wait ();
        }
    }
    
    SC_CTOR (cell)
    {
        firstloop = true;
        alive = false;
        alive_out (alive_signal);
#ifdef THREAD_PROCESSES
        SC_THREAD (thread_main);
#else
        SC_METHOD (update);
#endif
        sensitive << clk.pos ();
    }

};

static const struct
{
    int xoffs, yoffs;
}
neighbor_offsets[8] = 
{
    {-1, -1},   {0, -1},    {1, -1},
    {-1, 0},                {1, 0},
    {-1, 1},    {0, 1},     {1, 1}
};

static const int starter_pattern[21][36] = 
{
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1},
{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},

{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
};

int sc_main (int argc, char *argv[])
{
    int VGRID = 128, HGRID = 128;
    int TIME = 1024;
    cell *grid[VGRID][HGRID];
    
    sc_clock clock ("mainclock");
    
    char name[64] = "cell_";
    
    // sigh
    for (int y = 0; y < VGRID; y++)
    {
        for (int x = 0; x < HGRID; x++)
        {
            sprintf (name+5, "%d_%d", x, y);
            
            grid[y][x] = new cell (name);
            
            grid[y][x]->clk (clock);
        }
    }
    
    for (int y = 0; y < 21; y++)
    {
        for (int x = 0; x < 36; x++)
        {
            if (starter_pattern[y][x])
                grid[y][x]->alive = true;
        }
    }
    
    for (int y = 0; y < VGRID; y++)
    {
        for (int x = 0; x < HGRID; x++)
        {
            for (int i = 0; i < 8; i++)
            {
                int x2 = (x + neighbor_offsets[i].xoffs + HGRID) % HGRID,
                    y2 = (y + neighbor_offsets[i].yoffs + VGRID) % VGRID;
                grid[y][x]->neighbors_alive[i] (grid[y2][x2]->alive_signal);
            }
        }
    }
    
//#define INTERACTIVE
#ifdef INTERACTIVE
    initscr ();
    
    for (int i = 0; i < TIME; i++)
    {
        sc_start (1, SC_NS);
        
        int limy, limx;
        getmaxyx (stdscr, limy, limx);
        for (int y = 0; y < limy && y < VGRID; y++)
        {
            for (int x = 0; x < limx && x < HGRID; x++)
                mvaddch (y, x, grid[y][x]->alive ? '*' : ' ');
        }
        
        refresh ();
    }
    
    endwin ();
#else
    printf ("initialization done\n");
    sc_start (TIME, SC_NS);
#endif
    
    return 0;
}
