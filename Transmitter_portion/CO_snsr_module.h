#ifndef CO_SNSR_MOD_H_
#define CO_SNSR_MOD_H_

#include <Arduino.h>

#define         TRIGGER_VALUE                (800)
#define         MQ_PIN                       (A0)     //define which  analog input channel you are going to use
#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the  chart in datasheet
 
/**********************Software Related Macros***********************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are  going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are  going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 

/*********************Application Related Macros*********************************/
#define         GAS_LPG                      (0)
#define         GAS_CO                       (1)
#define         GAS_SMOKE                    (2)

/****************************Globals**********************************************/
extern float           LPGCurve[3];   //two points are taken from the curve.  
                                                          //with these two points,  a line is formed which is "approximately equivalent"
                                                          //to  the original curve. 
                                                          //data  format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59) 
extern float           COCurve[3];    //two points are taken from the curve.  
                                                           //with these two points,  a line is formed which is "approximately equivalent" 
                                                           //to  the original curve.
                                                          //data  format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
extern float           SmokeCurve[3];    //two points are taken from the curve.  
                                                          //with these two points,  a line is formed which is "approximately equivalent" 
                                                           //to  the original curve.
                                                          //data  format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     
extern float           Ro;                //Ro is initialized to 10 kilo ohms
  
 
/****************  MQResistanceCalculation **************************************
Input:   raw_adc  - raw value read from adc, which represents the voltage
Output:  the calculated  sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider.  Given the voltage
         across the load resistor and its resistance, the resistance  of the sensor
         could be derived.
**********************************************************************************/  
float MQResistanceCalculation(int raw_adc);
  
/*************************** MQCalibration **************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function  assumes that the sensor is in clean air. It use  
         MQResistanceCalculation  to calculates the sensor resistance in clean air 
         and then divides it  with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about 
         10, which differs  slightly between different sensors.
**********************************************************************************/  
float MQCalibration(int mq_pin);

/***************************  MQRead *******************************************
Input:   mq_pin - analog  channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation  to caculate the sensor resistenc (Rs).
         The Rs changes as the sensor  is in the different consentration of the target
         gas. The sample times  and the time interval between samples could be configured
         by changing  the definition of the macros.
**********************************************************************************/  
float MQRead(int mq_pin);

 
/***************************  MQGetGasPercentage ********************************
Input:   rs_ro_ratio -  Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the  target gas
Remarks: This function passes different curves to the MQGetPercentage  function which 
         calculates the ppm (parts per million) of the target  gas.
**********************************************************************************/  
int MQGetGasPercentage(float rs_ro_ratio, int gas_id);
 
/***************************  MQGetPercentage  ********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of  the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic  value of ppm) 
         of the line could be derived if y(rs_ro_ratio) is provided.  As it is a 
         logarithmic coordinate, power of 10 is used to convert the  result to non-logarithmic 
         value.
**********************************************************************************/  
int  MQGetPercentage(float rs_ro_ratio, float *pcurve); 


#endif