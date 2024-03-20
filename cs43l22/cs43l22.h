/*
 * cs43l22.h
 *
 *  Created on: Jan 19, 2024
 *      Author: AAD
 */

#ifndef COMPANENTS_CS43L22_CS43L22_H_
#define COMPANENTS_CS43L22_CS43L22_H_
/* Includes */
#include "main.h"
/* Extern */
/* Typedefs */
typedef struct CS43L22_Init_s
{
	/* POWER CONTROL 1 */
	uint8_t OutputDevice;
	/* Clock Control */
	uint8_t	SampleRate32KGroup;
	uint8_t	VideoClockSet27Mhz;
	/* Interface Control 1 */
	uint8_t MasterSlave;
	uint8_t	SCLKPolarity;
	uint8_t DSPMode;
	uint8_t DACInterface;
	uint8_t AudioWordLength;
	/* PassThrough X */
	uint8_t PassthroughA;
	uint8_t PassthroughB;
	/* Miscellaneous */
	uint8_t Miscellaneous;
}	CS43L22_InitTypeDef;

#define CS43L22_CHIP_ID 0b11100U
/*
 * 7. REGISTER DESCRIPTION
 * ReadOnly
 * 7.1 Chip I.D. and Revision Register (Address 01h) (Read Only)
 *    7          6          5          4          3       2        1        0
 *  CHIPID4    CHIPID3    CHIPID2    CHIPID1    CHIPID0  REVID2   REVID1   REVID0
 *
 * 7.1.1 Chip I.D. (Read Only)
 *        I.D. code for the CS43L22.
 *        CHIPID[4:0]   Device
 *        11100         CS43L22
 *
 * 7.1.2 Chip Revision (Read Only)
 *        CS43L22 revision level.
 *        REVID[2:0]    Revision Level
 *        000           A0
 *        001           A1
 *        010           B0
 *        011           B1
 */
#define CS43L22_REG_CHIP_ID_AND_REVISION	0x01
#define CS43L22_CHIP_ID_MASK	0xF8U
#define CS43L22_REVISION_MASK	0x07U

/*
 * 7.2 Power Control 1 (Address 02h)
 *   7      6      5      4      3      2      1      0
 * PDN7   PDN6   PDN5   PDN4   PDN3   PDN2   PDN1   PDN0
 *
 * 7.2.1 Power Down
 *        Configures the power state of the CS43L22.
 *        PDN[7:0]      Status
 *        0000 0001     Powered Down - same as setting 1001 1111
 *        1001 1110     Powered Up
 *        1001 1111     Powered Down - same as setting 0000 0001
 *
 *        Note:
 *        1. All states of PDN[7:0] not shown in the table are reserved.
 */
#define CS43L22_REG_POWER_CONTROL1	0x02
#define CS43L22_PowerDown1	0x01U
#define CS43L22_PowerDown2	0x9FU
#define CS43L22_PowerUp1			0x9EU

/*
 * Note: At stm32f407_Disc, Headhone is the way to go, check shematics
 * 7.3 Power Control 2 (Address 04h)
 *   7         6        5          4         3          2        1         0
 * PDN_HPB1  PDN_HPB0  PDN_HPA1   PDN_HPA0  PDN_SPKB1  PDN_SPKB0 PDN_SPKA1 PDN_SPKA0
 *
 * 7.3.1 Headphone Power Control
 *        Configures how the SPK/HP_SW pin, 6, controls the power for the headphone amplifier.
 *        PDN_HPx[1:0]   Headphone Status
 *        00             Headphone channel is ON when the SPK/HP_SW pin, 6, is LO.
 *                       Headphone channel is OFF when the SPK/HP_SW pin, 6, is HI.
 *        01             Headphone channel is ON when the SPK/HP_SW pin, 6, is HI.
 *                       Headphone channel is OFF when the SPK/HP_SW pin, 6, is LO.
 *        10             Headphone channel is always ON.
 *        11             Headphone channel is always OFF.
 *
 * 7.3.2 Speaker Power Control
 *        Configures how the SPK/HP_SW pin, 6, controls the power for the speaker amplifier.
 *        PDN_SPKx[1:0]  Speaker Status
 *        00             Speaker channel is ON when the SPK/HP_SW pin, 6, is LO.
 *                       Speaker channel is OFF when the SPK/HP_SW pin, 6, is HI.
 *        01             Speaker channel is ON when the SPK/HP_SW pin, 6, is HI.
 *                       Speaker channel is OFF when the SPK/HP_SW pin, 6, is LO.
 *        10             Speaker channel is always ON.
 *        11             Speaker channel is always OFF.
 */

