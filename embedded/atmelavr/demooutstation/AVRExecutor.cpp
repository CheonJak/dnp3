
#include "AVRExecutor.h"

#include "AVRTimer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include <CriticalSection.h>

using namespace openpal;

AVRExecutor* gpExecutor = nullptr;

SIGNAL(TIMER1_COMPA_vect)
{
	if(gpExecutor)
	{
		gpExecutor->Tick();		
	}	
}

void AVRExecutor::Tick()
{
	++ticks;	
}

void AVRExecutor::Sleep()
{
	set_sleep_mode(SLEEP_MODE_IDLE);
  
	sleep_enable();
	
	// things we might disable
	/*
	power_adc_disable();
	power_spi_disable();
	power_timer0_disable();
	power_timer2_disable();
	power_twi_disable(); 
	*/

	/* Now enter sleep mode. */
	sleep_mode();
  
	/* The program will continue from here after the timer timeout*/
	sleep_disable(); /* First thing to do is disable sleep. */
  
	/* Re-enable the peripherals. */
	// power_all_enable();
}


void AVRExecutor::Init()
{		
	 // Configure timer 1 for CTC mode
	TCCR1B |= (1 << WGM12);

	// Set CTC compare value to ~100Hz at 16MHz AVR clock, with prescale of 1024
	OCR1A = 155;
	
	// Start timer at FCPU / 1024
	TCCR1B |= ((1 << CS10) | (1 << CS12));

	TIMSK1 |= (1 << OCIE1A); // enable output compare interrupt for A compare
	
	gpExecutor = this;		
}

AVRExecutor::AVRExecutor() : ticks(0)
{	
	for(uint8_t i = 0; i < timers.Size(); ++i)
	{
		idleTimers.Enqueue(&timers[i]);
	}
}

MonotonicTimestamp AVRExecutor::GetTime()
{	
	// disable interrupts to ensure atomic access to the 'ticks' variable
	CriticalSection cs;
	return MonotonicTimestamp(ticks*10); // every tick represents 10 milliseconds since Init()				
}

ITimer* AVRExecutor::Start(const TimeDuration& duration, const Action0& action)
{	
	return Start(GetTime().Add(duration), action);
}
	
ITimer* AVRExecutor::Start(const MonotonicTimestamp& ts, const Action0& action)
{
	assert(idleTimers.IsNotEmpty());
	AVRTimer** pTimer = idleTimers.Pop();
	(*pTimer)->Set(this, action, ts);
	this->activeTimers.Add(*pTimer);
	return *pTimer;
}
	
void AVRExecutor::Post(const Action0& action)
{	
	assert(!work.IsFull());
	work.Enqueue(action);
}

void AVRExecutor::Run()
{
	while(RunOne());
}

bool AVRExecutor::RunOne()
{				
	if(RunOneTimer())
	{
		return true;
	}
	else
	{
		if(work.IsNotEmpty())
		{
			work.Peek()->Apply();
			work.Pop();
			return true;
		}
		else
		{
			return false;
		}
	}	
}

bool AVRExecutor::RunOneTimer()
{	
	MonotonicTimestamp time = GetTime();
	auto expired = [time](AVRTimer* pTimer) { return pTimer->ExpiresAt().milliseconds < time.milliseconds; };
	auto pNode = activeTimers.RemoveFirst(expired);
	if(pNode)
	{
		idleTimers.Enqueue(pNode->value);
		
		// make a copy of the task and run it
		Action0 copy(pNode->value->action);
		copy.Apply();		
		return true;
	}
	else
	{
		return false;
	}
}
			
void AVRExecutor::OnCancel(AVRTimer* pTimer)
{
	auto matches = [pTimer](AVRTimer* pItem){ return pTimer == pItem; };
	activeTimers.RemoveFirst(matches);	
	idleTimers.Enqueue(pTimer);
}
