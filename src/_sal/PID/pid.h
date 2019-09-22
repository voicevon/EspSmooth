
#ifndef PID_H
#define PID_H

class PID {
    public:
        double p_error;
        double i_error;
        double d_error;

        double Kp;
        double Ki;
        double Kd;

        PID();
        virtual ~PID();

        void Init(double kp, double ki, double kd);

        void UpdateError(double cte);

        /**
         * Compute the control command value according to PID controller
         * @return the steering angle
         */
        double OutputSteerAng();

        /**
         * Compute the throttle by PID controller
         * @param max_thro max throttle value
         * @return the computed throttle value
         */
        double OutputThrottle(double max_thro);
};

#endif /* PID_H */