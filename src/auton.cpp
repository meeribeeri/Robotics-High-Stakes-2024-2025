#include "main.h"




void forward(int millis) {
    left_drive.move(-90);
    right_drive.move(-90);
    pros::delay(millis);
    left_drive.move(0);
    right_drive.move(0);
}

void backward(int millis) {
    left_drive.move(90);
    right_drive.move(90);
    pros::delay(millis);
    left_drive.move(0);
    right_drive.move(0);
}


void right_turn() {
    left_drive.move(-90);
    right_drive.move(90);
    pros::delay(200);
    left_drive.move(0);
    right_drive.move(0);
}

void right(int delay) {
    left_drive.move(-90);
    right_drive.move(90);
    pros::delay(delay);
    left_drive.move(0);
    right_drive.move(0);
}

void left(int delay) {
    left_drive.move(90);
    right_drive.move(-90);
    pros::delay(delay);
    left_drive.move(0);
    right_drive.move(0);
}

void left_turn() {
    left_drive.move(90);
    right_drive.move(-90);
    pros::delay(200);
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
	clamp2.set_value(clamp_state);
}

void auto1() { //right side
    clamp();
    backward(1000);
    clamp();
    right_turn();
    intake_outtake.move(127*0.8);
    forward(500);
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
