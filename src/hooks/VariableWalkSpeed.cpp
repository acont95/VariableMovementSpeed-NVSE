#include <cstdint>
#include <atomic>
#include "VariableWalkSpeed.hpp"
#include "nvse/PluginAPI.h"
#include "nvse/SafeWrite.h"
#include "Bethesda/ActorValueOwner.hpp"
#include "Bethesda/TESObjectWEAP.hpp"
#include "Bethesda/TESObjectARMO.hpp"

CallDetour GetActorWalkSpeedDetour{};
CallDetour GetActorRunSpeedDetour{};

static std::atomic<float> fWalkSpeedMult = 1.0f;
static std::atomic<float> fRunSpeedMult = 1.0f;

float __cdecl Hook_GetActorWalkSpeed(CommonLib::ActorValueOwner* apOwner, CommonLib::TESObjectWEAP* pWeap, CommonLib::TESObjectARMO* pArmor, bool abSneaking, bool abNoWeapon, bool abIsNpc, bool abOverEncumbered) {
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

	return fWalkSpeed * fWalkSpeedMult;
}


float __cdecl Hook_GetActorRunSpeed(CommonLib::ActorValueOwner* apOwner, CommonLib::TESObjectWEAP* pWeap, CommonLib::TESObjectARMO* pArmor, bool abSneaking, bool abNoWeapon, bool abIsNpc, bool abOverEncumbered) {
	float fRunSpeed = CdeclCall<float>(
		GetActorRunSpeedDetour.GetOverwrittenAddr(),
		apOwner,
		pWeap,
		pArmor,
		abSneaking,
		abNoWeapon,
		abIsNpc,
		abOverEncumbered
	);

	return fRunSpeed * fRunSpeedMult;
}

void OnWalkSpeedEventHandler(CommonLib::TESObjectREFR* thisObj, void* parameters) {
	uintptr_t* args = static_cast<uintptr_t*>(parameters);
	float fArg = std::bit_cast<float>(args[0]);

	fWalkSpeedMult = fArg;
}

void OnRunSpeedEventHandler(CommonLib::TESObjectREFR* thisObj, void* parameters) {
	uintptr_t* args = static_cast<uintptr_t*>(parameters);
	float fArg = std::bit_cast<float>(args[0]);

	fRunSpeedMult = fArg;
}


void installVariableWalkSpeedHooks() {
	GetActorWalkSpeedDetour.WriteRelCall(0x00885B88, reinterpret_cast<std::uint32_t>(&Hook_GetActorWalkSpeed));
	GetActorRunSpeedDetour.WriteRelCall(0x00885D24, reinterpret_cast<std::uint32_t>(&Hook_GetActorRunSpeed));
}
