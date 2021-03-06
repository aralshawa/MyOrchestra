// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <time.h>

// The only file that needs to be included to use the Myo C++ SDK is myo.hpp.
#include <myo/myo.hpp>

#include "OrchestralController-C-Interface.h"

#define LEFT_MYO_ID 1
#define RIGHT_MYO_ID 2
#define MAX_DELTA 100

// Classes that inherit from myo::DeviceListener can be used to receive events from Myo devices. DeviceListener
// provides several virtual functions for handling different kinds of events. If you do not override an event, the
// default behavior is to do nothing.
class DataCollector : public myo::DeviceListener {
	
public:
	void *systemContext;
	static bool updateContextOfChanges;
	
	DataCollector()
		: onArmL(false), isUnlockedL(false), roll_w_l(0), pitch_w_l(0), yaw_w_l(0), past_pitch_l(0),
		onArmR(false), isUnlockedR(false), roll_w_r(0), pitch_w_r(0), yaw_w_r(0), past_pitch_r(0) {

	}

	void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
		knownMyos.push_back(myo);
	}

	// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
	void onUnpair(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == RIGHT_MYO_ID) {
			onArmR = false;
			isUnlockedR = false;
			roll_w_r = 0;
			pitch_w_r = 0;
			yaw_w_r = 0;
		}
		else if (identifyMyo(myo) == LEFT_MYO_ID) {
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
		using std::abs;
		using std::signbit;
		
		if (identifyMyo(myo) == RIGHT_MYO_ID) {
			past_pitch_r = pitch_w_r;
			past_yaw_r = yaw_w_r;
			
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
			yaw_delta_r = yaw_w_r - past_yaw_r;
			if (abs(yaw_delta_r) > MAX_DELTA)
			{
				if (signbit(yaw_delta_r))
				{
					yaw_w_r += 360;
				}
				else
				{
					yaw_w_r-=360;
				}
			}

		}
		else if (identifyMyo(myo) == LEFT_MYO_ID) {
			past_pitch_l = pitch_w_l;
			past_yaw_l = yaw_w_l;
			
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
			yaw_delta_l = yaw_w_l - past_yaw_l;
		}
	}

	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
		myo::WarmupState warmupState) {
		if (identifyMyo(myo) == RIGHT_MYO_ID) {
			onArmR = true;
		}
		else if (identifyMyo(myo) == LEFT_MYO_ID) {
			onArmL = true;
		}
	}

	void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == RIGHT_MYO_ID) {
			onArmR = false;
		}
		else if (identifyMyo(myo) == LEFT_MYO_ID) {
			onArmL = false;
		}
	}

	void onUnlock(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == RIGHT_MYO_ID) {
			isUnlockedR = true;
		}
		else if (identifyMyo(myo) == LEFT_MYO_ID) {
			isUnlockedL = true;
		}
	}

	void onLock(myo::Myo* myo, uint64_t timestamp) {
		if (identifyMyo(myo) == RIGHT_MYO_ID) {
			isUnlockedR = false;
		}
		else if (identifyMyo(myo) == LEFT_MYO_ID) {
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

	void calibrate() {

//		lim_yaw_l[2] = ((0.3333)*(double)(lim_yaw_l[1] - lim_yaw_l[0])) + lim_yaw_l[0];
//		lim_yaw_l[3] = ((0.6667)*(double)(lim_yaw_l[1] - lim_yaw_l[0])) + lim_yaw_l[0];
//
//		lim_yaw_r[2] = ((0.3333)*(double)(lim_yaw_r[1] - lim_yaw_r[0])) + lim_yaw_r[0];
//		lim_yaw_r[3] = ((0.6667)*(double)(lim_yaw_r[1] - lim_yaw_r[0])) + lim_yaw_r[0];

		lim_pitch_r[2] = ((0.6667)*(double)(lim_pitch_r[1] - lim_pitch_r[0])) + lim_pitch_r[0];

	}

	bool onArmL, onArmR;

	bool isUnlockedL, isUnlockedR;

	int roll_w_l, pitch_w_l, yaw_w_l;
	int roll_w_r, pitch_w_r, yaw_w_r;

	int pitch_delta_l, pitch_delta_r;
	int past_pitch_l, past_pitch_r;
	
	int yaw_delta_l, yaw_delta_r;
	int past_yaw_l, past_yaw_r;

	double lim_pitch_l[3] = { 0, 0, 0 };
	double lim_yaw_l[4] = { 0, 0, 0, 0 };

	double lim_pitch_r[3] = { 0, 0, 0 };
	double lim_yaw_r[4] = { 0, 0, 0, 0 };
	
	

	std::vector<myo::Myo*> knownMyos;
	
	
	static void beginCalibrationAndBeginLoop(void *context) {
		
		// We catch any exceptions that might occur below -- see the catch statement for more details.
		try {
			// First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
			// publishing your application. The Hub provides access to one or more Myos.
			myo::Hub hub("com.example.hello-myo");
			
			// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
			DataCollector collector;
			
			collector.systemContext = context;
			
			// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
			// Hub::run() to send events to all registered device listeners.
			hub.addListener(&collector);
			
			// Determine range
			printf("Begin Calibration\n");
			hub.run(1000 / 20);
			
			for (int i = 0; i < collector.knownMyos.size(); ++i){
				if (i == RIGHT_MYO_ID){
					collector.knownMyos[i]->notifyUserAction();
				}
			}
			
			//collector.lim_yaw_l[3] = collector.yaw_w_l-collector.lim_yaw_l[2];
			//collector.lim_yaw_l[2] = collector.yaw_w_l;
			//if(std::abs(collector.lim_yaw_l[3]) > MAX_DELTA)
			//{
			//	collector.lim_yaw_l[2] = collector.yaw_w_l + 360;
			//}
			
			collector.lim_pitch_r[0] = collector.pitch_w_r; collector.lim_pitch_r[1] = collector.pitch_w_r;
			collector.lim_yaw_r[0] = collector.yaw_w_r;   collector.lim_yaw_r[1] = collector.yaw_w_r;
			
			collector.lim_pitch_l[0] = collector.pitch_w_l; collector.lim_pitch_l[1] = collector.pitch_w_l;
			collector.lim_yaw_l[0] = collector.yaw_w_l;   collector.lim_yaw_l[1] = collector.yaw_w_l;
			
			
			
			for (int i = 0; i < 100; i++) {
				hub.run(1000 / 20);
				
				if (collector.pitch_w_r < collector.lim_pitch_r[0]) { collector.lim_pitch_r[0] = collector.pitch_w_r; }
				if (collector.pitch_w_r > collector.lim_pitch_r[1]) { collector.lim_pitch_r[1] = collector.pitch_w_r; }
//				if (collector.yaw_w_r < 300 && collector.yaw_w_r   < collector.lim_yaw_r[0]) { collector.lim_yaw_r[0] = collector.yaw_w_r; }
//				if (collector.yaw_w_r > 60 && collector.yaw_w_r   > collector.lim_yaw_r[1]) { collector.lim_yaw_r[1] = collector.yaw_w_r; }
				if (collector.yaw_w_r   < collector.lim_yaw_r[0]) { collector.lim_yaw_r[0] = collector.yaw_w_r; }
				if ( collector.yaw_w_r   > collector.lim_yaw_r[1]) { collector.lim_yaw_r[1] = collector.yaw_w_r; }
				
				if (collector.pitch_w_l < collector.lim_pitch_l[0]) { collector.lim_pitch_l[0] = collector.pitch_w_l; }
				if (collector.pitch_w_l > collector.lim_pitch_l[1]) { collector.lim_pitch_l[1] = collector.pitch_w_l; }
				if (collector.yaw_w_l   < collector.lim_yaw_l[0]) { collector.lim_yaw_l[0] = collector.yaw_w_l; }
				if (collector.yaw_w_l   > collector.lim_yaw_l[1]) { collector.lim_yaw_l[1] = collector.yaw_w_l; }
				
				printf("%f, %f\n", collector.lim_yaw_r[1], collector.lim_yaw_r[0]);
			}
			
			yawCalibrationComplete(collector.systemContext, {collector.lim_yaw_r[1], collector.lim_yaw_r[0]});
			pitchCalibrationComplete(collector.systemContext, {collector.lim_pitch_l[1], collector.lim_pitch_l[0]});
			
			collector.calibrate();
			
			for (int i = 0; i < collector.knownMyos.size(); ++i){
				if (i == RIGHT_MYO_ID){
					collector.knownMyos[i]->notifyUserAction();
				}
			}
			
			printf("Calibration Complete\n");
//			collector.knownMyos.clear();
			// Finally we enter our main loop.
			while (1) {
				// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
				// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
				hub.run(1000 / 20);
				for (int i = 0; i < collector.knownMyos.size(); ++i){
					if (i == RIGHT_MYO_ID){
						collector.knownMyos[i]->notifyUserAction();
					}
				}
//				printf("%d, %f", collector.yaw_w_l, collector.lim_yaw_l[1]-collector.lim_yaw_r[0]);
//				printf(" - - %d, %f", collector.yaw_w_r, collector.lim_yaw_r[1]-collector.lim_yaw_r[0] );
//				printf("          ");
//				printf("\r");
				if (collector.pitch_w_r > collector.lim_pitch_r[2] && updateContextOfChanges) {
					onUpdateSectionSelectYaw(collector.systemContext, collector.yaw_w_r);
					onUpdateVolumeSelectPitch(collector.systemContext, collector.pitch_w_l);
				}
				
			}
			
			// If a standard exception occurred, we print out its message and exit.
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			std::cerr << "Press enter to continue.";
			std::cin.ignore();
		}
	}
};

// This is why. http://stackoverflow.com/questions/12967850/c-compiling-error-undefined-symbols-for-architecture-x86-64
bool DataCollector::updateContextOfChanges = false;
