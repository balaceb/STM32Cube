/**
  @page TouchSensing 2 touchkey sensors example
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    TouchSensing\TouchSensing_2touchkeys\readme.txt
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the TouchSensing 2 touchkey sensors example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  @endverbatim

@par Description

This firmware is a basic example on how to use the STMTouch driver with 2 touchkey
sensors. The ECS and DTO are also used.

Observed behaviour:

- The LD1 is ON when the TS1 sensor is touched.
- The LD2 is ON when the TS2 sensor is touched.
- The LD4 blinks each time the ECS process is finished.
- The LD3 blinks continuously in case of error.

- On the LCD are also displayed:
  * The ECS state (ON/OFF).
  * The TS1 and TS2 touchkey sensors state (RELEASE, DETECT, ...) and their delta value.

- The ECS is ON when no touch is detected (all sensors are in the RELEASE state).
  The ECS is always present but its behavior can be modified using some parameters in
  the tsl_conf.h file.

- You can experiment the DTO by touching a sensor for few seconds. The RELEASE state
  is automatically entered and a re-calibration is performed after the timeout is reached.
  This timeout is programmable by the TSLPRM_DTO parameter in the tsl_conf.h file.

@par Project Directory contents

    - TouchSensing_2touchkeys\Inc\main.h                Header for main.c file
    - TouchSensing_2touchkeys\Inc\stm32f3xx_hal_conf.h  HAL Library configuration file
    - TouchSensing_2touchkeys\Inc\stm32f3xx_it.h        Header for stm32f3xx_it.c file
    - TouchSensing_2touchkeys\Src\stmCriticalSection.h  Header for stmCriticalSection.c file
    - TouchSensing_2touchkeys\Inc\tsl_conf.h            STMTouch driver configuration file
    - TouchSensing_2touchkeys\Inc\tsl_user.h            Header for tsl_user.c file

    - TouchSensing_2touchkeys\Src\main.c                Main program file
    - TouchSensing_2touchkeys\Src\stm32f3xx_hal_msp.c   HAL MSP module file
    - TouchSensing_2touchkeys\Src\stm32f3xx_it.c        Interrupt handlers file
    - TouchSensing_2touchkeys\Src\stmCriticalSection.c  STMStudio lock/unlock mechanism file
    - TouchSensing_2touchkeys\Src\system_stm32f3xx.c    System initialization file
    - TouchSensing_2touchkeys\Src\tsl_user.c            Touchsensing channels/banks description file
    
@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STM32303C-EVAL board. It can be
    easily tailored to any other devices that embed the TSC peripheral and to
    any other development board supporting touchsensing sensor.
     
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
