#include "TMC2208_Registers.h"

static void parseGCONF(uint32_t raw_value, TMC2208_GCONF_t *gconf) {
    gconf->raw_value = raw_value;
    gconf->i_scale_analog   = (raw_value & TMC2208_GCONF_I_SCALE_ANALOG_MASK) ? true : false;
    gconf->internal_rsense  = (raw_value & TMC2208_GCONF_INTERNAL_RSENSE_MASK) ? true : false;
    gconf->en_spreadcycle   = (raw_value & TMC2208_GCONF_EN_SPREADCYCLE_MASK) ? true : false;
    gconf->shaft            = (raw_value & TMC2208_GCONF_SHAFT_MASK) ? true : false;
    gconf->index_otpw       = (raw_value & TMC2208_GCONF_INDEX_OTPW_MASK) ? true : false;
    gconf->index_step       = (raw_value & TMC2208_GCONF_INDEX_STEP_MASK) ? true : false;
    gconf->pdn_disable      = (raw_value & TMC2208_GCONF_PDN_DISABLE_MASK) ? true : false;
    gconf->mstep_reg_select = (raw_value & TMC2208_GCONF_MSTEP_REG_SELECT_MASK) ? true : false;
    gconf->multistep_filt   = (raw_value & TMC2208_GCONF_MULTISTEP_FILT_MASK) ? true : false;
    gconf->test_mode        = (raw_value & TMC2208_GCONF_TEST_MODE_MASK) ? true : false;
}


static uint32_t buildGCONF(const TMC2208_GCONF_t *gconf) {
    uint32_t value = 0;
    if (gconf->i_scale_analog)   value |= TMC2208_GCONF_I_SCALE_ANALOG_MASK;
    if (gconf->internal_rsense)  value |= WTMC2208_GCONF_INTERNAL_RSENSE_MASK;
    if (gconf->en_spreadcycle)   value |= TMC2208_GCONF_EN_SPREADCYCLE_MASK;
    if (gconf->shaft)            value |= TMC2208_GCONF_SHAFT_MASK;
    if (gconf->index_otpw)       value |= TMC2208_GCONF_INDEX_OTPW_MASK;
    if (gconf->index_step)       value |= TMC2208_GCONF_INDEX_STEP_MASK;
    if (gconf->pdn_disable)      value |= TMC2208_GCONF_PDN_DISABLE_MASK;
    if (gconf->mstep_reg_select) value |= TMC2208_GCONF_MSTEP_REG_SELECT_MASK;
    if (gconf->multistep_filt)   value |= TMC2208_GCONF_MULTISTEP_FILT_MASK;
    if (gconf->test_mode)        value |= TMC2208_GCONF_TEST_MODE_MASK;
    return value;
}

//bool TMC2208_ReadGCONF_Raw(TMC2208_t *driver, uint32_t *value) {
//    if (driver == NULL || value == NULL) return false;
//    *value = driver->shadow_gconf;
//    return true;
//}

bool TMC2208_ReadGCONF(TMC2208_t *driver, TMC2208_GCONF_t *gconf) {
    if (driver == NULL || gconf == NULL) return false;
   // uint32_t raw_value = 0;
  //  if (!TMC_ReadRegister(driver, TMC2208_GCONF, &raw_value)) return false;
    parseGCONF(driver->shadow_gconf, gconf);
    return true;
}

//void TMC2208_WriteGCONF_Raw(TMC2208_t *driver, uint32_t value) {
//    if (driver == NULL) return;
//    driver->shadow_gconf=value;
//    TMC_WriteRegister(driver, TMC2208_GCONF, value);
//}

void TMC2208_WriteGCONF(TMC2208_t *driver, const TMC2208_GCONF_t *gconf) {
    if (driver == NULL || gconf == NULL) return;
    uint32_t value = buildGCONF(gconf);
    driver->shadow_gconf=value;
    TMC_WriteRegister(driver, TMC2208_GCONF, value);
}

void TMC2208_SetIScaleAnalog(TMC2208_t *driver, bool enable) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_I_SCALE_ANALOG_MASK, enable);
}

void TMC2208_SetInternalRsense(TMC2208_t *driver, bool enable) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_INTERNAL_RSENSE_MASK, enable);
}

void TMC2208_SetSpreadCycle(TMC2208_t *driver, bool enable) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_EN_SPREADCYCLE_MASK, enable);
}

void TMC2208_SetShaft(TMC2208_t *driver, bool inverse) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_SHAFT_MASK, inverse);
}

void TMC2208_SetIndexOTPW(TMC2208_t *driver, bool otpw_mode) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_INDEX_OTPW_MASK, otpw_mode);
}

