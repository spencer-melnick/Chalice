﻿// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChaliceUI, Log, All);

class CHALICEUI_API FChaliceUIModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};


// Widget check macros

#if WITH_EDITOR
#define CHECK_WIDGETS(X) \
	if (!(X)) \
	{ \
		return; \
	}
#else
#define CHECK_WIDGETS(X)
#endif
