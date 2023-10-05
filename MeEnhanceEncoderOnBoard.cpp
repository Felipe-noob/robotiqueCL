/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEnhanceEncoderOnBoard
 * \brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 * @file    MeEnhanceEncoderOnBoard.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/11/17
 * @brief   Driver for Encoder module on MeAuriga and MeMegaPi.
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
 * Payton         2017/11/17     1.0.0            Add description
 * </pre>
 *
 */

#include "MeEnhanceEncoderOnBoard.h"

/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeEnhanceEncoderOnBoard::MeEnhanceEncoderOnBoard() : MeEncoderOnBoard(0)
{
  _mode = DIRECT_MODE;
  _targetPwm = 0;
  _currentPwm = 0;
}

/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port
 * \param[in]
 *   slot - megapi slot from SLOT1 to SLOT4(Auriga SLOT1 and SLOT2).
 */
MeEnhanceEncoderOnBoard::MeEnhanceEncoderOnBoard(uint8_t slot) : MeEncoderOnBoard(slot)
{
  _mode = DIRECT_MODE;
  _targetPwm = 0;
  _currentPwm = 0;
}

/**
 * \par Function
 *    setEnhanceTarPWM
 * \par Description
 *    This function used for set the target pwm value.
 * \param[in]
 *    pwm_value - the target pwm value.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEnhanceEncoderOnBoard::setTarPWM(int16_t pwm_value)
{
  _mode = PWM_MODE;
  _targetPwm = constrain(pwm_value,-255,255);
}

/**
 * \par Function
 *    enhancePwmMove
 * \par Description
 *    This function used for  move with PWM mode.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEnhanceEncoderOnBoard::pwmMove(void)
{
  _currentPwm = getCurPwm();
  if(_targetPwm > _currentPwm + ONE_STEP_PWM)
  {
    _currentPwm += ONE_STEP_PWM;
  }
  else if(_targetPwm < _currentPwm - ONE_STEP_PWM)
  {
    _currentPwm -= ONE_STEP_PWM;
  }
  else
  {
    _currentPwm = _targetPwm;
  }
}

/**
 * \par Function
 *    loop
 * \par Description
 *    This function should be called without Blocked.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEnhanceEncoderOnBoard::loop(void)
{
  updateCurPos();
  updateSpeed();

  if(millis() - _move_time > 5)
  {
    _move_time = millis();
    if(_mode == PWM_MODE)
    {
      pwmMove();
      setMotorPwm(_currentPwm);
    }
  }
}