void TMC2208_SetIndexStep(TMC2208_t *driver, bool step_pulses) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_INDEX_STEP_MASK, step_pulses);
}

void TMC2208_SetPDNDisable(TMC2208_t *driver, bool disable) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_PDN_DISABLE_MASK, disable);
}

void TMC2208_SetMstepRegSelect(TMC2208_t *driver, bool use_register) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_MSTEP_REG_SELECT_MASK, use_register);
}

void TMC2208_SetMultistepFilt(TMC2208_t *driver, bool enable) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_MULTISTEP_FILT_MASK, enable);
}

void TMC2208_SetTestMode(TMC2208_t *driver, bool enable) {
    TMC2208_ModifyGCONFBit(driver, TMC2208_GCONF_TEST_MODE_MASK, enable);
}

void TMC2208_InitGCONF(TMC2208_t *driver,TMC2208_GCONF_t *gconf) {
    if (gconf == NULL||driver==NULL) return;
    gconf->i_scale_analog = false;
    gconf->internal_rsense = false;
    gconf->en_spreadcycle = false;
    gconf->shaft = false;
    gconf->index_otpw = false;
    gconf->index_step = false;
    gconf->pdn_disable = false;
    gconf->mstep_reg_select = false;
    gconf->multistep_filt = true;
    gconf->test_mode = false;
    gconf->raw_value = buildGCONF(gconf);
    driver->shadow_gconf=gconf->raw_value;
    TMC2208_WriteGCONF(driver, gconf);
}

void TMC2208_InitGCONF_InternalRsense(TMC2208_t *driver,TMC2208_GCONF_t *gconf) {
    if (gconf == NULL) return;

    gconf->internal_rsense = true;
    gconf->i_scale_analog = true;
   gconf->raw_value = buildGCONF(gconf);
    driver->shadow_gconf = gconf->raw_value;
    TMC2208_InitGCONF(driver,gconf);
}

bool TMC2208_ModifyGCONFBit(TMC2208_t *driver, uint32_t bit_mask, bool value) {
    if (driver == NULL) return false;
//    uint32_t current_value = 0;
//    if (!TMC_ReadRegister(driver, TMC2208_GCONF, &current_value)) return false;
//    if (value) {
//        current_value |= bit_mask;
//    } else {
//        current_value &= ~bit_mask;
//    }
    if(value)
    	driver->shadow_gconf|=bit_mask;
    else
    	driver->shadow_gconf&= ~bit_mask;
    TMC_WriteRegister(driver, TMC2208_GCONF, driver->shadow_gconf);
    return true;
}
bool TMC2208_GetIScaleAnalog(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_I_SCALE_ANALOG_MASK) ? true : false;
}
bool TMC2208_GetInternalRsense(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_INTERNAL_RSENSE_MASK) ? true : false;
}
bool TMC2208_GetSpreadCycle(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_EN_SPREADCYCLE_MASK) ? true : false;
}

bool TMC2208_GetShaft(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_SHAFT_MASK) ? true : false;
}
bool TMC2208_GetIndexOTPW(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_INDEX_OTPW_MASK) ? true : false;
}
bool TMC2208_GetIndexStep(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_INDEX_STEP_MASK) ? true : false;
}
bool TMC2208_GetPDNDisable(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_PDN_DISABLE_MASK) ? true : false;
}
bool TMC2208_GetMstepRegSelect(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_MSTEP_REG_SELECT_MASK) ? true : false;
}
bool TMC2208_GetMultistepFilt(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_MULTISTEP_FILT_MASK) ? true : false;
}
bool TMC2208_GetTestMode(TMC2208_t *driver) {
    if (driver == NULL) return false;
    return (driver->shadow_gconf & TMC2208_GCONF_TEST_MODE_MASK) ? true : false;
}


static void parseIOIN(uint32_t raw_value, TMC2208_IOIN_t *ioin) {
    ioin->raw_value = raw_value;
    ioin->enn           = (raw_value & TMC2208_IOIN_ENN_MASK) ? true : false;
    ioin->pdn_uart      = (raw_value & TMC2208_IOIN_PDN_UART_MASK) ? true : false;
    ioin->ms1           = (raw_value & TMC2208_IOIN_MS1_MASK) ? true : false;
    ioin->ms2           = (raw_value & TMC2208_IOIN_MS2_MASK) ? true : false;
    ioin->diag          = (raw_value & TMC2208_IOIN_DIAG_MASK) ? true : false;
    ioin->step_222x     = (raw_value & TMC2208_IOIN_STEP_222X_MASK) ? true : false;
    ioin->pdn_uart_bit6 = (raw_value & TMC2208_IOIN_PDN_UART2_MASK) ? true : false;
    ioin->step          = (raw_value & TMC2208_IOIN_STEP_MASK) ? true : false;
    ioin->sel_a         = (raw_value & TMC2208_IOIN_SEL_A_MASK) ? true : false;
    ioin->dir           = (raw_value & TMC2208_IOIN_DIR_MASK) ? true : false;
    ioin->version       = (uint8_t)((raw_value & TMC2208_IOIN_VERSION_MASK) >> TMC2208_IOIN_VERSION_SHIFT);
}

