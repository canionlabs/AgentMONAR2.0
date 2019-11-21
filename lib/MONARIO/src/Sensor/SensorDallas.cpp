#ifdef SENSOR_DALLAS
/*
* @Author: Ramon Melo
* @Date:   2018-07-24
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2018-08-20
*/

#define INVALID_VALUE -127

#include "SensorDallas.h"

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
        {
            Serial.print("0");
        }
        Serial.print(deviceAddress[i], HEX);
    }
}

namespace monar
{

SensorDallas::SensorDallas(OneWire *ow) : manager(ow), alert_temp_change(false)
{
    // pin mapping
    // pin_map[0] = MONAR_OUTPUT_TEMPERATURE_1;
    // pin_map[1] = MONAR_OUTPUT_TEMPERATURE_2;
    // pin_map[2] = MONAR_OUTPUT_TEMPERATURE_3;
    // pin_map[3] = MONAR_OUTPUT_TEMPERATURE_4;
    // pin_map[4] = MONAR_OUTPUT_TEMPERATURE_5;
    // pin_map[5] = MONAR_OUTPUT_TEMPERATURE_6;
    // pin_map[6] = MONAR_OUTPUT_TEMPERATURE_7;
    // pin_map[7] = MONAR_OUTPUT_TEMPERATURE_8;
    // END pin mapping

    Serial.print("Locating devices...");
    manager.begin();
    sensor_count = manager.getDeviceCount();

    Serial.print("Found ");
    Serial.print(sensor_count, DEC);
    Serial.println(" devices.");

    Serial.print("Parasite power is: ");
    if (manager.isParasitePowerMode())
    {
        Serial.println("ON");
    }
    else
    {
        Serial.println("OFF");
    }

    sensors = new DeviceAddress[sensor_count];

    for (int i = 0; i < sensor_count; ++i)
    {
        if (manager.getAddress(sensors[i], i))
        {
            Serial.println("Found Device");
            manager.setResolution(sensors[i], 9);
        }
        else
        {
            Serial.println("Unable to find address for Device");
        }

        setData(i, 0);
    }
}

void SensorDallas::service()
{
    manager.requestTemperatures();

    // float avg_temp = 0;

    for (int i = 0; i < sensor_count; ++i)
    {
        float current_temp = manager.getTempC(sensors[i]);

        if (current_temp == INVALID_VALUE)
        {
            current_temp = read(i);
        }

        // avg_temp += current_temp;
        setData(i, current_temp);
    }

    // if (sensor_count > 1)
    // {
    //     avg_temp /= sensor_count;
    // }

    // setData(MONAR_OUTPUT_TEMPERATURE_AVG, avg_temp);
}

int SensorDallas::length()
{
	return sensor_count;
}

char SensorDallas::prefix()
{
	return 't';
}

// void SensorDallas::receive(int pin, int value)
// {

//     switch (pin)
//     {
//     case MONAR_INPUT_TEMPERATURE_MIN:
//         temp_min = value;
//         alert_temp_change = true;

//         Serial.println("update min");
//         break;
//     case MONAR_INPUT_TEMPERATURE_MAX:
//         temp_max = value;
//         alert_temp_change = true;

//         Serial.println("update max");
//         break;
//     }
// }

// void SensorDallas::notify(void (*alert)(int, String, bool))
// {

//     if (alert_temp_change)
//     {

//         (*alert)(MONAR_OUTPUT_LOG, String("Configuração: Temperatura minima de ") + temp_min, false);
//         (*alert)(MONAR_OUTPUT_LOG, String("Configuração: Temperatura máxima de ") + temp_max, false);

//         alert_temp_change = false;
//     }

//     for (int i = 0; i < sensor_count; ++i)
//     {
//         float temp = info[pin_map[i]];

//         if (temp < temp_min)
//         {
//             (*alert)(MONAR_OUTPUT_LOG, String("Alerta: BAIXA Temperatura detectada"), true);
//             break;
//         }
//         if (temp > temp_max)
//         {
//             (*alert)(MONAR_OUTPUT_LOG, String("Alerta: ALTA Temperatura detectada"), true);
//             break;
//         }
//     }
// }
} // namespace monar
#endif