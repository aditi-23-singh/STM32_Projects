#ifndef TMC2208_REGISTERS_H
#define TMC2208_REGISTERS_H

#include <stdint.h>
#include <stdbool.h>
#include "TMC2208_Driver.h"

#define TMC2208_GCONF_I_SCALE_ANALOG_BIT      0
#define TMC2208_GCONF_INTERNAL_RSENSE_BIT     1
#define TMC2208_GCONF_EN_SPREADCYCLE_BIT      2
#define TMC2208_GCONF_SHAFT_BIT               3
#define TMC2208_GCONF_INDEX_OTPW_BIT          4
#define TMC2208_GCONF_INDEX_STEP_BIT          5
#define TMC2208_GCONF_PDN_DISABLE_BIT         6
#define TMC2208_GCONF_MSTEP_REG_SELECT_BIT    7
#define TMC2208_GCONF_MULTISTEP_FILT_BIT      8
#define TMC2208_GCONF_TEST_MODE_BIT           9


#define TMC2208_GCONF_I_SCALE_ANALOG_MASK     (1UL << 0)
#define TMC2208_GCONF_INTERNAL_RSENSE_MASK    (1UL << 1)
#define TMC2208_GCONF_EN_SPREADCYCLE_MASK     (1UL << 2)
#define TMC2208_GCONF_SHAFT_MASK              (1UL << 3)
#define TMC2208_GCONF_INDEX_OTPW_MASK         (1UL << 4)
#define TMC2208_GCONF_INDEX_STEP_MASK         (1UL << 5)
#define TMC2208_GCONF_PDN_DISABLE_MASK        (1UL << 6)
#define TMC2208_GCONF_MSTEP_REG_SELECT_MASK   (1UL << 7)
#define TMC2208_GCONF_MULTISTEP_FILT_MASK     (1UL << 8)
#define TMC2208_GCONF_TEST_MODE_MASK          (1UL << 9)


typedef struct {
    bool i_scale_analog;
    bool internal_rsense;
    bool en_spreadcycle;
    bool shaft;
    bool index_otpw;
    bool index_step;
    bool pdn_disable;
    bool mstep_reg_select;
    bool multistep_filt;
    bool test_mode;
    uint32_t raw_value;
} TMC2208_GCONF_t;



#define TMC2208_IOIN_ENN_BIT        0
#define TMC2208_IOIN_PDN_UART_BIT   1
#define TMC2208_IOIN_MS1_BIT        2
#define TMC2208_IOIN_MS2_BIT        3
#define TMC2208_IOIN_DIAG_BIT       4
#define TMC2208_IOIN_STEP_222X_BIT  5
#define TMC2208_IOIN_PDN_UART2_BIT  6
#define TMC2208_IOIN_STEP_BIT       7
#define TMC2208_IOIN_SEL_A_BIT      8
#define TMC2208_IOIN_DIR_BIT        9
#define TMC2208_IOIN_VERSION_SHIFT  24


#define TMC2208_IOIN_ENN_MASK       (1UL << 0)
#define TMC2208_IOIN_PDN_UART_MASK  (1UL << 1)
#define TMC2208_IOIN_MS1_MASK       (1UL << 2)
#define TMC2208_IOIN_MS2_MASK       (1UL << 3)
#define TMC2208_IOIN_DIAG_MASK      (1UL << 4)
#define TMC2208_IOIN_STEP_222X_MASK (1UL << 5)
#define TMC2208_IOIN_PDN_UART2_MASK (1UL << 6)
#define TMC2208_IOIN_STEP_MASK      (1UL << 7)
#define TMC2208_IOIN_SEL_A_MASK     (1UL << 8)
#define TMC2208_IOIN_DIR_MASK       (1UL << 9)
#define TMC2208_IOIN_VERSION_MASK   0xFF000000UL


typedef struct {
    bool     enn;
    bool     pdn_uart;
    bool     ms1;
    bool     ms2;
    bool     diag;
    bool     step_222x;
    bool     pdn_uart_bit6;
    bool     step;
    bool     sel_a;
    bool     dir;
    uint8_t  version;
    uint32_t raw_value;
} TMC2208_IOIN_t;


#define TMC2208_IHOLD_MASK          0x0000001FUL
#define TMC2208_IRUN_MASK           0x00001F00UL
#define TMC2208_IHOLDDELAY_MASK     0x000F0000UL

#define TMC2208_IHOLD_SHIFT         0
#define TMC2208_IRUN_SHIFT          8
#define TMC2208_IHOLDDELAY_SHIFT    16


typedef struct {
    uint8_t  ihold;
    uint8_t  irun;
    uint8_t  iholddelay;
    uint32_t raw_value;
} TMC2208_IHOLD_IRUN_t;



bool TMC2208_ReadGCONF(TMC2208_t *driver, TMC2208_GCONF_t *gconf);
bool TMC2208_ReadGCONF_Raw(TMC2208_t *driver, uint32_t *value);

void TMC2208_WriteGCONF(TMC2208_t *driver, const TMC2208_GCONF_t *gconf);
void TMC2208_WriteGCONF_Raw(TMC2208_t *driver, uint32_t value);