#define CS43L22_Output_HeadphoneOn	0xA0U
#define CS43L22_Output_HeadphoneOff	0xF0U
#define CS43L22_Output_SpeakerOn	0x0AU
#define CS43L22_Output_SpeakerOff	0x0FU
#define CS43L22_Output_Both	(CS43L22_Output_HeadphoneOn \
														 | CS43L22_Output_SpeakerOn)
#define CS43L22_REG_POWER_CONTROL2	0x04

/*
 * 7.4 Clocking Control (Address 05h)
 *   7      6      5        4       3         2      1      0
 * AUTO  SPEED1  SPEED0  32k_GROUP  VIDEOCLK  RATIO1  RATIO0  MCLKDIV2
 *
 * 7.4.1 Auto-Detect
 *        Configures the auto-detect circuitry for detecting the speed mode of the CS43L22 when operating as a slave.
 *        AUTO       Auto-detection of Speed Mode
 *        0          Disabled
 *        1          Enabled
 *        Application: “Serial Port Clocking” on page 30
 *        Notes:
 *        1. The SPEED[1:0] bits are ignored, and speed is determined by the MCLK/LRCK ratio.
 *        2. When AUTO is disabled and the CS43L22 operates in Master Mode, the MCLKDIV2 bit is ignored.
 *        3. Certain sample and MCLK frequencies require setting the SPEED[1:0] bits, the 32k_GROUP bit, and/or
 *           the VIDEOCLK bit and RATIO[1:0] bits. Low sample rates may also affect dynamic range performance in the
 *           typical audio band. Refer to the referenced application for more information.
 *
 * 7.4.2 Speed Mode
 *        Configures the speed mode of the DAC in Slave Mode and sets the appropriate MCLK divide ratio for LRCK and
 *        SCLK in Master Mode.
 *        SPEED[1:0]      Slave Mode                Master Mode
 *        00              DSM (50 kHz - 100 kHz)   MCLK/LRCK Ratio: 512, SCLK/LRCK Ratio: 64
 *        01              SSM (4 kHz - 50 kHz)     MCLK/LRCK Ratio: 256, SCLK/LRCK Ratio: 64
 *        10              HSM (12.5 kHz - 25 kHz)  MCLK/LRCK Ratio: 128, SCLK/LRCK Ratio: 64
 *        11              QSM (4 kHz - 12.5 kHz)   MCLK/LRCK Ratio: 128, SCLK/LRCK Ratio: 64
 *        Application: “Serial Port Clocking” on page 30
 *        Notes:
 *        1. Slave/Master Mode is determined by the M/S bit in “Master/Slave Mode” on page 41.
 *        2. Certain sample and MCLK frequencies require setting the SPEED[1:0] bits, the 32k_GROUP bit, and/or the
 *           VIDEOCLK bit and RATIO[1:0] bits. Low sample rates may also affect dynamic range performance in the
 *           typical audio band. Refer to the referenced application for more information.
 *        3. These bits are ignored when the AUTO bit is enabled.
 *
 * 7.4.3 32kHz Sample Rate Group
 *        Specifies whether or not the input/output sample rate is 8 kHz, 16 kHz, or 32 kHz.
 *        32k_GROUP   8 kHz, 16 kHz, or 32 kHz sample rate?
 *        0           No
 *        1           Yes
 *        Application: “Serial Port Clocking” on page 30
 *
 * 7.4.4 27 MHz Video Clock
 *        Specifies whether or not the external MCLK frequency is 27 MHz.
 *        VIDEOCLK    27 MHz MCLK?
 *        0           No
 *        1           Yes
 *        Application: “Serial Port Clocking” on page 30
 *
 * 7.4.5 Internal MCLK/LRCK Ratio
 *        Configures the internal MCLK/LRCK ratio.
 *        RATIO[1:0]  Internal MCLK Cycles per LRCK    SCLK/LRCK Ratio in Master Mode
 *        00          128                              64
 *        01          125                              62
 *        10          132                              66
 *        11          136                              68
 *        Application: “Serial Port Clocking” on page 30
 *
 * 7.4.6 MCLK Divide By 2
 *        Divides the input MCLK by 2 prior to all internal circuitry.
 *        MCLKDIV2    MCLK signal into DAC
 *        0           No divide
 *        1           Divided by 2
 *        Application: “Serial Port Clocking” on page 30
 */
