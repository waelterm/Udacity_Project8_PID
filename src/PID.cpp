#include "PID.h"
#include <iostream>

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {

	// Initializing Error
	p_error = 0;
	i_error = 0;
	d_error = 999999;

	// Initializing PID Coefficients
	Kp = Kp_;
	Ki = Ki_;
	Kd = Kd_;

}

void PID::UpdateError(double cte) {
	// At the first time step there is no d_error, because there is no previous error.
	if (d_error == 999999) {
		d_error = 0;
	}

	// Updating Integral Error
	i_error += cte;

	// Setting saturation of integral error to avoid integral windup. 
	if (i_error > 20)
		i_error = 20;
	if (i_error < -20)
		i_error = -20;

	// Updating Derivative Error
	if (cte != p_error)
		d_error = cte - p_error;

	// Updating Proportional Error
	p_error = cte;

	// Print Statements for Debugging
	//std::cout << "Integral Component: " << i_error*Ki << std::endl;
	//std::cout << "Differantial Component: " << d_error*Kd << std::endl;
	//std::cout << "Proportional Component " << p_error*Kp << std::endl;

}

double PID::TotalError() {
	// Calculating steering command
	double steering_output = Kp * p_error + Kd * d_error + Ki * i_error;
	return steering_output;
}

