/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEnhanceEncoderOnBoard
 * \brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 * @file    MeEnhanceEncoderOnBoard.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/11/17
 * @brief   Header for MeEnhanceEncoderOnBoard.cpp module
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for Encoder On MeAuriga and MeMegaPi.
 *
 * \par Method List:
 *
 *    1. void MeEnhanceEncoderOnBoard::pwmMove(uint8_t slot);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Payton        2017/11/17     1.0.0            Build New
 * </pre>
 */

#ifndef ME_Enhance_Encoder_OnBoard_H
#define ME_Enhance_Encoder_OnBoard_H

#include <Arduino.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <MeEncoderOnBoard.h>

//motor once change pwm value
#define ONE_STEP_PWM    60

/**
 * Class: MeEnhanceEncoderOnBoard
 * \par Description
 * Declaration of Class MeEnhanceEncoderOnBoard
 */
class MeEnhanceEncoderOnBoard : public MeEncoderOnBoard
{
public:
/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *	 None
 */
	MeEnhanceEncoderOnBoard();

/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port
 * \param[in]
 *	 slot - megapi slot from SLOT1 to SLOT4(Auriga SLOT1 and SLOT2).
 */
	MeEnhanceEncoderOnBoard(uint8_t slot);

/**
 * \par Function
 *		setTarPWM
 * \par Description
 *		This function used for set the target pwm value.
 * \param[in]
 *		pwm_value - the target pwm value.
 * \par Output
 *		None
 * \par Return
 *		None
 * \par Others
 *		None
 */
void setTarPWM(int16_t pwm_value);

/**
 * \par Function
 *    pwmMove
 * \par Description
 *    This function used for move with PWM mode.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void pwmMove(void);

/**
 * \par Function
 *		loop
 * \par Description
 *		This function should be called without Blocked.
 * \param[in]
 *		None
 * \par Output
 *		None
 * \par Return
 *		None
 * \par Others
 *		None
 */
void loop(void);


private:
  uint8_t _mode;
  int16_t _targetPwm;
  int16_t _currentPwm;
  long _move_time;

};
#endif



