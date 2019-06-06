/*
 * robot.h
 *
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#define PERIOD 60000
#define CW 6000
#define CCW 3000
#define STOP 4500

void init_servos();
void turn_right();
void turn_left();
void forward();
void backward();
void stop();

#endif /* ROBOT_H_ */
