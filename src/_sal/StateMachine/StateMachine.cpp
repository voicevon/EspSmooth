#include "StateMachine.h"


template<class T>
StateMachine<T>::StateMachine(T begin_state){
    __current_state = begin_state;
}


template <class T>
void StateMachine<T>::to_state(T new_state){
    if(is_allowed_to_enter_(new_state)){
        __current_state = new_state;
    }
}

