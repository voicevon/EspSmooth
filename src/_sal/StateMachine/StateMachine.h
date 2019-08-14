#pragma once


template <typename T>
class StateMachine{
    
    public:
        StateMachine(T begin_state);
        // void to_state(T new_state);
    
    protected:
        // virtual bool is_allowed_to_enter_(T new_state) = 0;
        
    private:
        // T __current_state;

};

