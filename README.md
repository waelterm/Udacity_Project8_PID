CarND-Controls-PID
---
This repository include my solution to the Self-Driving Car Engineer Nanodegree Program from Udacity

---


## Controller Description

### Proportional Control

A proportional controller, calculates an output in direct proportion to the error between the reference signal and the real measurement:
  
  `
control_output = -Kp * error
`  

This leads to a controller that oscillates. The higher the proportional gain Kp i, the higher the frequency of the oscillation will be.
The oscillations usually increase with time making the controller unstable. This can be seen in this [video](./Videos/P-Controller.mp4)

### Derivative Control

A derivative controller, calculates an output in direct proportion to rate of change of the error.
The rate of change can be discretized by using the previous error and the current error and dividing by the time difference between the measurements.
  
`
error_rate_of_change = (current_error - previous_error)/time_difference `
   
`control_output = - Kd * error_rate_of_change`  

This leads to a controller that responds only to the rate of the error. 
Due to it not using any information about the reference signal, it will drift away from the center due to the delay between measurement and actuation.
This can be seen in the following video: [video](./Videos/D-Controller.mp4)

### Integral Control

A integral controller, calculates an output in direct proportion to the integral of errors over time.
The integral can be discretized by using summing up the product of errors and time-steps. 

`error_integral += error*time_difference`
`control_output = - Ki * error_integral`  

This leads to a controller that responds with large delays because the error has to accumulate over time. 
This can be seen in the following video: [video](./Videos/I-Controller.mp4)

### PD-Control

A proportional and derivative controller uses both the error_rate_of_change and the error to calculate its response.

`error_rate_of_change = (current_error - previous_error)/time_difference `
   
`control_output = - Kd * error_rate_of_change - Kp * error`  

If tuned correctly, the derivative proportional will counteract the oscillations of the P-Controller by avoiding overshooting the desired state.
However, there can be a steady state offset due to noise or drifts in the system.
No large offsets can be seen in the video, this indicates that there is no significant drift in the system: 
[video](./Videos/PD-Controller.mp4)

### PID-Control

A proportional, integral and derivative controller uses the error_rate_of_change, the error_integral and the error to calculate its response.

`error_rate_of_change = (current_error - previous_error)/time_difference `

`error_integral += error*time_difference`
   
`control_output = - Kd * error_rate_of_change - Kp * error - Ki * error_integral`  

If tuned correctly, the derivative proportional will counteract the oscillations of the P-Controller by avoiding overshooting the desired state.
Furthermore, the integral portion will counteract any steady-state offsets due to drift. As there have not been found any large offsets in the PD-Controller, the Ki coefficient can be relatively small.
The result can be seen in the video: 
[video](./Videos/PID-Controller.mp4)


## Parameter Tuning
I followed the following four steps to find the final hyperparameters for this controller:
1. **P - Controller:** I started with a P-controller and adjusted the proportional gain until I found a value that could keep the vehicle in the lane for a few seconds.
As expected I saw oscialltions. The chosen Kp was 0.06.  
2. **PD - Controller:** I added the derivative control, starting with small values until I could not see any major oscillation anymore.
The chose Kd was 0.725
3. **I - Controller:** The PD controller did not see any significant offsets except for in curves, I experimented with different Ki values to decrease this offset.
The chosen Ki was 0.0012. However, I noticed that after the curve the integral component made the vehicle over-correct for the previous offset in the courve.
To avoid the integral component getting to large, I added a saturation on the integral error. This can be found in pid.cpp line 32 to line 35.
4. **Manual Twiddle Algorithm:** The previously mentioned constants of Kp = 0.06, Kd = 0.725, and Ki= 0.012, resulted in a controller that could complete an entire lap on the track.
For the Fine tuning, I added a print statement to give out the mean squared error of the controller.
I then manually followed the twiddle algorithm by letting the controller running for one lap, and using the mean-square-error to adjust the parameters.
  
    The final parameters were chosen to be :
    * Kp = 0.11
    * Ki = 0.0012
    * Kd = 1.625
    
    The best performance with these parameters was a mean squared error of ~0.2 meters.
    


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

