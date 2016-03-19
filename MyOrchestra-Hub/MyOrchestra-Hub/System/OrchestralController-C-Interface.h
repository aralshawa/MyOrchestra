//
//  OrchestralController-C-Interface.h
//  MyOrchestra-Hub
//
//  Created by Abdul Al-Shawa on 2016-03-19.
//  Copyright © 2016 Abdul Al-Shawa. All rights reserved.
//

#ifndef OrchestralController_C_Interface_h
#define OrchestralController_C_Interface_h

struct MaxMinCalibrationTuple {
	double max;
	double min;
};

/*
 Interface Details
 In order to call the following trampoline functions, the caller must hold a reference to the Obj-C object. Orchestral controller will call the following on the Myo layer to init the calibration.
 void beginCalibration(void *callbackContext)
 */

#pragma mark - C trampoline functions
void yawCalibrationComplete(void *context, struct MaxMinCalibrationTuple result);
void pitchCalibrationComplete(void *context, struct MaxMinCalibrationTuple result);

void onUpdateSectionSelectYaw(void *context, double degrees);
void onUpdateVolumeSelectPitch(void *context, double degrees);

#endif /* OrchestralController_C_Interface_h */
