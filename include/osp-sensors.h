/* Open Sensor Platform Project
 * https://github.com/sensorplatforms/open-sensor-platform
 *
 * Copyright (C) 2013 Sensor Platforms Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined (OSP_SENSORS_H)
#define   OSP_SENSORS_H

/* This file is meant to provide a common definition of sensor related enumerations/defines and
 * generally should not depend on any other includes
 */

/*-------------------------------------------------------------------------------------------------*\
 |    I N C L U D E   F I L E S
\*-------------------------------------------------------------------------------------------------*/
#include <stdint.h>
/*-------------------------------------------------------------------------------------------------*\
 |    C O N S T A N T S   &   M A C R O S
\*-------------------------------------------------------------------------------------------------*/
#define SENSOR_SUBTYPE_UNUSED               0   //!< Subtype is not used for the sensor type
#define SENSOR_SUBTYPE_START                1   //!< Subtype enumeration starts with 1
#define SENSOR_DEVICE_PRIVATE_BASE          0x10000 //!< Android defined private sensor type base

#define M_PSensorToAndroidBase(type)        ((type) | SENSOR_DEVICE_PRIVATE_BASE)
#define M_AndroidToPSensorBase(type)        ( type & 0xFFFF)

/*-------------------------------------------------------------------------------------------------*\
 |    T Y P E   D E F I N I T I O N S
\*-------------------------------------------------------------------------------------------------*/

/* Private Sensor types (translates to Android SENSOR_TYPE_DEVICE_PRIVATE_BASE start) */
typedef enum _PSensorType {
    PSENSOR_ENUM_FIRST_SENSOR              =  0,

    PSENSOR_DEBUG_TUNNEL                   =  PSENSOR_ENUM_FIRST_SENSOR, //!< Debug message pipe to host
    PSENSOR_ACCELEROMETER_RAW              = 1,  //!< raw accelerometer data (direct from sensor)
    PSENSOR_MAGNETIC_FIELD_RAW             = 2,  //!< magnetometer data (direct from sensor)
    PSENSOR_GYROSCOPE_RAW                  = 3,  //!< calibrated gyroscope data (direct from sensor)
    PSENSOR_LIGHT_UV                       = 4,  //!< UV light sensor data (Android Units)
    PSENSOR_LIGHT_RGB                      = 5,  //!< RGB light sensor data (Android Units)
//QUOC:  step sensitity detector is no longer support in MotionQ. Should we remove this? 
    PSENSOR_STEP                           = 6,  //!< step data
    PSENSOR_ACCELEROMETER_UNCALIBRATED     = 7,  //!< uncalibrated accelerometer data (Android Units)
// What is the difference between this orientation and Android orientation sensor?
    PSENSOR_ORIENTATION                    = 8,  //!< yaw, pitch, roll (also use this for Win8 Inclinometer)
    PSENSOR_CONTEXT_DEVICE_MOTION          = 9,  //!< context of device relative to world frame
    PSENSOR_CONTEXT_CARRY                  = 10, //!< context of device relative to user
    PSENSOR_CONTEXT_POSTURE                = 11, //!< context of user relative to world frame
    PSENSOR_CONTEXT_TRANSPORT              = 12, //!< context of environment relative to world frame
// Should gesture be part of android sensor ? 
    PSENSOR_GESTURE_EVENT                  = 13, //!< gesture event such as a double-tap or shake
    PSENSOR_HEART_RATE                     = 14, //!< heart-rate data
    SYSTEM_REAL_TIME_CLOCK                 = 15,  //!< Real time clock used for time stamp
	
	NUM_PRIVATE_SENSOR_TYPE                        //!< Total number of Private sensor type
} PSensorType_t;


//! use to specify the kind of sensor output result
/*!
 * \sa OSP_SubscribeSensorResult
 *
 *  Final units of input/outputs are defined by the sensor data convention field of the sensor descriptor.
 *  Flags in the descriptor specify if sensor is calibrated/uncalibrated and/or used as input
 *  If a sensor type not is supported by the library implementation, an error will be returned on its usage
 */
/* Android sensor types - this should match the defines/enumeration in Android's sensors.h */
/* Note that these are defined as SENSOR_ instead of SENSOR_TYPE_ to avoid clash with Android defines in
 * situations where this header in included for packet processing in Sensor HAL
 */
