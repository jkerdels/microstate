
#include <cstdio>

#include "microstate.h"

/*

basic usage of microstate state machine:

1) define an enum class with at least two states, a start state and a return 
   state.

2) define your state machine using the functions "make_state_machine" and
   "make_state"

3) run the state machine by executing the "execute_step" function for each
   state transition. The execute_step function will return true, if the
   return state is reached. It will then automatically reset to the start
   state.

*/

// example:

enum class State {
	initial,
	counting,
	return_
};

int foo; // a global counting variable

auto state_machine = mst::make_state_machine(
	State::initial, // the start state
	State::return_, // the return state

	mst::make_state( 
		State::initial, // the state you want to define

		[]() -> State { // the state transition function
			foo = 0;
			return State::counting;
		}
	),

	mst::make_state( 
		State::counting, 

		[]() -> State { 
			if (++foo < 10)
				return State::counting;
			return State::return_;
		}
	)

);


int main() {

	// run the state machine until the return state is reached
	while (!state_machine.execute_step());

	std::printf("%d\n", foo);

	return 0;
}

