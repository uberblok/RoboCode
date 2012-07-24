#include "WProgram.h"
#include <state_machine.h>

State TravelToDepot 			= State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
State TravelFromDepot			= State(travelFromDepot_Enter, travelFromDepot_Update, travelFromDepot_Exit);
State Error_TapeLost			= State(errorHandling_TapeLost_Enter, errorHandling_TapeLost_Update, errorHandling_TapeLost_Exit);
State Idle						= State(idle_Update);

FSM robotStateMachine(TravelToDepot);
StateHistory STATE_HISTORY(robotStateMachine);

void travelToDepot_Enter()
{
	TAPEFOLLOWER.enable();
}

void travelToDepot_Exit()
{
	TAPEFOLLOWER.disable();
}

void travelToDepot_Update()
{
	//if(!digitalRead(BUMPER))
	//{
		if(TAPEFOLLOWER.followTapeRightBiased())
                  return;
                else 
                  robotStateMachine.transitionTo(Error_TapeLost);
	//}
	//else
	//{
	//	TAPEFOLLOWER.stop();
	//	TAPEFOLLOWER.backUp();
	//	TAPEFOLLOWER.turnAround();
	//	robotStateMachine.transitionTo(TravelFromDepot);
	//}
}

void travelFromDepot_Enter()
{
	TAPEFOLLOWER.enable();
}

void travelFromDepot_Exit()
{
	TAPEFOLLOWER.disable();
}

void travelFromDepot_Update()
{
	if(digitalRead(LEFT_BUMPER) || digitalRead(RIGHT_BUMPER)) //Go until both sensors hit tape
	{
		TAPEFOLLOWER.stop();
		robotStateMachine.transitionTo(FindBlock);
	}
	else //Stop
	{
		TAPEFOLLOWER.followTapeRightBiased();
	}
}

void errorHandling_TapeLost_Enter()
{
	//TODO
	TAPEFOLLOWER.stop();
	
	LCD.clear();
	LCD.home();
	LCD.print("---ERROR---");
	LCD.setCursor(0,1);
	LCD.print("---TAPE LOST---");
	
	while(!readStart())
	{
		delay(100);
	}	
	STATE_HISTORY.rollback();
}

void findBlock_Enter()
{
	GRIPPER.enable();
}

void findBlock_Update()
{
	GRIPPER.close();
	if(GRIPPER.closed)
	{
		if(!GRIPPER.switchesClosed()) //back up, turn around, try again
		{
			GRIPPER.open();
			backUp(500);
			turnRight(45);
			backUp(500);
			turnLeft(45);
			forwardToDepot();
		}
		else //got a block
		{
			backUp(500);
			turnLeft(90);
			forwardToTape();
		}
	}
}

void findBlock_Exit()
{
	GRIPPER.disable();
}

void dropBlock_Enter()
{

}

void dropBlock_Update()
{

}

void dropBlock_Exit()
{
}