#include "main.h"

//60 spins 36 gear ratio (motor : wheel)
double circumference = 25.9338474;
//wheel dist center to center: 12.6 in
//center: 6.3 in
double target_right = 0;
double target_left = 0;

double in_to_cm(double inches) {
    return inches * 2.54;
}

double cm_to_wheel_degrees(double cm) {
    //angle/360 * diameter * pi = arc
    //arc/(diam * pi) * 360
    return (cm/circumference) * 360;
}

double wheel_degrees_to_motor(double degrees) {
    //36 / 60
    return (60.f / 36.f) * degrees;
}

double dist_convert(double inches) {
    return (60/36) * ((((inches - 17.5) * 2.54) / circumference) * 360);
}

double turn_degrees_motor(double angle) {
    return wheel_degrees_to_motor(cm_to_wheel_degrees((100.543531 * (angle/360))));
}

void delay_motors_till_target() {
    while (!(left_drive.get_position() < target_left + 5 && left_drive.get_position() > target_left - 5) && !(right_drive.get_position() < target_right + 5 && right_drive.get_position() > target_right - 5)) {
        pros::delay(5);
    }
}

void forward_relative(double degrees) {
    left_drive.move_relative(degrees, INT_MAX);
    right_drive.move_relative(degrees, INT_MAX);
    target_left = target_left + degrees;    
    target_right = target_right + degrees;
    delay_motors_till_target();
}

void turn_stationary(double angle) {
    double base_angle = turn_degrees_motor(angle);
    left_drive.move_relative(-base_angle, INT_MAX);
    right_drive.move_relative(base_angle, INT_MAX);
    target_left = target_left - base_angle;
    target_right = target_right + base_angle;
    delay_motors_till_target();
}

void turn_relative(double degrees_right, double degrees_left) {
    left_drive.move_relative(degrees_left, INT_MAX);
    right_drive.move_relative(degrees_right, INT_MAX);
    target_left = target_left + degrees_left;
    target_right = target_right + degrees_right;
    delay_motors_till_target();
}


void forward(int millis) {
    left_drive.move(90);
    right_drive.move(90);
    pros::delay(millis);
    left_drive.move(0);
    right_drive.move(0);
}

void backward(int millis) {
    left_drive.move(-90);
    right_drive.move(-90);
    pros::delay(millis);
    left_drive.move(0);
    right_drive.move(0);
}


void right_turn() {
    left_drive.move(90);
    right_drive.move(-90);
    pros::delay(150);
    left_drive.move(0);
    right_drive.move(0);
}

void right(int delay) {
    left_drive.move(90);
    right_drive.move(-90);
    pros::delay(delay);
    left_drive.move(0);
    right_drive.move(0);
}

void left(int delay) {
    left_drive.move(-90);
    right_drive.move(90);
    pros::delay(delay);
    left_drive.move(0);
    right_drive.move(0);
}

void left_turn() {
    left_drive.move(-90);
    right_drive.move(-90);
    pros::delay(150);
    left_drive.move(0);
    right_drive.move(0);
}

//gear ratio: 3 turns for 5 nof drive
void left_turn(int degrees) {
    left_drive.move_relative(degrees,90);
    left_drive.move_relative(-degrees,90);
}

void right_turn(int degrees) {
    left_drive.move_relative(-degrees,90);
    left_drive.move_relative(degrees,90);
}

void straight(int degrees) {
    left_drive.move_relative(degrees,90);
    left_drive.move_relative(degrees,90);
}

void clamp() {
    clamp_state = clamp_state ^ 0x1;
	clamp1.set_value(clamp_state);
}

void flag_change() {
    flag_state = flag_state ^ 0x1;
    flag.set_value(flag_state);
}

void auto1() { //right side
    clamp();
    backward(1000);
    clamp();
    intake_outtake.move(127*0.8);
    pros::delay(750);
    intake_outtake.move(0);


}

void auto2() { //left side
    clamp();
    backward(1000);
    clamp();
    left_turn();
    intake_outtake.move(127*0.8);
    forward(500);
    pros::delay(750);
    intake_outtake.move(0);
}

void auto3() { //fuck if I know
    clamp();
    backward(1000);
    clamp();
    intake_outtake.move(127*0.8);
    pros::delay(750);
}

void auto5() { //right side unknown working
    clamp();
    backward(1000);
    clamp();
    right(250);
    intake_outtake.move(127*0.8);
    forward(750);
    pros::delay(750);
    intake_outtake.move(0);
    right_turn();
    backward(1000);
}

void auto6() { //left side unknown working
    clamp();
    backward(1000);
    clamp();
    left(250);
    intake_outtake.move(127*0.8);
    forward(7500);
    pros::delay(750);
    intake_outtake.move(0);
    left_turn();
    backward(1000);
}

void shitton() {

    clamp();
    backward(1000);
    clamp();
    right(300);
    intake_outtake.move(127*0.8);
    forward(1500);
    pros::delay(750);
    left(250);
    forward(1000);
    pros::delay(200);
    intake_outtake.move(0);
    right(300);
    backward(100);
}
//diagonal: 33.31887
void skills() {
    forward_relative(dist_convert(23.08));
    clamp();
    pros::delay(250);
    intake_outtake.move(127*0.8);
    turn_stationary(90);
    forward_relative(dist_convert(46.64 - 23.08));
    forward_relative(dist_convert(-23.56));
    turn_stationary(-135);
    forward_relative(dist_convert(-(sqrt(pow(23.08-11.30,2)*2))));
    turn_stationary(-135);
    forward_relative(dist_convert(93.77-11.30));
    turn_stationary(180 + atan((23.08-11.30)/(93.77-70.20)));
    forward_relative(dist_convert(sqrt(pow(93.77 - 70.20,2) + pow(23.08 - 11.30,2))));
    turn_stationary((90 - atan((23.08-11.30)/(93.77-70.20))) + 90 + atan(11.30/70.20));
    forward_relative(dist_convert(sqrt(pow(70.20,2) + pow(11.30,2))));
    clamp();
    forward_relative(-dist_convert(1));
    turn_stationary(-90 - atan(11.30/70.20) + atan(3.50/(70.20-4.0)));
    forward(dist_convert(sqrt(pow(3.50,2) + pow(70.20 - 4.0,2))));
}

void skills_simple() {
    high_score.move(-127);
}