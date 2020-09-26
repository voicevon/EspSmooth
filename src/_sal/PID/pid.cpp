
#include <iostream>
#include "pid.h"
#include "HardwareSerial.h"

using namespace std;

PID::PID() {
}
PID::~PID() {}

void PID::Init(int sideId, double kp, double ki, double kd) {
    __sideId = sideId;
    Kp = kp;
    Ki = ki;
    Kd = kd;

    p_error = 0;
    // i_error = 0;
    d_error = 0;
    for(int i=0; i< ERR_BUFFER_COUNT ; i++){
        i_err_buffer[i] = 0;
    }
    target_serial.begin(115200);
}

void PID::UpdateError(double cte) {
    double pre_cte = p_error;

    p_error  = cte;
    // i_error += cte;
    d_error  = cte - pre_cte;
    i_err_buffer[i_err_buffer_current_index] = cte;
    i_err_buffer_current_index++;
    if(i_err_buffer_current_index >= ERR_BUFFER_COUNT )
        i_err_buffer_current_index = 0;
    double sum = 0;
    for(int i=0; i<ERR_BUFFER_COUNT; i++){
        sum += i_err_buffer[i];
    }
    sum_error = sum;
}


/**
 * Compute the control command value according to PID controller
 * @return the steering angle
 */
double PID::get_output() {
    __last_output =  Kp * p_error + Ki * sum_error + Kd * d_error;
    return __last_output;
}

void PID::show_config(){
    Serial.println("[C] PID::show_config()");
    Serial.print("    (P，I, D) = (");
    Serial.print(Kp);
    Serial.print(",");
    Serial.print(Ki);
    Serial.print(",");
    Serial.print(Kd);
    Serial.println(")");

}

void PID::show_errors_and_output(){
    if(__sideId == 1){
        int16_t err = p_error;
        int16_t out = __last_output;
        target_serial.write(0xff);
        target_serial.write(0xff);
        target_serial.write(0xff);
        target_serial.write(err / 256);
        target_serial.write(err % 256);
        target_serial.write(0xff);
        target_serial.write(out / 256);
        target_serial.write(out % 256);
    }
    // targetSerial.println(__sideId);
    // targetSerial.println("[C] PID::show_errors()");
    // targetSerial.print("    (P_error，I_error, D_error, p_out,i_out,d_out) = (");
    // targetSerial.print(p_error);
    // targetSerial.print(",");
    // targetSerial.print(sum_error);
    // targetSerial.print(",");
    // targetSerial.print(d_error);
    // targetSerial.println(")  (");

    // targetSerial.print(Kp * p_error);
    // targetSerial.print(",");
    // targetSerial.print(Ki * sum_error);
    // targetSerial.print(",");
    // targetSerial.print(Kd * d_error);
    // targetSerial.println(")");
    // targetSerial.print("last_output = ");
    // targetSerial.println(__last_output);
    // targetSerial.print("last_output = ");
}

/**
 * Compute the throttle by PID controller
 * @param max_thro max throttle value
 * @return the computed throttle value
 */
double PID::OutputThrottle(double max_thro){

    return max_thro - Kp*p_error - Ki*sum_error - Kd*d_error;
}