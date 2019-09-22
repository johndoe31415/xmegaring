/* Automatically generated HAL from hal.xml */
/* NEVER EDIT MANUALLY */

/* Generated on: 2019-09-17 20:51:37 */

#ifndef __HAL_H__
#define __HAL_H__

#include <avr/io.h>

#define nop()                                    __asm__ __volatile__("nop")

/* DebugRX -> PE2 (Input, Initially Pullup On) */
#define DebugRX_BIT                              2
#define DebugRX_PIN                              PORTE.IN
#define DebugRX_PORT                             PORTE.OUT
#define DebugRX_DDR                              PORTE.DIR
#define DebugRX_SetPullupActive()                DebugRX_PORT |= _BV(DebugRX_BIT)
#define DebugRX_SetPullupInactive()              DebugRX_PORT &= ~_BV(DebugRX_BIT)
#define DebugRX_ModeInput()                      DebugRX_DDR &= ~_BV(DebugRX_BIT)
#define DebugRX_IsInput()                        ((DebugRX_DDR & _BV(DebugRX_BIT)) == 0)
#define DebugRX_Get()                            (DebugRX_PIN & _BV(DebugRX_BIT))
#define DebugRX_GetBit()                         (DebugRX_Get() >> DebugRX_BIT)
#define DebugRX_IsInactive()                     (DebugRX_Get() == 0)
#define DebugRX_IsActive()                       (DebugRX_Get() != 0)
#define DebugRX_Init()                           { DebugRX_SetPullupActive(); DebugRX_ModeInput(); }

/* DebugTX -> PE3 (Output, Initially Inactive) */
#define DebugTX_BIT                              3
#define DebugTX_PIN                              PORTE.IN
#define DebugTX_PORT                             PORTE.OUT
#define DebugTX_DDR                              PORTE.DIR
#define DebugTX_ModeOutput()                     DebugTX_DDR |= _BV(DebugTX_BIT)
#define DebugTX_IsOutput()                       ((DebugTX_DDR & _BV(DebugTX_BIT)) != 0)
#define DebugTX_SetHIGH()                        DebugTX_PORT |= _BV(DebugTX_BIT)
#define DebugTX_SetLOW()                         DebugTX_PORT &= ~_BV(DebugTX_BIT)
#define DebugTX_Get()                            (DebugTX_PIN & _BV(DebugTX_BIT))
#define DebugTX_SetInactive()                    DebugTX_SetLOW()
#define DebugTX_SetActive()                      DebugTX_SetHIGH()
#define DebugTX_Toggle()                         DebugTX_PORT ^= _BV(DebugTX_BIT)
#define DebugTX_SetConditional(condition)        if (condition) DebugTX_SetActive(); else DebugTX_SetInactive()
#define DebugTX_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { DebugTX_SetActive(); } else if (conditionoff) { DebugTX_SetInactive(); } else if (conditiontoggle) { DebugTX_Toggle(); }
#define DebugTX_Pulse()                          { DebugTX_SetActive(); DebugTX_SetInactive(); }
#define DebugTX_PulseNop()                       { DebugTX_SetActive(); nop(); DebugTX_SetInactive(); }
#define DebugTX_IsInactive()                     (DebugTX_Get() == 0)
#define DebugTX_IsActive()                       (DebugTX_Get() != 0)
#define DebugTX_Init()                           { DebugTX_SetInactive(); DebugTX_ModeOutput(); }

/* SCK -> PD7 (Output, Initially Inactive, Active-Low) */
#define SCK_BIT                                  7
#define SCK_PIN                                  PORTD.IN
#define SCK_PORT                                 PORTD.OUT
#define SCK_DDR                                  PORTD.DIR
#define SCK_ModeOutput()                         SCK_DDR |= _BV(SCK_BIT)
#define SCK_IsOutput()                           ((SCK_DDR & _BV(SCK_BIT)) != 0)
#define SCK_SetHIGH()                            SCK_PORT |= _BV(SCK_BIT)
#define SCK_SetLOW()                             SCK_PORT &= ~_BV(SCK_BIT)
#define SCK_Get()                                (SCK_PIN & _BV(SCK_BIT))
#define SCK_SetInactive()                        SCK_SetHIGH()
#define SCK_SetActive()                          SCK_SetLOW()
#define SCK_Toggle()                             SCK_PORT ^= _BV(SCK_BIT)
#define SCK_SetConditional(condition)            if (condition) SCK_SetActive(); else SCK_SetInactive()
#define SCK_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { SCK_SetActive(); } else if (conditionoff) { SCK_SetInactive(); } else if (conditiontoggle) { SCK_Toggle(); }
#define SCK_Pulse()                              { SCK_SetActive(); SCK_SetInactive(); }
#define SCK_PulseNop()                           { SCK_SetActive(); nop(); SCK_SetInactive(); }
#define SCK_IsInactive()                         (SCK_Get() != 0)
#define SCK_IsActive()                           (SCK_Get() == 0)
#define SCK_Init()                               { SCK_SetInactive(); SCK_ModeOutput(); }

