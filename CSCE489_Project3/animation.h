#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
public:
    // Instance variables
    int paddle_position;
    int paddle_width;

    // Constructor & destructor methods
    Animation();
    ~Animation();

    // Additional methods
    void clear_screen();
    void update_screen();

private:

};

#endif
