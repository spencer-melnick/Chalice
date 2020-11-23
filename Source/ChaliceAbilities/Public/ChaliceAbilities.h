// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChaliceAbilities, Log, All);

class CHALICEABILITIES_API FChaliceAbilitiesModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
