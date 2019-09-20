#include "PID.h"
#include <iostream>
/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
	/**
	 * TODO: Initialize PID coefficients (and errors, if needed)
	 */
	p_error = 0;
	i_error = 0;
	d_error = 999999;
	/**
	 * PID Coefficients
	 */
	Kp = Kp_;
	Ki = Ki_;
	Kd = Kd_;

}

void PID::UpdateError(double cte) {
	if (d_error == 999999) {
		d_error = 0;
	}
	i_error += cte;
	// Anti-wind up saturation on integral error
	if (i_error > 20)
		i_error = 20;
	if (i_error < -20)
		i_error = -20;
	if (cte != p_error)
		d_error = cte - p_error;
	p_error = cte;
	//std::cout << "Integral Component: " << i_error * Ki << std::endl;
	//std::cout << "Differantial Component: " << d_error * Kd << std::endl;
	//std::cout << "Proportional Component " << p_error * Kp << std::endl;
}

double PID::TotalError() {
	double steering_output = Kp * p_error + Kd * d_error + Ki * i_error;
	return steering_output;  // TODO: Add your total error calc here!
}
