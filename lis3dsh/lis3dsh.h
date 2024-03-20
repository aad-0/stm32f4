/*
 * lis3dsh.h
 *
 *  Created on: Jan 15, 2024
 *      Author: AAD
 *
 *      Reference: lis3dsh.pdf
 *
 */

#ifndef COMPANENTS_LIS3DSH_LIS3DSH_H_
#define COMPANENTS_LIS3DSH_LIS3DSH_H_
/* Includes */
#include "stm32f4_disc1_accelero.h"
/* Typedef */
typedef struct LIS3DSH_Init_s
{
	/* Register 4 */
	uint8_t ODR;	// REG4[7:4], Output Data Rate
	uint8_t	BDU;	// REG[3],  Block Data Update
	uint8_t	AxisEnable;	// REG[2:0], ZYX
	/* Register 5 */
	uint8_t BW;	// Anti Alising filter BandWith
	uint8_t FScale;	// Scale Selection
	uint8_t	ST;	// Self Test
	uint8_t	SIM;	// Serial Interface Mod
} LIS3DSH_InitTypeDef;

#define LIS3DSH_DEVICE_ID	0x3f

/*
 *******************************************************************************
 *Registers*********************************************************************
 *******************************************************************************
 */
/*
 * Register: WHO_AM_I (0Fh)
 * Brief: Keeps Device Id
 * Mode: Read Only
 * Default: 00111111
 */
#define LIS3DSH_REG_WHO_AM_I	0x0F

// Output Data Rate
/* Table 55: ODR selection
 * ODR 3:0 - Output Data Rate (Power modes and corresponding frequencies):
 * 0000: Power down
 * 0001: 3.125 Hz
 * 0010: 6.25 Hz
 * 0011: 12.5 Hz
 * 0100: 25 Hz
 * 0101: 50 Hz
 * 0110: 100 Hz
 * 0111: 400 Hz
 * 1000: 800 Hz
 * 1001: 1600 Hz
 * Bits 7-4 are used for ODR in CTRL_REG4
 */
#define LIS3DSH_ODR_POWER_DOWN	0x00U	// Power down
#define LIS3DSH_ODR_3_125				0x10U	// 3.125 Hz
#define LIS3DSH_ODR_6_25				0x20U	// 6.25 Hz
#define LIS3DSH_ODR_12_5				0x30U	// 12.5 Hz
#define LIS3DSH_ODR_25					0x40U	// 25 Hz
#define LIS3DSH_ODR_50					0x50U	// 50 Hz
#define LIS3DSH_ODR_100					0x60U	// 100 Hz
#define LIS3DSH_ODR_400					0x70U	// 400 Hz
#define LIS3DSH_ODR_800					0x80U	// 800 Hz
#define LIS3DSH_ODR_1600				0x90U	// 1600 Hz

#define LIS3DSH_BDU_Enable	0x08U

#define LIS3DSH_X_Enable	0x01U
#define LIS3DSH_Y_Enable	0x02U
#define LIS3DSH_Z_Enable	0x04U
#define LIS3DSH_XYZ_Enable	(LIS3DSH_X_Enable \
														 | LIS3DSH_Y_Enable | LIS3DSH_Z_Enable)

/* Table 54: CTRL_REG4 register description
 * Read/Write
 * Default: 0000 0111
 * Bit 7-4: ODR 3:0 - Output data rate and power mode selection
 * Bit 3: BDU - Block Data Update (0: Continuous update; 1: Output registers not
 * 	updated until MSB and LSB have been read)
 * Bit 2: Zen - Z-axis enable (0: Z-axis disabled; 1: Z-axis enabled)
 * Bit 1: Yen - Y-axis enable (0: Y-axis disabled; 1: Y-axis enabled)
 * Bit 0: Xen - X-axis enable (0: X-axis disabled; 1: X-axis enabled)
 *
 * The BDU bit is used to inhibit the update of the output registers until both
 * upper and lower
 * 	register parts are read. In default mode (BDU = ‘0’)
 * 		the output register values are updated
 * 	continuously. When the BDU is activated (BDU = ‘1’),
 * 		the content of the output registers is
 *  not updated until both MSb and LSb are read which avoids
 *  	reading values related to different sample times
 */
#define LIS3DSH_REG_CTRL_REG4	0x20

#define LIS3DSH_BW_FILTER_800	0x00U
#define LIS3DSH_BW_FILTER_200	0x40U
#define LIS3DSH_BW_FILTER_400	0x80U
#define LIS3DSH_BW_FILTER_50	0xC0U

#define LIS3DSH_FULLSCALE_2		0x00U
#define LIS3DSH_FULLSCALE_4		0x08U
#define LIS3DSH_FULLSCALE_6		0x10U
#define LIS3DSH_FULLSCALE_8		0x18U
#define LIS3DSH_FULLSCALE_16	0x20U
#define LIS3DSH_FULLSCALE_MASK	0x38U

#define LIS3DSH_SELFTEST_NORMAL			0x00U
#define LIS3DSH_SELFTEST_POSITIVE		0x02U
#define LIS3DSH_SELFTEST_NEGATIVE		0x04U
#define LIS3DSH_SELFTEST_NOTALLOWED	0x06U