#define CS43L22_Clock_Auto	0x80U
/* Speed is Ignored when Auto Set */
#define CS43L22_MCLK_Div2	0x01U
#define CS43L22_32K_Group	0x10U	/* 8 16 32 */
#define CS43L22_Video_CLK	0x08U /* is Master Clock is set to 27 MHz */
#define CS43L22_REG_CLOCK_CONTROL	0x05

/*
 * 7.5 Interface Control 1 (Address 06h)
 *   7       6          5           4       3       2      1        0
 * M/S   INV_SCLK   Reserved     DSP    DACDIF1  DACDIF0   AWL1    AWL0
 *
 * 7.5.1 Master/Slave Mode
 *        Configures the serial port I/O clocking.
 *        M/S             Serial Port Clocks
 *        0               Slave (input ONLY)
 *        1               Master (output ONLY)
 *
 * 7.5.2 SCLK Polarity
 *        Configures the polarity of the SCLK signal.
 *        INV_SCLK        SCLK Polarity
 *        0               Not Inverted
 *        1               Inverted
 *
 * 7.5.3 DSP Mode
 *        Configures a data-packed interface format for the DAC.
 *        DSP             DSP Mode
 *        0               Disabled
 *        1               Enabled
 *        Application:    “DSP Mode” on page 32
 *        Notes:
 *        1. Select the audio word length using the AWL[1:0] bits (“Audio Word Length” on page 42).
 *        2. The interface format for the DAC must be set to “Left-Justified” when DSP Mode is enabled.
 *
 * 7.5.4 DAC Interface Format
 *        Configures the digital interface format for data on SDIN.
 *        DACDIF[1:0]      DAC Interface Format
 *        00              Left Justified, up to 24-bit data
 *        01              I²S, up to 24-bit data
 *        10              Right Justified
 *        11              Reserved
 *        Application:    “Digital Interface Formats” on page 31
 *        Note: Select the audio word length for Right Justified using the AWL[1:0] bits (“Audio Word Length” on
 *        page 42).
 *
 * 7.5.5 Audio Word Length
 *        Configures the audio sample word length used for the data into SDIN.
 *        AWL[1:0]        Audio Word Length
 *        00              32-bit data (DSP Mode) / 24-bit data (Right Justified)
 *        01              24-bit data (DSP Mode) / 20-bit data (Right Justified)
 *        10              20-bit data (DSP Mode) / 18-bit data (Right Justified)
 *        11              16-bit data (DSP Mode) / 16-bit data (Right Justified)
 *        Application:    “DSP Mode” on page 32
 *        Note: When the internal MCLK/LRCK ratio is set to 125 in Master Mode, the 32-bit data width option for DSP Mode
 *        is not valid unless SCLK=MCLK.
 */
#define CS43L22_SlaveMode	0x00U
#define CS43L22_MasterMode	0x80U
#define CS43L22_InvertedSCLK	0x40U
#define CS43L22_DSP_Mode	0x10U
#define CS43L22_DACInterface_LeftJustified24Bit	0x00U
#define CS43L22_DACInterface_I2S_24Bit	0x04U
#define CS43L22_DACInterface_RightJustified	0x08U
#define CS43L22_AudioWordLength_32Bit_DSP	0x00U
#define CS43L22_AudioWordLength_24Bit_RightJustified	0x00U
#define CS43L22_AudioWordLength_24Bit_DSP	0x01U
#define CS43L22_AudioWordLength_20BitRightJustified	0x01U
#define CS43L22_AudioWordLength_20Bit_DSP	0x02U
#define CS43L22_AudioWordLength_18Bit_RightJustified	0x02U
#define CS43L22_AudioWordLength_16Bit_DSP	0x03U
#define CS43L22_AudioWordLength_16Bit_RightJustified	0x03U

