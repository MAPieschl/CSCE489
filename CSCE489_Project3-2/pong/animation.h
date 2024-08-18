#ifndef ANIMATION_H
#define ANIMATION_H

#define TIME_CONSTANT 100000
#define USEC_SCALER 1000000
#define NSEC_SCALER 1000000000
#define REFRESH_PERIOD_S 0.1

const int TERMINAL_WIDTH = 80;
const int TERMINAL_HEIGHT = 23;

class Animation
{

private:
	
public:
    // Instance variables
    bool start_game;
    bool run_game;
    int next_paddle_position;
    int paddle_position;
    int paddle_width;
    char paddle_line[TERMINAL_WIDTH - 1];
    char ball_line[TERMINAL_WIDTH - 1];
    
    int ball_x;
    int ball_y;
    int ball_vx;
    int ball_vy;
    
    struct timespec ts;
    double last_move;
    double this_move;

    // Constructor & destructor methods
    Animation();
    ~Animation();

    // Additional methods
    void clear_screen();
    bool move_ball();
    void reset_ball();
    void update_screen();
    
};

#endif