void TMC2208_SetIScaleAnalog(TMC2208_t *driver, bool enable);
void TMC2208_SetInternalRsense(TMC2208_t *driver, bool enable);
void TMC2208_SetSpreadCycle(TMC2208_t *driver, bool enable);
void TMC2208_SetShaft(TMC2208_t *driver, bool inverse);
void TMC2208_SetIndexOTPW(TMC2208_t *driver, bool otpw_mode);
void TMC2208_SetIndexStep(TMC2208_t *driver, bool step_pulses);
void TMC2208_SetPDNDisable(TMC2208_t *driver, bool disable);
void TMC2208_SetMstepRegSelect(TMC2208_t *driver, bool use_register);
void TMC2208_SetMultistepFilt(TMC2208_t *driver, bool enable);
void TMC2208_SetTestMode(TMC2208_t *driver, bool enable);
void TMC2208_InitGCONF(TMC2208_t *driver, TMC2208_GCONF_t *gconf);
void TMC2208_InitGCONF_InternalRsense(TMC2208_t *driver,TMC2208_GCONF_t *gconf);
bool TMC2208_ModifyGCONFBit(TMC2208_t *driver, uint32_t bit_mask, bool value);

// Individual GCONF flag getters (read from shadow)
bool TMC2208_GetIScaleAnalog(TMC2208_t *driver);
bool TMC2208_GetInternalRsense(TMC2208_t *driver);
bool TMC2208_GetSpreadCycle(TMC2208_t *driver);
bool TMC2208_GetShaft(TMC2208_t *driver);
bool TMC2208_GetIndexOTPW(TMC2208_t *driver);
bool TMC2208_GetIndexStep(TMC2208_t *driver);
bool TMC2208_GetPDNDisable(TMC2208_t *driver);
bool TMC2208_GetMstepRegSelect(TMC2208_t *driver);
bool TMC2208_GetMultistepFilt(TMC2208_t *driver);
bool TMC2208_GetTestMode(TMC2208_t *driver);


bool TMC2208_ReadIOIN(TMC2208_t *driver, TMC2208_IOIN_t *ioin);
bool TMC2208_ReadIOIN_Raw(TMC2208_t *driver, uint32_t *value);
bool TMC2208_UpdateIOIN(TMC2208_t *driver);

static inline bool TMC2208_GetENN(const TMC2208_IOIN_t *ioin) {
    return ioin->enn;
}

static inline bool TMC2208_GetPDN_UART(const TMC2208_IOIN_t *ioin) {
    return ioin->pdn_uart;
}

static inline bool TMC2208_GetMS1(const TMC2208_IOIN_t *ioin) {
    return ioin->ms1;
}

static inline bool TMC2208_GetMS2(const TMC2208_IOIN_t *ioin) {
    return ioin->ms2;
}

static inline bool TMC2208_GetDIAG(const TMC2208_IOIN_t *ioin) {
    return ioin->diag;
}

static inline bool TMC2208_GetSTEP(const TMC2208_IOIN_t *ioin) {
    return ioin->step;
}

static inline bool TMC2208_GetDIR(const TMC2208_IOIN_t *ioin) {
    return ioin->dir;
}

static inline uint8_t TMC2208_GetVersion(const TMC2208_IOIN_t *ioin) {
    return ioin->version;
}

static inline bool TMC2208_IsTMC220x(const TMC2208_IOIN_t *ioin) {
    return (ioin->version == 0x20);
}

static inline bool TMC2208_IsTMC222x(const TMC2208_IOIN_t *ioin) {
    return (ioin->version == 0x22);
}

static inline bool TMC2208_IsEnabled(const TMC2208_IOIN_t *ioin) {
    return !ioin->enn;
}

static inline uint8_t TMC2208_GetMicrostepMode(const TMC2208_IOIN_t *ioin) {
    return (ioin->ms2 << 1) | ioin->ms1;
}



bool TMC2208_ReadIHOLD_IRUN(TMC2208_t *driver, TMC2208_IHOLD_IRUN_t *ihold_irun);
bool TMC2208_ReadIHOLD_IRUN_Raw(TMC2208_t *driver, uint32_t *value);
void TMC2208_WriteIHOLD_IRUN(TMC2208_t *driver, const TMC2208_IHOLD_IRUN_t *ihold_irun);
void TMC2208_WriteIHOLD_IRUN_Raw(TMC2208_t *driver, uint32_t value);
void ParseIHOLD_IRUN(uint32_t raw_value, TMC2208_IHOLD_IRUN_t *ihold_irun);
void TMC2208_SetCurrent(TMC2208_t *driver, uint8_t irun, uint8_t ihold, uint8_t iholddelay);
void TMC2208_SetRunCurrent(TMC2208_t *driver, uint8_t irun);
void TMC2208_SetHoldCurrent(TMC2208_t *driver, uint8_t ihold);
void TMC2208_SetHoldDelay(TMC2208_t *driver, uint8_t iholddelay);
void TMC2208_InitIHOLD_IRUN(TMC2208_IHOLD_IRUN_t *ihold_irun, uint8_t irun, uint8_t ihold, uint8_t iholddelay);

uint8_t TMC2208_GetIHOLD(TMC2208_t *driver);
uint8_t TMC2208_GetIRUN(TMC2208_t *driver);
uint8_t TMC2208_GetIHOLDDELAY(TMC2208_t *driver);
uint16_t TMC2208_GetIHOLD_mA(TMC2208_t *driver, uint16_t rsense_milliohm);
uint16_t TMC2208_GetIRUN_mA(TMC2208_t *driver, uint16_t rsense_milliohm);

uint16_t TMC2208_CurrentToRMS(uint8_t cs_value, uint16_t rsense_milliohm);
uint8_t TMC2208_RMSToCurrent(uint16_t current_ma, uint16_t rsense_milliohm);

#endif /* TMC2208_REGISTERS_H */