#define LIS3DSH_SERIAL_INTERFACE_MODE_4_WIRE	0x00U
#define LIS3DSH_SERIAL_INTERFACE_MODE_3_WIRE	0x01U
/*
 * Sensitivity Values for LIS3DSH Accelerometer
 *
 * Full-Scale (FS) Setting:
 * FS[2:0] | Sensitivity (mg/digit)
 * -------------------------------
 *   000   |      0.06
 *   001   |      0.12
 *   010   |      0.18
 *   011   |      0.24
 *   100   |      0.73
 */
#define LIS3DSH_SENSIVITY_AT_FULLSCALE_2	0.06f
#define LIS3DSH_SENSIVITY_AT_FULLSCALE_4	0.12f
#define LIS3DSH_SENSIVITY_AT_FULLSCALE_6	0.18f
#define LIS3DSH_SENSIVITY_AT_FULLSCALE_8	0.24f
#define LIS3DSH_SENSIVITY_AT_FULLSCALE_16	0.73f

/*
 * 7.24 CTRL_REG5 (24h)
 * Control register 5.
 * Read/Write
 * Default 0000 0000
 * Table 62. Control register 5
 *   BW2      BW1      FSCALE2   FSCALE1   FSCALE0   ST2   ST1   SIM
 *
 * Table 63. Control register 5 description
 * BW2:BW1 - Anti-aliasing filter bandwidth. Default value: 00
 *           (00: 800 Hz; 01: 200 Hz; 10: 400 Hz; 11: 50 Hz)
 * FSCALE2:0 - Full-scale selection. Default value: 00
 *             (000: ±2 g; 001: ±4 g; 010: ±6 g; 011: ±8 g; 100: ±16 g)
 * ST2:1 - Self-test enable. Default value: 00
 *         (00: self-test disabled)
 * SIM - SPI serial interface mode selection. Default value: 0
 *       (0: 4-wire interface; 1: 3-wire interface)
 *
 * Table 64. Self-test mode selection
 *   ST2    ST1    Self-test mode
 *   0      0      Normal mode
 *   0      1      Positive sign self-test
 *   1      0      Negative sign self-test
 *   1      1      Not allowed
 */
#define LIS3DSH_REG_CTRL_REG5	0x24

/*
 * 7.27 OUT_X (28h - 29h)
 * X-axis output register.
 * Default: 0000 0000
 * Table 69. OUT_X_L register
 *   XD7    XD6    XD5    XD4    XD3    XD2    XD1    XD0
 *
 * Table 70. OUT_X_L register description
 * XD[7:0] - X-axis output, low values. Default value: 0000 0000
 *
 * Table 71. OUT_X_H register
 *   XD15   XD14   XD13   XD12   XD11   XD10   XD9    XD8
 *
 * Table 72. OUT_X_H register description
 * XD[15:8] - X-axis output, high values. Default value: 0000 0000
 */
#define LIS3DSH_REG_OUT_X_LOW		0x28
#define LIS3DSH_REG_OUT_X_HIGH	0x29

/*
 * 7.28 OUT_Y (2Ah - 2Bh)
 * Y-axis output register.
 * Default: 0000 0000
 * Table 73. OUT_Y_L register
 *   YD7    YD6    YD5    YD4    YD3    YD2    YD1    YD0
 *
 * Table 74. OUT_Y_L register description
 * YD[7:0] - Y-axis output, low values. Default value: 0000 0000
 *
 * Table 75. OUT_Y_H register
 *   YD15   YD14   YD13   YD12   YD11   YD10   YD9    YD8
 *
 * Table 76. OUT_Y_H register description
 * YD[15:8] - Y-axis output, high values. Default value: 0000 0000
 */
#define LIS3DSH_REG_OUT_Y_LOW		0x2A
#define LIS3DSH_REG_OUT_Y_HIGH	0x2B

/*
 * 7.29 OUT_Z (2Ch - 2Dh)
 * Z-axis output register.
 * Default: 0000 0000
 * Table 77. OUT_Z_L register
 *   ZD7    ZD6    ZD5    ZD4    ZD3    ZD2    ZD1    ZD0
 *
 * Table 78. OUT_Z_L register description
 * ZD[7:0] - Z-axis output, low values. Default value: 0000 0000
 *
 * Table 79. OUT_Z_H register
 *   ZD15   ZD14   ZD13   ZD12   ZD11   ZD10   ZD9    ZD8
 *
 * Table 80. OUT_Z_H register description
 * ZD[15:8] - Z-axis output, high values. Default value: 0000 0000
 */
#define LIS3DSH_REG_OUT_Z_LOW		0x2C
#define LIS3DSH_REG_OUT_Z_HIGH	0x2D

/* Functions */
uint8_t LIS3DSH_GET_WHO_AM_I (void);
void LIS3DSH_Init (LIS3DSH_InitTypeDef * pInit);
void LIS3DSH_ReadXYZ (int16_t * pDataOut);
/* Board IO Functions */
void Accelero_IO_read(uint8_t* pBuffer, uint8_t WriteAddr, \
											uint16_t NumByteToWrite);
void Accelero_IO_write(uint8_t* pBuffer, uint8_t WriteAddr, \
											 uint16_t NumByteToWrite);
#endif /* COMPANENTS_LIS3DSH_LIS3DSH_H_ */
