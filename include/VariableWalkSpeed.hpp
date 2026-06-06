#pragma once
#include "Bethesda/TESObjectREFR.hpp"

void installVariableWalkSpeedHooks();
void OnWalkSpeedEventHandler(CommonLib::TESObjectREFR* thisObj, void* parameters);
void OnRunSpeedEventHandler(CommonLib::TESObjectREFR* thisObj, void* parameters);
