#ifndef PINS_HEADER_GUARD
#define PINS_HEADER_GUARD

enum ANALOG_IN
{
	LEFT_TAPE_QRD = 0,
	RIGHT_TAPE_QRD = 1,
	LEFT_RANGEFINDER = 2,
	RIGHT_RANGEFINDER = 3,
	LEFT_OUTBOARD_QRD = 4,
	RIGHT_OUTBOARD_QRD = 5
};

enum MOTORS
{
	LEFT_DRIVE_MOTOR = 0,
	RIGHT_DRIVE_MOTOR = 1,
    GRIPPER_MOTOR = 2,
    LIFTER_MOTOR = 3
};

enum DIGITAL_IN
{
    GRIPPER_RIGHT_SWITCH = 1,
    RIGHT_BUMPER = 0,
    LEFT_BUMPER = 15,
    CENTRE_BUMPER = 7,
    GRIPPER_CENTRE_SWITCH = 8,
    SLIDE_TOP_SWITCH = 11,
    GRIPPER_LEFT_SWITCH = 14,
    SLIDE_BOTTOM_SWITCH = 9
};

enum MOTOR_SPEEDS
{
        MAX_MOTOR_SPEED = 1023,
	FAST_MOTOR_SPEED = 512,
	MEDIUM_MOTOR_SPEED = 300,
	SLOW_MOTOR_SPEED = 250
};

#endif