#define	CS43L22_REG_INTERFACE_CONTROL1	0x06


/*
 * 7.7 Passthrough x Select: PassA (Address 08h), PassB (Address 09h)
 *   7        6          5           4         3        2         1         0
 * Reserved Reserved  Reserved  Reserved  PASSASEL4 PASSASEL3 PASSASEL2 PASSASEL1
 *
 * 7.7.1 Passthrough Input Channel Mapping
 *        Selects one or sums/mixes the analog input signal into the passthrough Amplifier. Each bit of the
 *        PASSx_SEL[4:1] word corresponds to individual channels (i.e. PASSx_SEL1 selects AIN1x,
 *        PASSx_SEL2 selects AIN2x, etc.).
 *        PASSxSEL[4:1]      Selected Input to Passthrough Channelx
 *        00000              No inputs selected
 *        00001              AIN1x
 *        00010              AIN2x
 *        00100              AIN3x
 *        01000              AIN4x
 *        Application:       “Analog Passthrough” on page 25
 *        Note: Table does not show all possible combinations.
 */
#define CS43L22_Passthrough_NONE 		0x00U
#define CS43L22_Passthrough_AIN1_X	0x01U
#define CS43L22_Passthrough_AIN2_X	0x02U
#define CS43L22_Passthrough_AIN3_X	0x04U
#define CS43L22_Passthrough_AIN4_X	0x08U

#define CS43L22_REG_PASSTHROUGH_A	0x08
#define CS43L22_REG_PASSTHROUGH_B	0x09

/*
 * 7.11 Miscellaneous Controls (Address 0Eh)
 *   7        6         5         4                       3              2               1               0
 * PASSTHRUB PASSTHRUA PASSBMUTE PASSAMUTE             FREEZE         DEEMPH         DIGSFT           DIGZC
 *
 * 7.11.1 Passthrough Analog
 *        Configures an analog passthrough from the analog inputs to the headphone/line outputs.
 *        PASSTHRUx       Analog In Routed to HP/Line Output
 *        0               Disabled
 *        1               Enabled
 *
 * 7.11.2 Passthrough Mute
 *        Configures an analog mute on the channel x analog in to analog out passthrough.
 *        PASSxMUTE       Passthrough Mute
 *        0               Disabled
 *        1               Enabled
 *
 * 7.11.3 Freeze Registers
 *        Configures a hold on all register settings.
 *        FREEZE          Control Port Status
 *        0               Register changes take effect immediately
 *
 *        1               Modifications may be made to all control port registers without the changes taking effect until after the
 *                        FREEZE is disabled.
 *
 * 7.11.4 HP/Speaker De-Emphasis
 *        Configures a 15µs/50µs digital de-emphasis filter response on the headphone/line and speaker outputs.
 *        DEEMPHASIS      Control Port Status
 *        0               Disabled
 *        1               Enabled
 *
 * 7.11.5 Digital Soft Ramp
 *        Configures an incremental volume ramp from the current level to the new level at the specified rate.
 *        DIGSFT          Volume Changes                  Affected Digital Volume Controls
 *        0               Does not occur with a soft ramp  MSTxMUTE, HPxMUTE, SPKxMUTE, PCMxMUTE, PCMxVOL[7:0]
 *                                                             MSTxVOL[7:0], HPxVOL[7:0], SPKxVOL[7:0]
 *        1               Occurs with a soft ramp          MSTxVOL[7:0], HPxVOL[7:0], SPKxVOL[7:0]
 *        Ramp Rate:      1/8 dB every LRCK cycle
 *
 * 7.11.6 Digital Zero Cross
 *        Configures when the signal level changes occur for the digital volume controls.
 *        DIGZC            Volume Changes                  Affected Digital Volume Controls
 *                          Do not occur on a zero cross-   MSTxMUTE, HPxMUTE, SPKxMUTE, PCMxMUTE, PCMxVOL[7:0]
 *        0
 *                          ing                              MSTxVOL[7:0], HPxVOL[7:0], SPKxVOL[7:0]
 *        1                 Occur on a zero crossing       HPxVOL[7:0], SPKxVOL[7:0]
 *
 *        Notes:
 *        1. If the signal does not encounter a zero crossing, the requested volume change will occur after a
 *           timeout period between 1024 and 2048 sample periods (21.3 ms to 42.7 ms at 48 kHz sample rate).
 *        2. The zero cross function is independently monitored and implemented for each channel.
 *        3. The DIS_LIMSFT bit is ignored when zero cross is enabled.
 */

