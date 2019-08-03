#pragma once



class MotorPidController{

    public:

        MotorPidController(){};
        float get_output_value(float target_value,float sensor_value);

        
    private:


};