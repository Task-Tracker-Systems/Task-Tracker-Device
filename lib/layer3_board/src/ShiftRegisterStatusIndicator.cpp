#include "ShiftRegisterStatusIndicator.hpp"

ShiftRegisterStatusIndicator::ShiftRegisterStatusIndicator(IOutputShiftRegister &outputShiftRegister,
                                                           const IOutputShiftRegister::BitNumber bit)
    : shiftRegister(outputShiftRegister), bit(bit)
{
}

void ShiftRegisterStatusIndicator::off()
{
    shiftRegister.writePin(bit, false);
}

void ShiftRegisterStatusIndicator::on()
{
    shiftRegister.writePin(bit, true);
}
