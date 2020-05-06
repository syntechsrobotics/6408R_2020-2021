#include "main.h"

// Defined in initialize.cpp
extern pros::Motor FR;
extern pros::Motor FL;
extern pros::Motor BR;
extern pros::Motor BL;

/*
	Maps from (-127) -> 127 to (-127) -> 127 using the function
	((127 * pow(4, ((abs(x) - 50) / 12.5))) / (pow(4, ((abs(x) - 50) / 12.5))+1))) * ((x > 0) - (x < 0))
	https://www.desmos.com/calculator/w7dktkaote
	Used to ease in/out joystick movement for more precise control.
	If the horizontal input is at 25%, it only has 5% of the power, reducing the
	amount that the robot will slowly veer off course from an imperfect control
	stick.
*/
int sigmoid_map[255] = {-127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -126, -126, -126, -126, -126, -126, -126, -126, -126, -126, -125, -125, -125, -125, -124, -124, -124, -123, -123, -123, -122, -122, -121, -120, -120, -119, -118, -117, -116, -115, -113, -112, -110, -109, -107, -105, -103, -100, -98, -95, -93, -90, -87, -84, -81, -77, -74, -71, -67, -63, -60, -56, -53, -50, -46, -43, -40, -37, -34, -32, -29, -27, -24, -22, -20, -18, -17, -15, -14, -12, -11, -10, -9, -8, -7, -7, -6, -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 7, 7, 8, 9, 10, 11, 12, 14, 15, 17, 18, 20, 22, 24, 27, 29, 32, 34, 37, 40, 43, 46, 50, 53, 56, 60, 64, 67, 71, 74, 77, 81, 84, 87, 90, 93, 95, 98, 100, 103, 105, 107, 109, 110, 112, 113, 115, 116, 117, 118, 119, 120, 120, 121, 122, 122, 123, 123, 123, 124, 124, 124, 125, 125, 125, 125, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};

void opcontrol()
{
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    int turn_input, strafe_input, throttle_input;
    int motor_speeds[4];
    while (true)
    {
        turn_input = sigmoid_map[master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)];
        strafe_input = sigmoid_map[master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)];
        throttle_input = sigmoid_map[master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)];

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