#define CS43L22_Passthrough_Analog_Enable_A		0x40U
#define CS43L22_Passthrough_Analog_Disable_A	0x00U
#define CS43L22_Passthrough_Analog_Mute_A			0x10U
#define CS43L22_Passthrough_Analog_Enable_B		0x80U
#define CS43L22_Passthrough_Analog_Disable_B	0x00U
#define CS43L22_Passthrough_Analog_Mute_B			0x20U
#define CS43L22_Freeze_Registers							0x08U
#define	CS43L22_DeEmphasis										0x04U
#define CS43L22_Digital_Soft_Ramp							0x02U
#define CS43L22_Digital_Zero_Cross						0x01U

#define CS43L22_REG_MISCELLANEOUS_CONTROL			0x0E

/*
 * 7.12 Playback Control 2 (Address 0Fh)
 *   7                 6              5                4                 3              2              1               0
 * HPBMUTE          HPAMUTE        SPKBMUTE        SPKAMUTE         SPKB=A        SPKSWAP       SPKMONO        MUTE50/50
 *
 * 7.12.1 Headphone Mute
 *        Configures a digital mute on headphone channel x.
 *        HPxMUTE          Headphone Mute
 *        0                Disabled
 *        1                Enabled
 *
 * 7.12.2 Speaker Mute
 *        Configures a digital mute on speaker channel x.
 *        SPKxMUTE         Speaker Mute
 *        0                Disabled
 *        1                Enabled
 *
 * 7.12.3 Speaker Volume Setting B=A
 *        Configures independent or ganged volume control of the speaker volume. Mute is not affected.
 *        SPKB=A           Single Volume Control for the Speaker Channel
 *        0                Disabled
 *        1                Enabled
 *
 * 7.12.4 Speaker Channel Swap
 *        Configures a channel swap on the speaker channels.
 *        SPKSWAP          Speaker Output
 *        0                Channel A
 *        1                Channel B
 *        Application:     “Mono Speaker Output Configuration” on page 28
 *
 * 7.12.5 Speaker MONO Control
 *        Configures a parallel full bridge output for the speaker channels.
 *        SPKMONO          Parallel Full Bridge Output
 *        0                Disabled
 *        1                Enabled
 *        Application:     “Mono Speaker Output Configuration” on page 28
 *
 * 7.12.6 Speaker Mute 50/50 Control
 *        Configures how the speaker channels mute.
 *        MUTE50/50        Speaker Mute 50/50
 *        0                Disabled; The PWM amplifiers outputs modulated silence when SPKxMUTE is enabled.
 *                         Enabled; The PWM amplifiers switch at an exact 50%-duty-cycle signal (not modulated) when SPKxMUTE is
 *        1
 *                         enabled.
 */
#define CS43L22_HeadphoneB_Mute				0x80U
#define CS43L22_HeadphoneA_Mute				0x40U
#define CS43L22_SpeakerB_Mute					0x20U
#define CS43L22_SpeakerA_Mute					0x10U
#define CS43L22_Speaker_Volume_Single	0x08U
#define CS43L22_Speaker_Channel_Swap	0x04U
#define CS43L22_Speaker_Mono_Control	0x02U
#define CS43L22_Speaker_Mute_50_50		0x01U

#define CS43L22_REG_PLAYBACK_CONTROL2	0x0F

