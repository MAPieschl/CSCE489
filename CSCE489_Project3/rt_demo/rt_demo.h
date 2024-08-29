#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_PID_LEN 5

const char control_path[] = {"../temp/ctl_fifo"};
const char gnome_path[] = {"../temp/gnome_fifo"};

const char *pong_terminals[] = {"gnome-terminal --title Pong -- ./../pong/pong",
						"gnome-terminal --title Controller -- ./../pong_controller/pong_controller"};			
const int num_pong_terminals = 2;

const char *monitor_terminals[] = {"gnome-terminal --title Monitor -- top"};				
const int num_monitor_terminals = 1;

const char *stress_terminals[] = {"gnome-terminal --title Stressor -- ./../stressor/stressor 10",
								"gnome-terminal --title Stressor -- ./../stressor/stressor 1"};	
const int num_stress_terminals = 2;
const int num_child_stressors = 1;
const int num_sibling_stressors = 10;

#endif