/* MOSI -> PD5 (Output, Initially Inactive) */
#define MOSI_BIT                                 5
#define MOSI_PIN                                 PORTD.IN
#define MOSI_PORT                                PORTD.OUT
#define MOSI_DDR                                 PORTD.DIR
#define MOSI_ModeOutput()                        MOSI_DDR |= _BV(MOSI_BIT)
#define MOSI_IsOutput()                          ((MOSI_DDR & _BV(MOSI_BIT)) != 0)
#define MOSI_SetHIGH()                           MOSI_PORT |= _BV(MOSI_BIT)
#define MOSI_SetLOW()                            MOSI_PORT &= ~_BV(MOSI_BIT)
#define MOSI_Get()                               (MOSI_PIN & _BV(MOSI_BIT))
#define MOSI_SetInactive()                       MOSI_SetLOW()
#define MOSI_SetActive()                         MOSI_SetHIGH()
#define MOSI_Toggle()                            MOSI_PORT ^= _BV(MOSI_BIT)
#define MOSI_SetConditional(condition)           if (condition) MOSI_SetActive(); else MOSI_SetInactive()
#define MOSI_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { MOSI_SetActive(); } else if (conditionoff) { MOSI_SetInactive(); } else if (conditiontoggle) { MOSI_Toggle(); }
#define MOSI_Pulse()                             { MOSI_SetActive(); MOSI_SetInactive(); }
#define MOSI_PulseNop()                          { MOSI_SetActive(); nop(); MOSI_SetInactive(); }
#define MOSI_IsInactive()                        (MOSI_Get() == 0)
#define MOSI_IsActive()                          (MOSI_Get() != 0)
#define MOSI_Init()                              { MOSI_SetInactive(); MOSI_ModeOutput(); }

/* CS -> PD4 (Output, Initially Inactive, Active-Low) */
#define CS_BIT                                   4
#define CS_PIN                                   PORTD.IN
#define CS_PORT                                  PORTD.OUT
#define CS_DDR                                   PORTD.DIR
#define CS_ModeOutput()                          CS_DDR |= _BV(CS_BIT)
#define CS_IsOutput()                            ((CS_DDR & _BV(CS_BIT)) != 0)
#define CS_SetHIGH()                             CS_PORT |= _BV(CS_BIT)
#define CS_SetLOW()                              CS_PORT &= ~_BV(CS_BIT)
#define CS_Get()                                 (CS_PIN & _BV(CS_BIT))
#define CS_SetInactive()                         CS_SetHIGH()
#define CS_SetActive()                           CS_SetLOW()
#define CS_Toggle()                              CS_PORT ^= _BV(CS_BIT)
#define CS_SetConditional(condition)             if (condition) CS_SetActive(); else CS_SetInactive()
#define CS_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { CS_SetActive(); } else if (conditionoff) { CS_SetInactive(); } else if (conditiontoggle) { CS_Toggle(); }
#define CS_Pulse()                               { CS_SetActive(); CS_SetInactive(); }
#define CS_PulseNop()                            { CS_SetActive(); nop(); CS_SetInactive(); }
#define CS_IsInactive()                          (CS_Get() != 0)
#define CS_IsActive()                            (CS_Get() == 0)
#define CS_Init()                                { CS_SetInactive(); CS_ModeOutput(); }

/* LEDRingBrightness -> PD1 (Output, Initially Inactive) */
#define LEDRingBrightness_BIT                    1
#define LEDRingBrightness_PIN                    PORTD.IN
#define LEDRingBrightness_PORT                   PORTD.OUT
#define LEDRingBrightness_DDR                    PORTD.DIR
#define LEDRingBrightness_ModeOutput()           LEDRingBrightness_DDR |= _BV(LEDRingBrightness_BIT)
#define LEDRingBrightness_IsOutput()             ((LEDRingBrightness_DDR & _BV(LEDRingBrightness_BIT)) != 0)
#define LEDRingBrightness_SetHIGH()              LEDRingBrightness_PORT |= _BV(LEDRingBrightness_BIT)
#define LEDRingBrightness_SetLOW()               LEDRingBrightness_PORT &= ~_BV(LEDRingBrightness_BIT)
#define LEDRingBrightness_Get()                  (LEDRingBrightness_PIN & _BV(LEDRingBrightness_BIT))
#define LEDRingBrightness_SetInactive()          LEDRingBrightness_SetLOW()
#define LEDRingBrightness_SetActive()            LEDRingBrightness_SetHIGH()
#define LEDRingBrightness_Toggle()               LEDRingBrightness_PORT ^= _BV(LEDRingBrightness_BIT)
#define LEDRingBrightness_SetConditional(condition) if (condition) LEDRingBrightness_SetActive(); else LEDRingBrightness_SetInactive()
#define LEDRingBrightness_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { LEDRingBrightness_SetActive(); } else if (conditionoff) { LEDRingBrightness_SetInactive(); } else if (conditiontoggle) { LEDRingBrightness_Toggle(); }
#define LEDRingBrightness_Pulse()                { LEDRingBrightness_SetActive(); LEDRingBrightness_SetInactive(); }
#define LEDRingBrightness_PulseNop()             { LEDRingBrightness_SetActive(); nop(); LEDRingBrightness_SetInactive(); }
#define LEDRingBrightness_IsInactive()           (LEDRingBrightness_Get() == 0)
#define LEDRingBrightness_IsActive()             (LEDRingBrightness_Get() != 0)
#define LEDRingBrightness_Init()                 { LEDRingBrightness_SetInactive(); LEDRingBrightness_ModeOutput(); }

