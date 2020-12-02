// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChaliceDebug, Log, All);

class CHALICEDEBUG_API FChaliceDebugModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	TArray<FDelegateHandle> DisplayDelegateHandles;

};
