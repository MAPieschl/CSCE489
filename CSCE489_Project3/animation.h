#ifndef ANIMATION_H
#define ANIMATION_H

const int TERMINAL_WIDTH = 80;
const int TERMINAL_HEIGHT = 23;

class Animation
{

private:
	
public:
    // Instance variables
    int paddle_position;
    int paddle_width;
    char paddle_line[TERMINAL_WIDTH - 1];
    char ball_line[TERMINAL_WIDTH - 1];
    
    int ball_x;
    int ball_y;
    int ball_vx;
    int ball_vy;
    
    double start_time;
    double end_time;
    double time_warp;
    double time_warp_ave;
    double time_warp_buffer[100];

    // Constructor & destructor methods
    Animation();
    ~Animation();

    // Additional methods
    void clear_screen();
    void move_ball();
    void update_screen();

};

#endif
