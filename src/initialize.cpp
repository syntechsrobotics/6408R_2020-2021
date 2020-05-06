#include "main.h"

pros::Motor FL(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor FR(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor BL(3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor BR(4, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

// pros::Imu inertial(21);
// pros::Vision vision(20);
void initialize()
{
}

void disabled() {}

void competition_initialize()
{
    // Inertial.reset();
    // while(Inertial.is_calibrating){
    //     //indicate on screen
    //     pros::delay(20);
    // }
}
