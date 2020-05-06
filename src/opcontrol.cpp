#include "main.h"

// Defined in initialize.cpp
extern pros::Motor FR;
extern pros::Motor FL;
extern pros::Motor BR;
extern pros::Motor BL;

void opcontrol()
{
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    int turn_input, strafe_input, throttle_input;
    int motor_speeds[4];
    while (true)
    {
        turn_input = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        strafe_input = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
        throttle_input = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

        // X-drive is wacky
        motor_speeds[0] = throttle_input - strafe_input - turn_input;
        motor_speeds[1] = throttle_input + strafe_input + turn_input;
        motor_speeds[2] = throttle_input + strafe_input - turn_input;
        motor_speeds[3] = throttle_input - strafe_input + turn_input;

        // Limit speeds if they exceed the maximum
        int max_speed = 0;
        for (int i = 0; i < 3; i++)
        {
            if (std::abs(motor_speeds[i]) > max_speed)
            {
                max_speed = std::abs(motor_speeds[i]);
            }
        }

        if (max_speed > 127)
        {
            float speed_ratio = max_speed / 127;
            for (int i = 0; i < 3; i++)
            {
                motor_speeds[i] /= speed_ratio;
            }
        }

        FR.move_velocity(motor_speeds[0]);
        FL.move_velocity(motor_speeds[1]);
        BR.move_velocity(motor_speeds[2]);
        BL.move_velocity(motor_speeds[3]);

        pros::delay(20);
    }
}