/*
 * 7.13 Passthrough x Volume: PASSAVOL (Address 14h) & PASSBVOL (Address 15h)
 *    7              6                5                4             3               2                1                0
 *  PASSxVOL7      PASSxVOL6        PASSxVOL5        PASSxVOL4     PASSxVOL3       PASSxVOL2        PASSxVOL1        PASSxVOL0
 *
 * 7.13.1 Passthrough x Volume
 *        Sets the volume/gain of the analog input signal routed to the headphone/line output.
 *        PASSxVOL[7:0]    Gain
 *        0111 1111        12 dB
 *        ...              ...
 *        0001 1000        12 dB
 *        ...              ...
 *        0000 0001        +0.5 dB
 *        0000 0000        0 dB
 *        11111 1111       -0.5 dB
 *        ...              ...
 *        1000 1000        -60.0 dB
 *        ...              ...
 *        1000 0000        -60.0 dB
 *        Step Size:       0.5 dB (approximate)
 *        Application:     “Passthrough Analog” on page 45
 *
 *        Notes:
 *        1. This register is ignored when the PASSTHRUx bit (“Passthrough Analog” on page 45) is disabled.
 *        2. The step size may deviate from 0.5 dB at settings below -40 dB. Code settings 0x95, 0xA1, 0xAD and
 *           0xB9 are not guaranteed to be monotonic.
 */

#define CS43L22_REG_PASSAVOL	0x14
#define CS43L22_REG_PASSBVOL	0x15

/*
 * 7.14 PCMx Volume: PCMA (Address 1Ah) & PCMB (Address 1Bh)
 *      7                 6                  5             4                  3               2          1          0
 * PCMxMUTE           PCMxVOL6           PCMxVOL5      PCMxVOL4           PCMxVOL3        PCMxVOL2   PCMxVOL1   PCMxVOL0
 *
 * 7.14.1 PCM Channel x Mute
 *         Configures a digital mute on the PCM data from the serial data input (SDIN) to the DSP.
 *             PCMxMUTE           PCM Mute
 *             0                  Disabled
 *             1                  Enabled
 *
 * 7.14.2 PCM Channel x Volume
 *         Sets the volume/gain of the PCM data from the serial data input (SDIN) to the DSP.
 *             PCMxVOL[6:0]       Volume
 *             001 1000           +12.0 dB
 *             ...                ...
 *             000 0001           +0.5 dB
 *             000 0000           0 dB
 *             111 1111           -0.5 dB
 *             ...                ...
 *             001 1001           -51.5 dB
 *             Step Size:         0.5 dB
 */
#define CS43L22_Pcm_Mute	0x80U
#define CS43L22_REG_PCMA	0x1A
#define CS43L22_REG_PCMB	0x1B

/*
 * 7.19 Master Volume Control: MSTA (Address 20h) & MSTB (Address 21h)
 *      7                6                 5           4          3          2          1          0
 * MSTxVOL7         MSTxVOL6          MSTxVOL5    MSTxVOL4   MSTxVOL3   MSTxVOL2   MSTxVOL1   MSTxVOL0
 *
 * 7.19.1 Master Volume Control
 *          Sets the volume of the signal out the DSP.
 *           MSTxVOL[7:0]      Master Volume
 *           0001 1000         +12.0 dB
 *           ···               ···
 *           0000 0000         0 dB
 *           1111 1111         -0.5 dB
 *           1111 1110         -1.0 dB
 *           ···               ···
 *           0011 0100         -102 dB
 *           ···               ···
 *           0001 1001         -102 dB
 *           Step Size:        0.5 dB
 */
#define CS43L22_REG_MSTA	0x20
#define CS43L22_REG_MSTB	0x21


/* Function Like Macros */
#define VOLUME_CONVERT_A(Volume)    (((Volume) > 100)? (255):	\
		((uint8_t)(((Volume) * 255) / 100)))
#define VOLUME_CONVERT_D(Volume)    (((Volume) > 100)? (24):	\
		((uint8_t)((((Volume) * 48) / 100) - 24)))


/* Functions */
uint8_t CS43L22_ReadChipID (void);
uint8_t CS43L22_ReadRevision (void);
void CS43L22_PowerDown (void);
void CS43L22_PowerUp (void);
void CS43L22_Init (CS43L22_InitTypeDef * pInit);
void CS43L22_Start (void);
void CS43L22_SetVolume (uint8_t Volume);
/* Board IO Functions */


#endif /* COMPANENTS_CS43L22_CS43L22_H_ */