//bool TMC2208_ReadIOIN_Raw(TMC2208_t *driver, uint32_t *value) {
//    if (driver == NULL || value == NULL) return false;
//    return TMC_ReadRegister(driver, TMC2208_IOIN, value);
//}

bool TMC2208_ReadIOIN(TMC2208_t *driver, TMC2208_IOIN_t *ioin) {
    if (driver == NULL || ioin == NULL) return false;
    uint32_t raw_value = 0;
    if (!TMC_ReadRegister(driver, TMC2208_IOIN, &raw_value)) return false;
    parseIOIN(raw_value, ioin);
    return true;
}



 void parseIHOLD_IRUN(uint32_t raw_value, TMC2208_IHOLD_IRUN_t *ihold_irun) {
    ihold_irun->raw_value = raw_value;
    ihold_irun->ihold = (uint8_t)((raw_value & TMC2208_IHOLD_MASK) >> TMC2208_IHOLD_SHIFT);
    ihold_irun->irun = (uint8_t)((raw_value & TMC2208_IRUN_MASK) >> TMC2208_IRUN_SHIFT);
    ihold_irun->iholddelay = (uint8_t)((raw_value & TMC2208_IHOLDDELAY_MASK) >> TMC2208_IHOLDDELAY_SHIFT);
}


static uint32_t buildIHOLD_IRUN(const TMC2208_IHOLD_IRUN_t *ihold_irun) {
    uint32_t value = 0;
    value |= ((uint32_t)(ihold_irun->ihold & 0x1F) << TMC2208_IHOLD_SHIFT);
    value |= ((uint32_t)(ihold_irun->irun & 0x1F) << TMC2208_IRUN_SHIFT);
    value |= ((uint32_t)(ihold_irun->iholddelay & 0x0F) << TMC2208_IHOLDDELAY_SHIFT);
    return value;
}

bool TMC2208_ReadIHOLD_IRUN_Raw(TMC2208_t *driver, uint32_t *value) {
    if (driver == NULL || value == NULL) return false;
    *value = driver->shadow_ihold_irun;
    return /*TMC_ReadRegister(driver, TMC2208_IHOLD_IRUN, value);*/ true;
}

bool TMC2208_ReadIHOLD_IRUN(TMC2208_t *driver, TMC2208_IHOLD_IRUN_t *ihold_irun) {
    if (driver == NULL || ihold_irun == NULL) return false;
//    uint32_t raw_value = 0;
   // if (!TMC_ReadRegister(driver, TMC2208_IHOLD_IRUN, &raw_value)) return false;
    parseIHOLD_IRUN(driver->shadow_ihold_irun, ihold_irun);
    return true;
}

//void TMC2208_WriteIHOLD_IRUN_Raw(TMC2208_t *driver, uint32_t value) {
//    if (driver == NULL) return;
//    TMC_WriteRegister(driver, TMC2208_IHOLD_IRUN, value);
//    driver->shadow_ihold_irun = value;
//}

void TMC2208_WriteIHOLD_IRUN(TMC2208_t *driver, const TMC2208_IHOLD_IRUN_t *ihold_irun) {
    if (driver == NULL || ihold_irun == NULL) return;
    uint32_t value = buildIHOLD_IRUN(ihold_irun);
    TMC_WriteRegister(driver, TMC2208_IHOLD_IRUN , value);
    driver->shadow_ihold_irun = value;
}

void TMC2208_SetCurrent(TMC2208_t *driver, uint8_t irun, uint8_t ihold, uint8_t iholddelay) {
    if (driver == NULL) return;
    TMC2208_IHOLD_IRUN_t ihold_irun;
    ihold_irun.irun = irun & 0x1F;
    ihold_irun.ihold = ihold & 0x1F;
    ihold_irun.iholddelay = iholddelay & 0x0F;
    TMC2208_WriteIHOLD_IRUN(driver, &ihold_irun);
}

