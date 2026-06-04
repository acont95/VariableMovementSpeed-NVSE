#include <cstdint>
#include "VariableWalkSpeed.hpp"
#include "nvse/PluginAPI.h"
#include "nvse/SafeWrite.h"
#include "Bethesda/TESGlobal.hpp"
#include "Bethesda/ActorValueOwner.hpp"
#include "Bethesda/TESObjectWEAP.hpp"

constexpr char CONFIG_SECTION[] = "VariableWalkSpeed";
constexpr char GLOBAL_VAR_FORM[] = "VMSWalkSpeedMult";

CallDetour GetWalkSpeedDetour{};
CallDetour GetActorWalkSpeedDetour{};

constexpr std::uint32_t TESForm_GetFormByEditorID_Address = 0x00483A00;

float __cdecl Hook_GetActorWalkSpeed(ActorValueOwner* apOwner, TESObjectWEAP* pWeap, TESObjectARMO* pArmor, bool abSneaking, bool abNoWeapon, bool abIsNpc, bool abOverEncumbered) {
	float fWalkSpeed = CdeclCall<float>(
		GetActorWalkSpeedDetour.GetOverwrittenAddr(), 
		apOwner,
		pWeap,
		pArmor,
		abSneaking,
		abNoWeapon,
		abIsNpc,
		abOverEncumbered
	);
	CommonLib::TESForm* pForm = CdeclCall<CommonLib::TESForm*>(TESForm_GetFormByEditorID_Address, GLOBAL_VAR_FORM);
	if (pForm && pForm->cFormType == CommonLib::ENUM_FORM_ID::GLOB_ID) {
		CommonLib::TESGlobal* pGlobal = static_cast<CommonLib::TESGlobal*>(pForm);
		return fWalkSpeed * pGlobal->fValue;
	}

	return fWalkSpeed;
}


void installVariableWalkSpeedHooks() {
	GetActorWalkSpeedDetour.WriteRelCall(0x00885B88, reinterpret_cast<std::uint32_t>(&Hook_GetActorWalkSpeed));
}
