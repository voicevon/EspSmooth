#include "Actuator.h"

#include <math.h>

Actuator::Actuator()
{

}
Actuator::~Actuator(){

}


void Actuator::change_steps_per_mm(float new_steps)
{
    steps_per_mm = new_steps;
    last_milestone_steps = roundf(last_milestone_mm * steps_per_mm);
    current_position_steps = last_milestone_steps;
}

void Actuator::change_last_milestone(float new_milestone)
{
    last_milestone_mm = new_milestone;
    last_milestone_steps = roundf(last_milestone_mm * steps_per_mm);
    current_position_steps = last_milestone_steps;
}

void Actuator::set_last_milestones(float mm, int32_t steps)
{
    last_milestone_mm= mm;
    last_milestone_steps= steps;
    current_position_steps= last_milestone_steps;
}

void Actuator::update_last_milestones(float mm, int32_t steps)
{
    last_milestone_steps += steps;
    last_milestone_mm = mm;
}

int32_t Actuator::steps_to_target(float target)
{
    int32_t target_steps = roundf(target * steps_per_mm);
    return target_steps - last_milestone_steps;
}






#ifdef BOARD_PRIMEALPHA
// prime Alpha has TMC2660 drivers so this handles the setup of those drivers
#include "TMC26X.h"

bool Actuator::vmot= false;
bool Actuator::setup_tmc2660(ConfigReader& cr, const char *actuator_name)
{
    char axis= motor_id<3?'X'+motor_id:'A'+motor_id-3;
    printf("DEBUG: setting up tmc2660 for %s, axis %c\n", actuator_name, axis);
    tmc2660= new TMC26X(axis);
    if(!tmc2660->config(cr, actuator_name)) {
        delete tmc2660;
        return false;
    }
    tmc2660->init();

    return true;
}

bool Actuator::init_tmc2660()
{
    if(tmc2660 == nullptr) return false;
    tmc2660->init();
    return true;
}

bool Actuator::set_current(float c)
{
    if(tmc2660 == nullptr) return false;
    // send current to TMC2660
    tmc2660->setCurrent(c*1000.0F); // sets current in milliamps
    return true;
}

bool Actuator::set_microsteps(uint16_t ms)
{
    if(tmc2660 == nullptr) return false;
    tmc2660->setMicrosteps(ms); // sets microsteps
    return true;
}

int Actuator::get_microsteps()
{
    if(tmc2660 == nullptr) return 0;
    return tmc2660->getMicrosteps();
}

void Actuator::enable(bool state)
{
    if(tmc2660 == nullptr) {
        if(en_pin.connected()) {
            en_pin.set(!state);
        }
        return;
    }

    if(state && !vmot){
        //printf("WARNING: %d: trying to enable motors when vmotor is off\n", motor_id);
        if(is_enabled())
            tmc2660->setEnabled(false);
        return;
    }

    // if we have lost Vmotor since last time then we need to re load all the drivers configs
    if(state && vmot_lost) {
        if(vmot) {
            tmc2660->init();
            tmc2660->setEnabled(true);
            vmot_lost= false;
            printf("DEBUG: tmc2660: %d inited\n", motor_id);
        }else{
            tmc2660->setEnabled(false);
        }
        return;
    }

    // we don't want to enable/disable it if it is already in that state to avoid sending SPI all the time
    bool en= is_enabled();
    if((!en && state) || (en && !state)) {
        tmc2660->setEnabled(state);
    }
}

bool Actuator::is_enabled() const
{
    if(tmc2660 == nullptr) {
        if(en_pin.connected()) {
            return !en_pin.get();
        }
        // presume always enabled
        return true;
    }
    return tmc2660->isEnabled();
}

void Actuator::dump_status(OutputStream& os, bool flag)
{
    if(tmc2660 == nullptr) return;
    tmc2660->dump_status(os, flag);
}

void Actuator::set_raw_register(OutputStream& os, uint32_t reg, uint32_t val)
{
    if(tmc2660 == nullptr) return;
    tmc2660->set_raw_register(os, reg, val);
}

bool Actuator::set_options(GCode& gcode)
{
    if(tmc2660 == nullptr) return false;
    return tmc2660->set_options(gcode);
}

bool Actuator::check_driver_error()
{
    if(tmc2660 == nullptr) return false;
    return tmc2660->check_errors();
}

#else

//Minialpha has enable pins on the drivers



#endif
