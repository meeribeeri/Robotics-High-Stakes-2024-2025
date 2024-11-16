#include "main.h"



std::vector<int[]> readFile(const char* file_name) {
    //FILE* usd_auton_file = fopen(file_name,"r");
    //left motors, right motors, clamp, 
}

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


void left_turn() {
    left_drive.move(-90);
    right_drive.move(90);
    pros::delay(200);
    left_drive.move(0);
    right_drive.move(0);
}

void right_turn() {
    left_drive.move(90);
    right_drive.move(-90);
    pros::delay(200);
    left_drive.move(0);
    right_drive.move(0);
}

void clamp() {
    clamp_state = clamp_state ^ 0x1;
	clamp1.set_value(clamp_state);
	clamp2.set_value(clamp_state);
}

void auto1() {
    clamp();
    backward(1000);
    clamp();
    right_turn();
    intake_outtake.move(127*0.8);
    forward(500);
    pros::delay(750);
    intake_outtake.move(0);


}

void auto2() {
    clamp();
    backward(1000);
    clamp();
    left_turn();
    intake_outtake.move(127*0.8);
    forward(500);
    pros::delay(750);
    intake_outtake.move(0);
}

void auto3() {
    forward(500);
}