/* LEDRing -> PD0 (Output, Initially Inactive) */
#define LEDRing_BIT                              0
#define LEDRing_PIN                              PORTD.IN
#define LEDRing_PORT                             PORTD.OUT
#define LEDRing_DDR                              PORTD.DIR
#define LEDRing_ModeOutput()                     LEDRing_DDR |= _BV(LEDRing_BIT)
#define LEDRing_IsOutput()                       ((LEDRing_DDR & _BV(LEDRing_BIT)) != 0)
#define LEDRing_SetHIGH()                        LEDRing_PORT |= _BV(LEDRing_BIT)
#define LEDRing_SetLOW()                         LEDRing_PORT &= ~_BV(LEDRing_BIT)
#define LEDRing_Get()                            (LEDRing_PIN & _BV(LEDRing_BIT))
#define LEDRing_SetInactive()                    LEDRing_SetLOW()
#define LEDRing_SetActive()                      LEDRing_SetHIGH()
#define LEDRing_Toggle()                         LEDRing_PORT ^= _BV(LEDRing_BIT)
#define LEDRing_SetConditional(condition)        if (condition) LEDRing_SetActive(); else LEDRing_SetInactive()
#define LEDRing_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { LEDRing_SetActive(); } else if (conditionoff) { LEDRing_SetInactive(); } else if (conditiontoggle) { LEDRing_Toggle(); }
#define LEDRing_Pulse()                          { LEDRing_SetActive(); LEDRing_SetInactive(); }
#define LEDRing_PulseNop()                       { LEDRing_SetActive(); nop(); LEDRing_SetInactive(); }
#define LEDRing_IsInactive()                     (LEDRing_Get() == 0)
#define LEDRing_IsActive()                       (LEDRing_Get() != 0)
#define LEDRing_Init()                           { LEDRing_SetInactive(); LEDRing_ModeOutput(); }

/* DebugLED -> PR0 (Output, Initially Inactive, Active-Low) */
#define DebugLED_BIT                             0
#define DebugLED_PIN                             PORTR.IN
#define DebugLED_PORT                            PORTR.OUT
#define DebugLED_DDR                             PORTR.DIR
#define DebugLED_ModeOutput()                    DebugLED_DDR |= _BV(DebugLED_BIT)
#define DebugLED_IsOutput()                      ((DebugLED_DDR & _BV(DebugLED_BIT)) != 0)
#define DebugLED_SetHIGH()                       DebugLED_PORT |= _BV(DebugLED_BIT)
#define DebugLED_SetLOW()                        DebugLED_PORT &= ~_BV(DebugLED_BIT)
#define DebugLED_Get()                           (DebugLED_PIN & _BV(DebugLED_BIT))
#define DebugLED_SetInactive()                   DebugLED_SetHIGH()
#define DebugLED_SetActive()                     DebugLED_SetLOW()
#define DebugLED_Toggle()                        DebugLED_PORT ^= _BV(DebugLED_BIT)
#define DebugLED_SetConditional(condition)       if (condition) DebugLED_SetActive(); else DebugLED_SetInactive()
#define DebugLED_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { DebugLED_SetActive(); } else if (conditionoff) { DebugLED_SetInactive(); } else if (conditiontoggle) { DebugLED_Toggle(); }
#define DebugLED_Pulse()                         { DebugLED_SetActive(); DebugLED_SetInactive(); }
#define DebugLED_PulseNop()                      { DebugLED_SetActive(); nop(); DebugLED_SetInactive(); }
#define DebugLED_IsInactive()                    (DebugLED_Get() != 0)
#define DebugLED_IsActive()                      (DebugLED_Get() == 0)
#define DebugLED_Init()                          { DebugLED_SetInactive(); DebugLED_ModeOutput(); }

#define initHAL() {\
		DebugRX_Init();\
		DebugTX_Init();\
		SCK_Init();\
		MOSI_Init();\
		CS_Init();\
		LEDRingBrightness_Init();\
		LEDRing_Init();\
		DebugLED_Init();\
}

#endif
