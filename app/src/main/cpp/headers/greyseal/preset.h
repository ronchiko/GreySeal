#pragma once

#include <seal.ntt.h>

#include "greyseal/object.h"

typedef int Seal_Preset;

/**
 * \brief Loads and caches a preset from a file
 * \param path The path to the preset
 * \return The index of the new preset
 */
Seal_Preset Seal_LoadCachePreset(const Seal_String&);
/**
 * \brief Setups the Ntt parser and preset cache
 * \return -1 on failure, 0 otherwise
 */
int Seal_SetupNttEnvironment(void);
/**
 * \brief Cleans all the preset cache
 */
void Seal_PresetWipeCache(void);
/**
 * \brief Ends the Ntt environment by SealNtt_Cleanup() and Seal_PresetWipeCache()
 */
void Seal_EndNttEnvironment(void);
/**
 * \brief Overrides an object information with a presets info
 * \param o The object to override
 * \param presetIndex The index of the preset to override with
 * \param alsoPosition Set to 1 if the transform of the objects transform should be overridden
 * \return
 */
int Seal_OverwriteObject(Seal_Entity* o, Seal_Preset presetIndex, Seal_Bool alsoPosition = SEAL_FALSE);