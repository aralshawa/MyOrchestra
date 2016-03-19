// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.
#pragma comment(lib, "winmm.lib")

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>


#include <windows.h>
#include <mmsystem.h>
#include <fstream>
#include <conio.h>

// The only file that needs to be included to use the Myo C++ SDK is myo.hpp.
#include <myo/myo.hpp>

// Classes that inherit from myo::DeviceListener can be used to receive events from Myo devices. DeviceListener
// provides several virtual functions for handling different kinds of events. If you do not override an event, the
// default behavior is to do nothing.
class DataCollector : public myo::DeviceListener {
	
public:
    DataCollector()
    : onArmL(false), isUnlockedL(false), actionEnabledL(false), roll_w_l(0), pitch_w_l(0), yaw_w_l(0), past_pitch_l(0),
	  onArmR(false), isUnlockedR(false), actionEnabledR(false), roll_w_r(0), pitch_w_r(0), yaw_w_r(0), past_pitch_r(0){

    }

	void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
		knownMyos.push_back(myo);
	}

    // onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
    void onUnpair(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == 1) {
			onArmR = false;
			isUnlockedR = false;
			roll_w_r = 0;
			pitch_w_r = 0;
			yaw_w_r = 0;
		} else if (identifyMyo(myo) == 2) {
			onArmL = false;
			isUnlockedL = false;
			roll_w_l = 0;
			pitch_w_l = 0;
			yaw_w_l = 0;
		}
    }

    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {

        using std::atan2;
        using std::asin;
        using std::sqrt;
        using std::max;
        using std::min;
		if (identifyMyo(myo) == 1) {
			past_pitch_r = pitch_w_r;

			// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
			float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
				1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
			float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
			float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
				1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

			// Convert the floating point angles in radians to a scale from 0 to 360.
			roll_w_r = static_cast<int>((roll + (float)M_PI) / (M_PI * 2.0f) * 360);
			pitch_w_r = static_cast<int>((pitch + (float)M_PI / 2.0f) / M_PI * 360);
			yaw_w_r = static_cast<int>((yaw + (float)M_PI) / (M_PI * 2.0f) * 360);

			pitch_delta_r = pitch_w_r - past_pitch_r;

		} else if (identifyMyo(myo) == 2) {
			past_pitch_l = pitch_w_l;

			// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
			float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
				1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
			float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
			float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
				1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

			// Convert the floating point angles in radians to a scale from 0 to 360.
			roll_w_l = static_cast<int>((roll + (float)M_PI) / (M_PI * 2.0f) * 360);
			pitch_w_l = static_cast<int>((pitch + (float)M_PI / 2.0f) / M_PI * 360);
			yaw_w_l = static_cast<int>((yaw + (float)M_PI) / (M_PI * 2.0f) * 360);

			pitch_delta_l = pitch_w_l - past_pitch_l;
		}
    }

    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
		if (identifyMyo(myo) == 1) {
			if (pose != myo::Pose::unknown && pose == myo::Pose::fingersSpread) {
				actionEnabledR = false;
			} else if (pose != myo::Pose::unknown && myo::Pose::fist) {
				actionEnabledR = true;
			}
		} else if (identifyMyo(myo) == 2) {
			if (pose != myo::Pose::unknown && pose == myo::Pose::fingersSpread) {
				actionEnabledL = false;
			} else if (pose != myo::Pose::unknown && myo::Pose::fist) {
				actionEnabledL = true;
			}
		}
    }

    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                   myo::WarmupState warmupState) {
		if (identifyMyo(myo) == 1) {
			onArmR = true;
		} else if (identifyMyo(myo) == 2) {
			onArmL = true;
		}
    }

    void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == 1) {
			onArmR = false;
		}
		else if (identifyMyo(myo) == 2) {
			onArmL = false;
		}
    }

    void onUnlock(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == 1) {
			isUnlockedR = true;
		}
		else if (identifyMyo(myo) == 2) {
			isUnlockedL = true;
		}
    }

    void onLock(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == 1) {
			isUnlockedR = false;
		}
		else if (identifyMyo(myo) == 2) {
			isUnlockedL = false;
		}
    }

	size_t identifyMyo(myo::Myo* myo) {
		// Walk through the list of Myo devices that we've seen pairing events for.
		for (size_t i = 0; i < knownMyos.size(); ++i) {
			// If two Myo pointers compare equal, they refer to the same Myo device.
			if (knownMyos[i] == myo) {
				return i + 1;
			}
		}

		return 0;
	}

    bool onArmL, onArmR;

	bool isUnlockedL, isUnlockedR;

	int roll_w_l, pitch_w_l, yaw_w_l;
	int roll_w_r, pitch_w_r, yaw_w_r;

	bool actionEnabledL, actionEnabledR;
	int pitch_delta_l, pitch_delta_r;
	int past_pitch_l, past_pitch_r;

	static const int refresh_time = 50;

	std::vector<myo::Myo*> knownMyos;
};

int main(int argc, char** argv) {
    // We catch any exceptions that might occur below -- see the catch statement for more details.
    try {

    // First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
    // publishing your application. The Hub provides access to one or more Myos.
    myo::Hub hub("com.example.hello-myo");

    //std::cout << "Attempting to find a Myo..." << std::endl;

    //// Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
    //// immediately.
    //// waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
    //// if that fails, the function will return a null pointer.
    //myo::Myo* myo = hub.waitForMyo(10000);

    //// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
    //if (!myo) {
    //    throw std::runtime_error("Unable to find a Myo!");
    //}

    //// We've found a Myo.
    //std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

    // Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
    DataCollector collector;

    // Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
    // Hub::run() to send events to all registered device listeners.
    hub.addListener(&collector);

    // Finally we enter our main loop.
    while (1) {
        // In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
        // In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
        hub.run(collector.refresh_time);
        


		std::cout << "                              ";
		std::cout << '\r';
		std::cout << "Yaw: " << "[" << collector.yaw_w_l << "," << collector.yaw_w_r << "]";

		//if (collector.yaw_w_r > 300 || collector.yaw_w_r < 80) {
		//	std::cout << "Section: " << "String     ";
		//	if (collector.pitch_w_r > 220 || collector.pitch_w_r < 150) {
		//		PlaySound("string.wav", NULL, SND_FILENAME);
		//	}
		//}
		//else if (collector.yaw_w_r > 150) {
		//	std::cout << "Section: " << "Percussion ";
		//	if (collector.pitch_w_r > 220 || collector.pitch_w_r < 150) {
		//		PlaySound("percussion.wav", NULL, SND_FILENAME);
		//	}
		//} else {
		//	std::cout << "Section: " << "Woodwind   ";
		//	if (collector.pitch_w_r > 220 || collector.pitch_w_r < 150) {
		//		PlaySound("woodwind.wav", NULL, SND_FILENAME);
		//	}
		//}
    }

    // If a standard exception occurred, we print out its message and exit.
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        return 1;
    }
}
