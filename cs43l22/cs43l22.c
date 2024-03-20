/*
 * cs43l22.c
 *
 *  Created on: Jan 19, 2024
 *      Author: AAD
 */
/* Includes */
#include "cs43l22.h"
#include "stm32f4_disc1_codec.h"

/*
 * @brief: Returns chipID Without offet at register
 * @note:	if /readwrite op fails, it calls Error Handler
 * @retval: chipId
 */
uint8_t
CS43L22_ReadChipID (void)
{
	uint8_t reg = 0x00;
	Codec_IO_read(&reg, CS43L22_REG_CHIP_ID_AND_REVISION, 0x01, 0x01);
	return (reg & CS43L22_CHIP_ID_MASK) >> 3;
}

/*
 * @brief: Returns chip revision id
 * @note:	if /readwrite op fails, it calls Error Handler
 * @retval: chip revision id
 */
uint8_t
CS43L22_ReadRevision (void)
{
	uint8_t reg = 0x00;
	Codec_IO_read(&reg, CS43L22_REG_CHIP_ID_AND_REVISION, 0x01, 0x01);
	return (reg & CS43L22_REVISION_MASK);
}

/*
 * @brief: Sets Chip to powerdown mode. This does not shutdowns the chip
 * @note:	if /readwrite op fails, it calls Error Handler
 * @retval: None
 */
void
CS43L22_PowerDown (void)
{
	uint8_t powerDown = CS43L22_PowerDown1;
	Codec_IO_write(&powerDown, CS43L22_REG_POWER_CONTROL1, 0x01, 0x01);
}

/*
 * @brief: Sets Chip to powerup mode
 * @note:	if /readwrite op fails, it calls Error Handler
 * @retval: None
 */
void
CS43L22_PowerUp (void)
{
	uint8_t powerUp = CS43L22_PowerUp1;
	Codec_IO_write(&powerUp, CS43L22_REG_POWER_CONTROL1, 0x01, 0x01);
}

/*
 * @brief: Initialises CS43L22 Chip
 * Sets Power Controls, output device and Interface
 * @note: this init works for just slave chips
 * @retval: None
 */
void
CS43L22_Init (CS43L22_InitTypeDef * pInit)
{
	/* Wake up The Chip */
	CS43L22_POWER_WAKEUP();
	/* Set Codec to power Down Mode */
	CS43L22_PowerDown();
	uint8_t reg = 0x00;
	/* Set Output Device, Speaker or Headphone */
	reg = pInit->OutputDevice;
	Codec_IO_write(&reg, CS43L22_REG_POWER_CONTROL2, 0x01, 0x01);
	/* Set Clock Auto, MasterClock Div 2 Audio Sampling rate
	 * 32K Group, Video  	Set 27Mhz
	 */
	reg = CS43L22_Clock_Auto; //| CS43L22_MCLK_Div2;
	// User
	reg |= pInit->SampleRate32KGroup;
	reg |= pInit->VideoClockSet27Mhz;
	Codec_IO_write(&reg, CS43L22_REG_CLOCK_CONTROL, 0x01, 0x01);
	/* Interface 1 */
	reg = 0x00;
	if (CS43L22_SlaveMode != pInit->MasterSlave)
		{
			Error_Handler();
		}
	Codec_IO_read(&reg, CS43L22_REG_INTERFACE_CONTROL1, 0x01, 0x01);
	// clear register 5th bit, which is reserved
	reg &= (0x01 << 5);
	reg |= pInit->MasterSlave;
	reg |= pInit->SCLKPolarity;
	reg |= pInit->DSPMode;
	reg |= pInit->DACInterface;
	reg |= pInit->AudioWordLength;
	Codec_IO_write(&reg, CS43L22_REG_INTERFACE_CONTROL1, 0x01, 0x01);
	/* PassThrough A */
	Codec_IO_read(&reg, CS43L22_REG_PASSTHROUGH_A, 0x01, 0x01);
	// clear reserved last 4 bits
	reg &= 0xF0;
	reg |= pInit->PassthroughA;
	Codec_IO_write(&reg, CS43L22_REG_PASSTHROUGH_A, 0x01, 0x01);
	/* PassThrough B */
	Codec_IO_read(&reg, CS43L22_REG_PASSTHROUGH_B, 0x01, 0x01);
	// clear reserved last 4 bits
	reg &= 0xF0;
	reg |= pInit->PassthroughB;
	Codec_IO_write(&reg, CS43L22_REG_PASSTHROUGH_B, 0x01, 0x01);
	/* Miscellaneous */
	reg = 0x00;
	reg |= pInit->Miscellaneous;
	Codec_IO_write(&reg, CS43L22_REG_MISCELLANEOUS_CONTROL, 0X01, 0X01);
	/* UnMute */
	reg = 0x00;
	Codec_IO_write(&reg, CS43L22_REG_PLAYBACK_CONTROL2, 0x01, 0x01);
	// Set volume to default 0dB
	reg = 0x00;
	Codec_IO_write(&reg, CS43L22_REG_PASSAVOL, 0x01, 0x01);
	Codec_IO_write(&reg, CS43L22_REG_PASSBVOL, 0x01, 0x01);
	Codec_IO_write(&reg, CS43L22_REG_PCMA, 0x01, 0x01);
	Codec_IO_write(&reg, CS43L22_REG_PCMB, 0x01, 0x01);
}

void
CS43L22_Start (void)
{
	uint8_t reg = 0x00;
	/* Set Required Init Settings */
	reg = 0x99;
	Codec_IO_write(&reg, 0x00, 0x01, 0x01);
	reg = 0x80;
	Codec_IO_write(&reg, 0x47, 0x01, 0x01);
	Codec_IO_read(&reg, 0x32, 0x01, 0x01);
	reg |= 0x80;
	Codec_IO_write(&reg, 0x32, 0x01, 0x01);
	Codec_IO_read(&reg, 0x32, 0x01, 0x01);
	reg &= ~(0x80);
	Codec_IO_write(&reg, 0x32, 0x01, 0x01);
	reg = 0x00;
	Codec_IO_write(&reg, 0x00, 0x01, 0x01);
	CS43L22_PowerUp();
}

void
CS43L22_SetVolume (uint8_t Volume)
{
	int8_t tempVol = Volume - 50;
	tempVol *= (127 / 5);
	Codec_IO_write((uint8_t)(tempVol), CS43L22_REG_PASSAVOL, 0x01, 0x01);
	Codec_IO_write((uint8_t)(tempVol), CS43L22_REG_PASSBVOL, 0x01, 0x01);

	tempVol = (uint8_t)(VOLUME_CONVERT_D(Volume));
	Codec_IO_write((uint8_t)(tempVol), CS43L22_REG_MSTA, 0x01, 0x01);
	Codec_IO_write((uint8_t)(tempVol), CS43L22_REG_MSTB, 0x01, 0x01);
}
