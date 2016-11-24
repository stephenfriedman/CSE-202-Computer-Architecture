/*********************************************************
 * config.h - Configuration data for the driver.c program.
 *********************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* 
 * CPEs for the baseline (naive) version of the rotate function that
 * was handed out to the students. Rd is the measured CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
#define R64    3.0
#define R128   4.1
#define R256   6.7
#define R512   10.9
#define R1024  13.6
#define R2048  34.4
#define R4096  46.8

/* 
 * CPEs for the baseline (naive) version of the smooth function that
 * was handed out to the students. Sd is the measure CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
#define S32   64.5
#define S64   64.7
#define S128  64.8
#define S256  64.9
#define S512  65.0
#define S1024  66.3
#define S2048  65.8


#endif /* _CONFIG_H_ */