typedef enum _ASensorType {
    SENSOR_META_DATA                        = 0,
    SENSOR_ACCELEROMETER                    = 1,
    SENSOR_GEOMAGNETIC_FIELD                = 2,
    SENSOR_MAGNETIC_FIELD                   = SENSOR_GEOMAGNETIC_FIELD,
    SENSOR_ORIENTATION                      = 3,
    SENSOR_GYROSCOPE                        = 4,
    SENSOR_LIGHT                            = 5,
    SENSOR_PRESSURE                         = 6,
    SENSOR_TEMPERATURE                      = 7,
    SENSOR_PROXIMITY                        = 8,
    SENSOR_GRAVITY                          = 9,
    SENSOR_LINEAR_ACCELERATION              = 10,
    SENSOR_ROTATION_VECTOR                  = 11,
    SENSOR_RELATIVE_HUMIDITY                = 12,
    SENSOR_AMBIENT_TEMPERATURE              = 13,
    SENSOR_MAGNETIC_FIELD_UNCALIBRATED      = 14,
    SENSOR_GAME_ROTATION_VECTOR             = 15,
    SENSOR_GYROSCOPE_UNCALIBRATED           = 16,
    SENSOR_SIGNIFICANT_MOTION               = 17,
    SENSOR_STEP_DETECTOR                    = 18,
    SENSOR_STEP_COUNTER                     = 19,
    SENSOR_GEOMAGNETIC_ROTATION_VECTOR      = 20,

    NUM_ANDROID_SENSOR_TYPE,                      //!< Total number of Android sensor type

    AP_PSENSOR_DEBUG_TUNNEL                 =  M_PSensorToAndroidBase(PSENSOR_DEBUG_TUNNEL),
    AP_PSENSOR_ACCELEROMETER_RAW            =  M_PSensorToAndroidBase(PSENSOR_ACCELEROMETER_RAW),
    AP_PSENSOR_MAGNETIC_FIELD_RAW           =  M_PSensorToAndroidBase(PSENSOR_MAGNETIC_FIELD_RAW),
    AP_PSENSOR_GYROSCOPE_RAW                =  M_PSensorToAndroidBase(PSENSOR_GYROSCOPE_RAW),
    AP_PSENSOR_LIGHT_UV                     =  M_PSensorToAndroidBase(PSENSOR_LIGHT_UV),
    AP_PSENSOR_LIGHT_RGB                    =  M_PSensorToAndroidBase(PSENSOR_LIGHT_RGB),
    AP_PSENSOR_STEP                         =  M_PSensorToAndroidBase(PSENSOR_STEP),
    AP_PSENSOR_ACCELEROMETER_UNCALIBRATED   =  M_PSensorToAndroidBase(PSENSOR_ACCELEROMETER_UNCALIBRATED),
    AP_PSENSOR_ORIENTATION                  =  M_PSensorToAndroidBase(PSENSOR_ORIENTATION),
    AP_PSENSOR_CONTEXT_DEVICE_MOTION        =  M_PSensorToAndroidBase(PSENSOR_CONTEXT_DEVICE_MOTION),
    AP_PSENSOR_CONTEXT_CARRY                =  M_PSensorToAndroidBase(PSENSOR_CONTEXT_CARRY),
    AP_PSENSOR_CONTEXT_POSTURE              =  M_PSensorToAndroidBase(PSENSOR_CONTEXT_POSTURE),
    AP_PSENSOR_CONTEXT_TRANSPORT            =  M_PSensorToAndroidBase(PSENSOR_CONTEXT_TRANSPORT),
    AP_PSENSOR_GESTURE_EVENT                =  M_PSensorToAndroidBase(PSENSOR_GESTURE_EVENT),
    AP_PSENSOR_HEART_RATE                   =  M_PSensorToAndroidBase(PSENSOR_HEART_RATE),
    AP_SYSTEM_REAL_TIME_CLOCK               =  M_PSensorToAndroidBase(SYSTEM_REAL_TIME_CLOCK),
	
} ASensorType_t; 



/* Sensor Parameter Identifiers */
typedef enum _SensorParamId {
    SENSOR_PARAM_ERROR_CODE                 = 0, //!< Used to convey error code instead of parameter value
    SENSOR_PARAM_OFFSET                     = 1, //!< Offset or bias of a sensor
    SENSOR_PARAM_DATA_RATE                  = 2, //!< Datarate for the sensor
    SENSOR_PARAM_BAND_WIDTH                 = 3, //!< Bandwidth setting for the sensor
    SENSOR_PARAM_HP_FILTER                  = 4, //!< High Pass filter setting for the sensor
    SENSOR_PARAM_LP_FILTER                  = 5, //!< Low Pass filter setting for the sensor
    SENSOR_PARAM_ENABLE                     = 6, //!< Sensor Enable control

    NUM_SENSOR_PARAM
} SensorParamId_t;

#if (1) 
// QUOC comment: Define the enumeration for the physical input sensors. This is different than 
// the sensor type in ASensorType_t which is used solely for subscribes sensor output data.
//
typedef enum _InputSensor
{
    ACCEL_INPUT_SENSOR,
    MAG_INPUT_SENSOR,
    GYRO_INPUT_SENSOR,
	LIGHT_INPUT_SENSOR,
    PRESSURE_INPUT_SENSOR,
	TEMPERATURE_INPUT_SENSOR,
	PROXIMITY_INPUT_SENSOR,
	RELATIVE_HUMIDITY_INPUT_SENSOR,
	AMBIENT_TEMPERATURE_INPUT_SENSOR,
	
    NUM_INPUT_SENSORS,
    UNKNOWN_INPUT_SENSOR = 0xFF
} InputSensor_t;
#endif 


