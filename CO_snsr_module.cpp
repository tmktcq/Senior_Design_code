#include "CO_snsr_module.h"

float LPGCurve[3] = {2.3, 0.21, -0.47}; // two points are taken from the curve.
                                        // with these two points,  a line is formed which is "approximately equivalent"
                                        // to  the original curve.
                                        // data  format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59)
float COCurve[3] = {2.3, 0.72, -0.34}; // two points are taken from the curve.
                                       // with these two points,  a line is formed which is "approximately equivalent"
                                       // to  the original curve.
                                       // data  format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15)
float SmokeCurve[3] = {2.3, 0.53, -0.44}; // two points are taken from the curve.
                                          // with these two points,  a line is formed which is "approximately equivalent"
                                          // to  the original curve.
                                          // data  format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)
float Ro = 10; // Ro is initialized to 10 kilo ohms

float MQResistanceCalculation(int raw_adc)
{
    return (((float)RL_VALUE * (1023 - raw_adc) / raw_adc));
}

float MQCalibration(int mq_pin)
{
    int i;
    float val = 0;

    for (i = 0; i < CALIBARAION_SAMPLE_TIMES; i++)
    { // take multiple samples
        val += MQResistanceCalculation(analogRead(mq_pin));
        delay(CALIBRATION_SAMPLE_INTERVAL);
    }
    val = val / CALIBARAION_SAMPLE_TIMES; // calculate the average  value

    val = val / RO_CLEAN_AIR_FACTOR; // divided  by RO_CLEAN_AIR_FACTOR yields the Ro
                                     // according  to the chart in the datasheet

    return val;
}

float MQRead(int mq_pin)
{
    int i;
    float rs = 0;

    for (i = 0; i < READ_SAMPLE_TIMES; i++)
    {
        rs += MQResistanceCalculation(analogRead(mq_pin));
        delay(READ_SAMPLE_INTERVAL);
    }

    rs = rs / READ_SAMPLE_TIMES;

    return rs;
}

int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
    if (gas_id == GAS_LPG)
    {
        return MQGetPercentage(rs_ro_ratio, LPGCurve);
    }
    else if (gas_id == GAS_CO)
    {
        return MQGetPercentage(rs_ro_ratio, COCurve);
    }
    else if (gas_id == GAS_SMOKE)
    {
        return MQGetPercentage(rs_ro_ratio, SmokeCurve);
    }

    return 0;
}

int MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
    return (pow(10, (((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
}