#pragma once

#include "_hal/Pin/OutputPin.h"

class SigmaDeltaPwm : public OutputPin {
public:
    SigmaDeltaPwm();
    void on_tick(void);


    void     max_pwm(int);
    int      max_pwm(void);

    void     pwm(int);
    int      get_pwm() const { return _pwm; }
    // void     set(bool);
    
    void    as_output(){}
    inline void set(bool value) override;
    

private:
    int  _max;
    int  _pwm;
    int  _sd_accumulator;
    bool _sd_direction;
};