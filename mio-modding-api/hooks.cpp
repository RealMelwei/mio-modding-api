#include "Windows.h"
#include "mio-modding-api.h"
#include "polyhook2/Detour/NatDetour.hpp"
#include <vector>
#include <functional>



namespace ModAPI {
	namespace Hooks {
		namespace Combat {
			// Functions hooked to OnHitEnemy
			std::vector<std::function<void(uintptr_t, uintptr_t)>> hitenemy_hooks;

			uintptr_t hitenemy_trampoline = NULL;

			NOINLINE void __cdecl ResolveOnHitEnemyHook(uintptr_t enemy_info_address, uintptr_t hit_info_address) {
				// Run all functions connected to this event
				for (int i = 0; i < hitenemy_hooks.size(); i++) {
					hitenemy_hooks[i](enemy_info_address, hit_info_address);
				}
				// Run the original Enemy Hit function (at the address provided in enemyhit_trampoline)
				typedef int func(uintptr_t, uintptr_t);
				func* trampoline = (func*)(hitenemy_trampoline);
				int i = trampoline(enemy_info_address, hit_info_address);
			}

			// Enter a function to call it when an enemy is hit. Parameters are the addresses of enemy data and hit data.
			MODDING_API void RunOnHitEnemy(std::function<void(uintptr_t, uintptr_t)> callback) {
				hitenemy_hooks.push_back(callback);
			}
		}

		// Initialize and connect hooks
		MODDING_API void InitializeHooks() {
			static PLH::NatDetour enemyhit_hook_detour = PLH::NatDetour((uintptr_t)ModAPI::Addresses::g_HitEnemyAddress, (uintptr_t)Combat::ResolveOnHitEnemyHook, &Combat::hitenemy_trampoline);
			enemyhit_hook_detour.hook();
		}
	}
}