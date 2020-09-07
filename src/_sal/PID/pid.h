
#ifndef PID_H
#define PID_H

# define ERR_BUFFER_COUNT 300
class PID {
    public:

        double Kp;
        double Ki;
        double Kd;

        PID();
        virtual ~PID();

        ///
        /// sidId: 1=left side, 2=right side
        ///
        void Init(int sideId,double kp, double ki, double kd);

        void UpdateError(double cte);

        void show_config();
        void show_errors_and_output();

        /**
         * Compute the control command value according to PID controller
         * @return the steering angle
         */
        double get_output();

        /**
         * Compute the throttle by PID controller
         * @param max_thro max throttle value
         * @return the computed throttle value
         */
        double OutputThrottle(double max_thro);
    private:
        int __sideId;
        float __last_output = 0;
        double p_error;
        // double i_error;
        double d_error;
        double sum_error;
        double i_err_buffer[ERR_BUFFER_COUNT];   //
        int i_err_buffer_current_index = 0;
};

#endif /* PID_H */