# microstate
a micro footprint C++ state machine

This C++ state machine is a header-only "library" that is designed to ease the
programming of embedded devices. As a typical use case, it supports structuring
the executing of your code within a single main loop, e.g., you can define 
multiple state machines that each control certain aspects of your device's 
behavior as if you had multiple threads of execution running in parallel.

Usage of the state machine is pretty simple:

1) define your state labels using an enum class
2) define the state machine
3) execute the state machine step by step

Example:

```
#include "microstate.h"

enum class MyStates {
	initial,
	counting,
	return_
};

int foo;

auto my_state_machine = mst::make_state_machine(
	MyStates::initial,								// set start state
	MyStates::return_,								// set return state

	mst::make_state(								// define states
		MyStates::inital,
		[]() -> MyStates {
			foo = 0;
			return MyStates::counting
		}
	),

	mst::make_state(								
		MyStates::counting,
		[]() -> MyStates {
			if (++foo < 10)
				return MyStates::counting
			return MyStates::return_
		}
	)	
);


// ...

bool perform_counting = false;		// one way to activate a state machine
									// could be by using global indicators

// ...

{

	// somewhere in your main loop

	if ((perform_counting) && (my_state_machine.execute_step())) {
		perform_counting = false;
	}

}

```

Execute_step() returns true only if the return state is reached, otherwise
it will return false. The basic idea is to have one main state machine
that runs continuously (i.e., no state returns the return state) and
triggers the execution of smaller state machines for specific tasks via,
e.g., global indicators as used above. The smaller state machines do
reach the return state (after which they reset to the start state 
automatically).

