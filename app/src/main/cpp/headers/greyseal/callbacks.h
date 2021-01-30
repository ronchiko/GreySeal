#pragma once

/**
 * \brief The types of seal callbacks possible
 */
typedef enum _Seal_EngineCallbackType_e {
    SEAL_UPDATE,
    SEAL_BEGIN,
    SEAL_END
} Seal_EngineCallbackType;

/**
 * \brief Invokes a seal callback event to the systems
 */
void Seal_InvokeCallback(Seal_EngineCallbackType, float);