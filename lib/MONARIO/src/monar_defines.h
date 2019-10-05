#pragma once

// ███╗   ███╗ ██████╗ ███╗   ██╗ █████╗ ██████╗
// ████╗ ████║██╔═══██╗████╗  ██║██╔══██╗██╔══██╗
// ██╔████╔██║██║   ██║██╔██╗ ██║███████║██████╔╝
// ██║╚██╔╝██║██║   ██║██║╚██╗██║██╔══██║██╔══██╗
// ██║ ╚═╝ ██║╚██████╔╝██║ ╚████║██║  ██║██║  ██║
// ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// List of Defines used by all systems
// The ports may not be in numeric order

/**
 * List of OUTPUT ports
 *
 * [5-50]
 */

                                         // [type, description]
#define MONAR_OUTPUT_VOLTAGE          5  // [float, input voltage of the board]
#define MONAR_OUTPUT_CURRENT          6  // [int, ]
#define MONAR_OUTPUT_TEMPERATURE_1    7  // [float]
#define MONAR_OUTPUT_TEMPERATURE_2    8  // [float]
#define MONAR_OUTPUT_TEMPERATURE_3    9  // [float]
#define MONAR_OUTPUT_TEMPERATURE_4    10 // [float]
#define MONAR_OUTPUT_TEMPERATURE_5    11 // [float]
#define MONAR_OUTPUT_TEMPERATURE_6    12 // [float]
#define MONAR_OUTPUT_TEMPERATURE_7    13 // [float]
#define MONAR_OUTPUT_TEMPERATURE_8    14 // [float]
#define MONAR_OUTPUT_TEMPERATURE_AVG  15 // [float]
#define MONAR_OUTPUT_LOG              16 // [string, log messages]

/**
 * List of INPUT ports
 *
 * [51-100]
 */
                                      // [type]
#define MONAR_INPUT_TEMPERATURE_MIN   51 // [int]
#define MONAR_INPUT_TEMPERATURE_MAX   52 // [int]
#define MONAR_INPUT_RELAY             53 // [int]

/**
 * List of String
 */

#define MONAR_CONNECTED               "ONLINE"
#define MONAR_APP_CONNECTED           "Welcome back!"