void TMC2208_SetRunCurrent(TMC2208_t *driver, uint8_t irun) {
    if (driver == NULL) return;
    TMC2208_IHOLD_IRUN_t ihold_irun;
    parseIHOLD_IRUN(driver->shadow_ihold_irun, &ihold_irun);
   // if (!TMC2208_ReadIHOLD_IRUN(driver, &ihold_irun)) return;
    ihold_irun.irun = irun & 0x1F;
    TMC2208_WriteIHOLD_IRUN(driver, &ihold_irun);
}

void TMC2208_SetHoldCurrent(TMC2208_t *driver, uint8_t ihold) {
    if (driver == NULL) return;
    TMC2208_IHOLD_IRUN_t ihold_irun;
    parseIHOLD_IRUN(driver->shadow_ihold_irun, &ihold_irun);
//    if (!TMC2208_ReadIHOLD_IRUN(driver, &ihold_irun)) return;
    ihold_irun.ihold = ihold & 0x1F;
 //   driver->shadow_ihold_irun = buildIHOLD_IRUN(&ihold_irun);
    TMC2208_WriteIHOLD_IRUN(driver, &ihold_irun);
}

void TMC2208_SetHoldDelay(TMC2208_t *driver, uint8_t iholddelay) {
    if (driver == NULL) return;
    TMC2208_IHOLD_IRUN_t ihold_irun;
    parseIHOLD_IRUN(driver->shadow_ihold_irun, &ihold_irun);
//    if (!TMC2208_ReadIHOLD_IRUN(driver, &ihold_irun)) return;
    ihold_irun.iholddelay = iholddelay & 0x0F;
    TMC2208_WriteIHOLD_IRUN(driver, &ihold_irun);
}

void TMC2208_InitIHOLD_IRUN(TMC2208_IHOLD_IRUN_t *ihold_irun, uint8_t irun, uint8_t ihold, uint8_t iholddelay) {
    if (ihold_irun == NULL) return;
    ihold_irun->irun = irun & 0x1F;
    ihold_irun->ihold = ihold & 0x1F;
    ihold_irun->iholddelay = iholddelay & 0x0F;
    ihold_irun->raw_value = buildIHOLD_IRUN(ihold_irun);
}
uint8_t TMC2208_GetIHOLD(TMC2208_t *driver) {
    if (driver == NULL) return 0;
    return (uint8_t)((driver->shadow_ihold_irun & TMC2208_IHOLD_MASK) >> TMC2208_IHOLD_SHIFT);
}
uint8_t TMC2208_GetIRUN(TMC2208_t *driver) {
    if (driver == NULL) return 0;
    return (uint8_t)((driver->shadow_ihold_irun & TMC2208_IRUN_MASK) >> TMC2208_IRUN_SHIFT);
}
uint8_t TMC2208_GetIHOLDDELAY(TMC2208_t *driver) {
    if (driver == NULL) return 0;
    return (uint8_t)((driver->shadow_ihold_irun & TMC2208_IHOLDDELAY_MASK) >> TMC2208_IHOLDDELAY_SHIFT);
}
uint16_t TMC2208_GetIHOLD_mA(TMC2208_t *driver, uint16_t rsense_milliohm) {
    uint8_t ihold = TMC2208_GetIHOLD(driver);
    return TMC2208_CurrentToRMS(ihold, rsense_milliohm);
}
uint16_t TMC2208_GetIRUN_mA(TMC2208_t *driver, uint16_t rsense_milliohm) {
    uint8_t irun = TMC2208_GetIRUN(driver);
    return TMC2208_CurrentToRMS(irun, rsense_milliohm);
}

/* Current conversion helpers */
uint16_t TMC2208_CurrentToRMS(uint8_t cs_value, uint16_t rsense_milliohm) {

    if (rsense_milliohm == 110) {
        return (uint16_t)((cs_value + 1) * 35);
    } else {
        uint32_t vfs = 325;
        uint32_t current = (uint32_t)(cs_value + 1) * vfs * 1000;
        current = current / (32 * (rsense_milliohm + 20));
        current = (current * 707) / 1000;
        return (uint16_t)current;
    }
}

uint8_t TMC2208_RMSToCurrent(uint16_t current_ma, uint16_t rsense_milliohm) {
    /* Reverse calculation */
    if (rsense_milliohm == 110) {
        uint8_t cs = (uint8_t)(current_ma / 35);
        if (cs > 0) cs--;
        if (cs > 31) cs = 31;
        return cs;
    } else {
        uint32_t vfs = 325;
        uint32_t cs = (uint32_t)current_ma * 32 * (rsense_milliohm + 20);
        cs = (cs * 1000) / 707;
        cs = cs / (vfs * 1000);
        if (cs > 0) cs--;
        if (cs > 31) cs = 31;
        return (uint8_t)cs;
    }
}
