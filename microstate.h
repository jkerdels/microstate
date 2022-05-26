#ifndef MICROSTATE_H
#define MICROSTATE_H

#include <stdint.h>

namespace mst {

  template<typename E>
  using StateTransitionFunc = E (*) ();

  template<typename E>
  struct STF {
    E state_transition_id;
    StateTransitionFunc<E> state_transition_func;
  };

  template<typename E, typename F>
  auto make_state(E state_id, F func) -> STF<E> 
  {
    return STF<E>({state_id,func});
  }

  template<typename E, typename... StateTransitions>
  class StateMachine {

    static const uint8_t nr_of_states = sizeof...(StateTransitions);

    STF<E> state_transitions[nr_of_states];

    E start_id;
    E end_id;

    int8_t next_transition;

  public:

    StateMachine(E _start_id, E _end_id, StateTransitions... st) :
      state_transitions{static_cast<STF<E>>(st)...},
      start_id(_start_id),
      end_id(_end_id),
      next_transition(0)
    {
      next_transition = get_idx(start_id);
    }

    bool execute_step() {
      E next_id = state_transitions[next_transition].state_transition_func();
      if (next_id == end_id) {
        next_transition = get_idx(start_id);
        return true;
      }
      next_transition = get_idx(next_id);
      return false;
    }

  private:

    int8_t get_idx(E state_id) {
      for (uint8_t i = 0; i < nr_of_states; ++i){
        if (state_transitions[i].state_transition_id == state_id) {
          return (int8_t)i;
        }
      }
      return -1; 
    }
    
  };

  template<typename E, typename... StateTransitions>
  auto make_state_machine(E start_id, E end_id, StateTransitions... transitions) -> StateMachine<E,StateTransitions...> {
    return StateMachine<E,StateTransitions...>(start_id,end_id,transitions...);
  }

}

#endif
