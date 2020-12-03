// Copyright (c) 2020 Spencer Melnick

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"


// Forward declarations

class UWeaponTraceComponent;
struct FWeaponTraceShape;


struct CHALICEEDITOR_API HWeaponTraceVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY()

	HWeaponTraceVisProxy(const UActorComponent* InComponent, int32 ShapeIndex = 0)
		: HComponentVisProxy(InComponent, HPP_Wireframe), ShapeIndex(ShapeIndex)
	{};

	int32 ShapeIndex;
};

class CHALICEEDITOR_API FWeaponTraceVisualizer : public FComponentVisualizer
{
	
private:

	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;

	UWeaponTraceComponent* GetEditedTraceComponent() const;
	FWeaponTraceShape* GetEditedTraceShape() const;
	virtual void EndEditing() override;
	

	int32 SelectedShapeIndex = INDEX_NONE;
	FComponentPropertyPath TracePropertyPath;
};
