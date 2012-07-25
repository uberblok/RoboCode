#ifndef TAPE_FOLLOWER_HEADER_GUARD
#define TAPE_FOLLOWER_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <io_helper_functions.h>
#include <signal.h>
#include <observer.h>
#include <pins.h>

//TODO Try out a self-adjusting QRD threshold setter by spinning around in a circle, recording minimum, maximum, and average values

enum TAPEFOLLOWING_CONSTANTS 
{
  initialSpeed = 200,
  initialQRDThresholdL = 200,
  initialQRDThresholdR = 200,
  initialProportionalGain = 110,
  initialDerivGain = 60,
  initialMaxError = 3,
  SHARP_TURN_SPEED = 250
};

enum TurningBias 
{
	LEFT,
	RIGHT,
	NONE
};

class TapeFollower
{
public:
  TapeFollower()
  : kP(initialProportionalGain),
    kD(initialDerivGain),
    baseSpeed(initialSpeed),
	leftQRD(LEFT_TAPE_QRD, initialQRDThresholdL),
	rightQRD(RIGHT_TAPE_QRD, initialQRDThresholdR),
	leftOutboardQRD(LEFT_OUTBOARD_QRD, initialQRDThresholdL),
	rightOutboardQRD(RIGHT_OUTBOARD_QRD, initialQRDThresholdR),
    time(0),
    lastTime(0),
    error(0),
    lastError(0),
	lastDifferentError(0),
    count(0),
    leftMotorSpeed(0),
    rightMotorSpeed(0)
    {
		
    }
	
	void stop()
	{
		motor.speed(LEFT_DRIVE_MOTOR,0);
		motor.speed(RIGHT_DRIVE_MOTOR,0);
	}
	
	void turnAround()
	{
		while(leftOutboardQRD.belowThreshold()) //Turn left until the left outboard QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, -baseSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
			leftOutboardQRD.read();
		}
		while(leftQRD.belowThreshold()) //Turn left slower until the left QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, -0.5*baseSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, 0.5*baseSpeed);
			leftQRD.read();
		}
		while(leftQRD.aboveThreshold()) //Inch left until the left QRD is just off the tape (and the robot is straight)
		{
			motor.speed(LEFT_DRIVE_MOTOR, -0.1*baseSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, 0.1*baseSpeed);
			leftQRD.read();
		}
	}
	
	void enable() //Enable tape following by adding the sensors to the observer
	{
		OBSERVER.addSignal(&leftQRD);
		OBSERVER.addSignal(&rightQRD);
		OBSERVER.addSignal(&leftOutboardQRD);
		OBSERVER.addSignal(&rightOutboardQRD);
	}
	
	void disable() //Disable tape following by removing the sensors from the observer
	{
		stop();
		OBSERVER.removeSignal(&leftQRD);
		OBSERVER.removeSignal(&rightQRD);
		OBSERVER.removeSignal(&leftOutboardQRD);
		OBSERVER.removeSignal(&rightOutboardQRD);
	}
	
	void makeHardLeft()
	{
		//TODO
		while(leftQRD.belowThreshold())
		{
			motor.speed(LEFT_DRIVE_MOTOR, -SHARP_TURN_SPEED);
			motor.speed(RIGHT_DRIVE_MOTOR, SHARP_TURN_SPEED);
			leftQRD.read();
		}
		
		motor.speed(LEFT_DRIVE_MOTOR, baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
	}
	
	void makeHardRight()
	{
		while(rightQRD.aboveThreshold())
		{
			motor.speed(LEFT_DRIVE_MOTOR, SHARP_TURN_SPEED);
			motor.speed(RIGHT_DRIVE_MOTOR, -SHARP_TURN_SPEED);
			rightQRD.read();
		}
		
		motor.speed(LEFT_DRIVE_MOTOR, baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
	}
	
  boolean followTape()
  {
    //Error function
    if(leftQRD.aboveThreshold() && rightQRD.aboveThreshold()) error = 0; //Both on tape
    else if(leftQRD.belowThreshold() && rightQRD.aboveThreshold()) error = 1; //Left off tape, turn right
    else if(leftQRD.aboveThreshold() && rightQRD.belowThreshold()) error = -1; //Right off tape, turn left
    else if(leftQRD.belowThreshold() && rightQRD.belowThreshold()) //Both off tape -- use history
    {
        if(lastError>0) 
			error = 3;
        else 
			error = -3;
    }

    //Derivative estimation
    if(error != lastError)
    {
		lastDifferentError = lastError;
		lastTime = time;
		time = 1;
    }
	else
	{
		++time;
	}
	
    int correction = kP * error + kD * (error - lastDifferentError) / ((float)(time + lastTime));
    
	if(correction > 0) //turn right
	{
		leftMotorSpeed = baseSpeed;
		rightMotorSpeed = baseSpeed-correction;
	}
	else // correction < 0 turn left
	{
		leftMotorSpeed = baseSpeed+correction;
		rightMotorSpeed = baseSpeed;
	}

    motor.speed(LEFT_DRIVE_MOTOR, leftMotorSpeed);
    motor.speed(RIGHT_DRIVE_MOTOR, rightMotorSpeed);
    lastError = error;
    ++count;

    if(count == 100)
    {
      display();
      count = 0;
    }
    
    return true;
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print("L:" + String(leftMotorSpeed) + " ");
    LCD.print("R:" + String(rightMotorSpeed));
    LCD.setCursor(0,1);
    LCD.print("L:" + String(leftQRD.reading) + " ");
    LCD.print("R:" + String(rightQRD.reading));
  }
  
  AnalogSignal leftQRD;
  AnalogSignal rightQRD;
  AnalogSignal leftOutboardQRD;
  AnalogSignal rightOutboardQRD;

  unsigned long time;
  unsigned long lastTime;
  char error;
  char lastError;
  char lastDifferentError;
  char count;
  
  int leftMotorSpeed;
  int rightMotorSpeed;
  
  int QRDThresholdL;
  int QRDThresholdR;
  int kP;
  int kD;
  int baseSpeed;
};

extern TapeFollower TAPEFOLLOWER;

#endif

