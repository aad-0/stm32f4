/*
 * lis3dsh.c
 *
 *  Created on: Jan 15, 2024
 *      Author: AAD
 */
/* Includes */
#include "stdint.h"
#include "lis3dsh.h"
/* Static variables */
uint8_t LIS3DSH_Initialized = 0;

uint8_t
LIS3DSH_GET_WHO_AM_I (void)
{
	uint8_t deviceId = 0x00;
	Accelero_IO_read(&deviceId, LIS3DSH_REG_WHO_AM_I, 1);
	return deviceId;
}


/*
 * @brief: Initilalize LIS3DSH
 * @param1: LIS3DSH_Init *
 * 		uint8_t ODR;	// LIS3DSH_ODR_X
 * 		uint8_t	BDU;	// LIS3DSH_BDU_Enable or 0
 * 		uint8_t	AxisEnable;	// LIS3DSH_X_Enable
 * 		uint8_t BW;	// LIS3DSH_BW_FILTER_X
 * 		uint8_t FScale;	// LIS3DSH_FULLSCALE_X
 * 		uint8_t	ST;	// LIS3DSH_SERIAL_INTERFACE_MODE_X_WIRE
 * 		uint8_t	SIM;	// Serial Interface Mod
 *	@retval: None
 */
void
LIS3DSH_Init (LIS3DSH_InitTypeDef * pInit)
{
	if (! LIS3DSH_Initialized)
		{
			/* Set Register 4 */
			++ LIS3DSH_Initialized;
			uint8_t reg = 0x00;
			reg |= pInit->ODR;
			reg |= pInit->BDU;
			reg |= pInit->AxisEnable;
			// Write
			Accelero_IO_write(&reg, LIS3DSH_REG_CTRL_REG4, 1);
			/* Set Register 5 */
			reg = 0x00;
			reg |= pInit->BW;
			reg |= pInit->FScale;
			reg |= pInit->ST;
			reg |= pInit->SIM;// Write
			Accelero_IO_write(&reg, LIS3DSH_REG_CTRL_REG5, 1);
		}
}

/*
 * @brief: pDataOut[3] // XYZ
 * @retval: None
 */
void
LIS3DSH_ReadXYZ (int16_t * pDataOut)
{
	/* Read Register 5 for sensivity */
	uint8_t ctrlReg5 = 0x00;
	float sensivity = 0.0f;
	Accelero_IO_read(&ctrlReg5, LIS3DSH_REG_CTRL_REG5, 1);
	/* Manipulate Data respect to Sensivity */
	// pData[0] LOW pData[0.1] High
	// Set Sensivity
	switch (LIS3DSH_FULLSCALE_MASK & ctrlReg5)
	{
		case LIS3DSH_FULLSCALE_2:
			sensivity = LIS3DSH_SENSIVITY_AT_FULLSCALE_2;
			break;
		case LIS3DSH_FULLSCALE_4:
			sensivity = LIS3DSH_SENSIVITY_AT_FULLSCALE_4;
			break;
		case LIS3DSH_FULLSCALE_6:
			sensivity = LIS3DSH_SENSIVITY_AT_FULLSCALE_6;
			break;
		case LIS3DSH_FULLSCALE_8:
			sensivity = LIS3DSH_SENSIVITY_AT_FULLSCALE_8;
			break;
		case LIS3DSH_FULLSCALE_16:
			sensivity = LIS3DSH_SENSIVITY_AT_FULLSCALE_16;
			break;
		default:
			// Error
			Error_Handler();
			return;
	}
	/* Reag XYZ Registers */
	Accelero_IO_read(((uint8_t *)pDataOut + 0), LIS3DSH_REG_OUT_X_LOW, 1);
	Accelero_IO_read(((uint8_t *)pDataOut + 1), LIS3DSH_REG_OUT_X_HIGH, 1);
	Accelero_IO_read(((uint8_t *)pDataOut + 2), LIS3DSH_REG_OUT_Y_LOW, 1);
	Accelero_IO_read(((uint8_t *)pDataOut + 3), LIS3DSH_REG_OUT_Y_HIGH, 1);
	Accelero_IO_read(((uint8_t *)pDataOut + 4), LIS3DSH_REG_OUT_Z_LOW, 1);
	Accelero_IO_read(((uint8_t *)pDataOut + 5), LIS3DSH_REG_OUT_Z_HIGH, 1);

	size_t i = 0;
	for (i = 0; i < 3; ++i)
		{
			pDataOut[i] *= sensivity;
		}
}