//!  Use these values as a sub-type for  STEP result
typedef enum _StepSubType {
    CONTEXT_STEP  = SENSOR_SUBTYPE_START,  //!< only one kind of step now
    STEP_SEGMENT_DETECTOR,                 //!< low compute trigger for analyzing if step may have occured

    NUM_PSENSOR_STEP_SUBTYPE
} StepSubType_t;

//! Use these values as a sub-type for CONTEXT_DEVICE_MOTION result
typedef enum _ContextDeviceMotionSubType {
    CONTEXT_DEVICE_MOTION_STILL  = SENSOR_SUBTYPE_START,
    CONTEXT_DEVICE_MOTION_ACCELERATING,
    CONTEXT_DEVICE_MOTION_ROTATING,
    CONTEXT_DEIVCE_MOTION_TRANSLATING,
    CONTEXT_DEVICE_MOTION_FREE_FALLING,
    CONTEXT_DEVICE_MOTION_SIGNIFICANT_MOTION,        //!< significant motion (as specified by Android HAL 1.0)
    CONTEXT_DEVICE_MOTION_SIGNIFICANT_STILLNESS,     //!< complement to significant motion
    CONTEXT_DEVICE_MOTION_CHANGE_DETECTOR,           //!< low compute trigger for seeing if context may have changed

    NUM_PSENSOR_CONTEXT_DEVICE_MOTION_SUBTYPE
} ContextDeviceMotionSubType_t;

//!  Use these values as a sub-type for  CONTEXT_CARRY result
typedef enum _ContextCarrySubType {
    CONTEXT_CARRY_IN_POCKET     = SENSOR_SUBTYPE_START,
    CONTEXT_CARRY_IN_HAND,
    CONTEXT_CARRY_NOT_ON_PERSON,
    CONTEXT_CARRY_IN_HAND_FRONT,
    CONTEXT_CARRY_IN_HAND_SIDE,

    NUM_PSENSOR_CONTEXT_CARRY_SUBTYPE
} ContextCarrySubType_t;

//!  Use these values as a sub-type for CONTEXT_POSTURE result
typedef enum _ContextPostureSubType {
    CONTEXT_POSTURE_WALKING     = SENSOR_SUBTYPE_START,
    CONTEXT_POSTURE_STANDING,
    CONTEXT_POSTURE_SITTING,
    CONTEXT_POSTURE_JOGGING,
    CONTEXT_POSTURE_RUNNING,

    NUM_PSENSOR_CONTEXT_POSTURE_SUBTYPE
} ContextPostureSubType_t;


//!  Use these values as a sub-type for  CONTEXT_TRANSPORT result
typedef enum _ContextTransportSubType {
    CONTEXT_TRANSPORT_VEHICLE   = SENSOR_SUBTYPE_START,
    CONTEXT_TRANSPORT_CAR,
    CONTEXT_TRANSPORT_TRAIN,
    CONTEXT_TRANSPORT_UP_STAIRS,
    CONTEXT_TRANSPORT_DOWN_STAIRS,
    CONTEXT_TRANSPORT_UP_ELEVATOR,
    CONTEXT_TRANSPORT_DOWN_ELEVATOR,
    CONTEXT_TRANSPORT_ON_BIKE,

    NUM_PSENSOR_CONTEXT_TRANSPORT_SUBTYPE
} ContextTransportSubType_t;

//!  Use these values as a sub-type for  GESTURE_EVENT result
typedef enum _GestureSubType {
    SENSOR_GESTURE_TAP          = SENSOR_SUBTYPE_START,
    SENSOR_GESTURE_DOUBLE_TAP,
    SENSOR_GESTURE_SHAKE,

    NUM_PSENSOR_GESTURE_SUBTYPE
} GestureSubType_t;

//! Time Stamp definition for capturing raw time counts
typedef union _TimeStamp {
    uint64_t TS64;
    uint32_t TS32[2];
    uint8_t  TS8[8];
} TimeStamp_t;

//! Generic structure definition for a 3-axis sensor raw values in 2's complement format
typedef struct TriAxisRawData_t
{
    TimeStamp_t  TStamp;
    int32_t      Axis[3];
} TriAxisRawData_t;

/*-------------------------------------------------------------------------------------------------*\
 |    E X T E R N A L   V A R I A B L E S   &   F U N C T I O N S
\*-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------*\
 |    P U B L I C   V A R I A B L E S   D E F I N I T I O N S
\*-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------*\
 |    P U B L I C   F U N C T I O N   D E C L A R A T I O N S
\*-------------------------------------------------------------------------------------------------*/


#endif /* OSP_SENSORS_H */
/*-------------------------------------------------------------------------------------------------*\
 |    E N D   O F   F I L E
\*-------------------------------------------------------------------------------------------------*/
