#include "WProgram.h"
#include "TimedAction.h"

/*
|| @constructor
|| | Initialize the TimedAction object and activate it
|| #
||
|| @parameter intervl  the interval to call the function
|| @parameter function the funtion to call each interval
*/
TimedAction::TimedAction(unsigned long intervl, void (*function)())
{
  active = true;
  previous = 0;
  interval = intervl;
  execute = function;
}

/*
|| @constructor
|| | Initialize the TimedAction object and activate it
|| #
||
|| @parameter prev     the wait time, before starting the interval
|| @parameter intervl  the interval to call the function
|| @parameter function the funtion to call each interval
*/
TimedAction::TimedAction(unsigned long prev, unsigned long intervl, void (*function)())
{
  active = true;
  previous = prev;
  interval = intervl;
  execute = function;
}

/*
|| @description
|| | Reset the interval timing
|| #
*/
void TimedAction::reset()
{
  previous = millis();
}

/*
|| @description
|| | Disable the calling of this TimedAction
|| #
*/
void TimedAction::disable()
{
  active = false;
}

/*
|| @description
|| | Enable the calling of this TimedAction
|| #
*/
void TimedAction::enable()
{
  active = true;
}

/*
|| @description
|| | Check if it is time for this TimedAction to call the function
|| #
*/
void TimedAction::check()
{
  if (active && (millis() - previous >= interval))
  {
    previous = millis();
    execute();
  }
}

/*
|| @description
|| | Set the interval of this TimedAction
|| #
||
|| @parameter intervl the interval to call the function
*/
void TimedAction::setInterval(unsigned long intervl)
{
  interval = intervl;
}

