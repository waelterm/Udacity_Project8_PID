#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
	auto found_null = s.find("null");
	auto b1 = s.find_first_of("[");
	auto b2 = s.find_last_of("]");
	if (found_null != string::npos) {
		return "";
	}
	else if (b1 != string::npos && b2 != string::npos) {
		return s.substr(b1, b2 - b1 + 1);
	}
	return "";
}

int main() {
	uWS::Hub h;
	double squared_error_sum = 0.0;
	long long number_of_cycles = 0;
	PID pid;
	pid.Init(0.11, 0.0012, 1.625); //0.06 0.0012,0.725

	h.onMessage([&pid, &squared_error_sum, &number_of_cycles](uWS::WebSocket<uWS::SERVER> ws, char* data, size_t length,
		uWS::OpCode opCode) {
		// "42" at the start of the message means there's a websocket message event.
		// The 4 signifies a websocket message
		// The 2 signifies a websocket event
		if (length && length > 2 && data[0] == '4' && data[1] == '2') {
			auto s = hasData(string(data).substr(0, length));

			if (s != "") {
				auto j = json::parse(s);

				string event = j[0].get<string>();

				if (event == "telemetry") {
					// j[1] is the data JSON object
					double cte = std::stod(j[1]["cte"].get<string>());
					double speed = std::stod(j[1]["speed"].get<string>());
					double angle = std::stod(j[1]["steering_angle"].get<string>());
					double steer_value;
					pid.UpdateError(cte);
					steer_value = -1.5 * pid.TotalError();
					if (steer_value > 1) {
						steer_value = 1;
					}
					else if (steer_value < -1) {
						steer_value = -1;
					}

					// DEBUG
					//std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl; 

					// Calculating mean-aquared-error
					squared_error_sum += cte * cte;
					number_of_cycles += 1;
					// std::cout << "Average Squared Error: " << squared_error_sum/number_of_cycles <<std::endl;

					json msgJson;
					msgJson["steering_angle"] = steer_value;
					msgJson["throttle"] = 0.22;
					auto msg = "42[\"steer\"," + msgJson.dump() + "]";
					//std::cout << msg << std::endl;
					ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
				}  // end "telemetry" if
			}
			else {
				// Manual driving
				string msg = "42[\"manual\",{}]";
				ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
			}
		}  // end websocket message if
	}); // end h.onMessage

	h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
		std::cout << "Connected!!!" << std::endl;
	});

	h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
		char* message, size_t length) {
		ws.close();
		std::cout << "Disconnected" << std::endl;
	});

	int port = 4567;
	if (h.listen(port)) {
		std::cout << "Listening to port " << port << std::endl;
	}
	else {
		std::cerr << "Failed to listen to port" << std::endl;
		return -1;
	}

	h.run();
}
