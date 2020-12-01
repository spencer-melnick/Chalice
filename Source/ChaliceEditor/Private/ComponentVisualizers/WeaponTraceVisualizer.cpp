// Copyright (c) 2020 Spencer Melnick

#include "ComponentVisualizers/WeaponTraceVisualizer.h"
#include "ChaliceGame/Combat/WeaponTraceComponent.h"


IMPLEMENT_HIT_PROXY(HWeaponTraceVisProxy, HComponentVisProxy)


// FWeaponTraceVisualizer

void FWeaponTraceVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UWeaponTraceComponent* WeaponTraceComponent = CastChecked<UWeaponTraceComponent>(Component);

	TracePropertyPath = FComponentPropertyPath(WeaponTraceComponent);

	const FLinearColor DefaultColor(0.1f, 0.73f, 0.98f);
	const FLinearColor SelectedColor(0.55f, 0.88f, 1.f);

	for (int32 Index = 0; Index < WeaponTraceComponent->TraceShapes.Num(); Index++)
	{
		const FWeaponTraceShape& TraceShape = WeaponTraceComponent->TraceShapes[Index];
		const FVector ShapeLocation = WeaponTraceComponent->GetTraceShapeLocation(TraceShape);
		const FLinearColor ShapeColor = SelectedShapeIndex == Index ? SelectedColor : DefaultColor;

		PDI->SetHitProxy(new HWeaponTraceVisProxy(Component, Index));
		DrawWireSphere(PDI, ShapeLocation, ShapeColor, TraceShape.Radius, 16, SDPG_Foreground);
	}

	PDI->SetHitProxy(nullptr);
}

bool FWeaponTraceVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	if (!VisProxy || !VisProxy->Component.IsValid())
	{
		return false;
	}

	if (VisProxy->IsA(HWeaponTraceVisProxy::StaticGetType()))
	{
		HWeaponTraceVisProxy* WeaponVisProxy = static_cast<HWeaponTraceVisProxy*>(VisProxy);
		SelectedShapeIndex = WeaponVisProxy->ShapeIndex;
	}

	return true;
}

bool FWeaponTraceVisualizer::GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const
{
	return false;
	
	/* UWeaponTraceComponent* TraceComponent = GetEditedTraceComponent();
	FWeaponTraceShape* TraceShape = GetEditedTraceShape();

	if (!TraceComponent || !TraceShape)
	{
		return false;
	}

	OutMatrix = TraceComponent->GetTraceShapeTransform(*TraceShape).ToMatrixWithScale();
	return true; */
}

bool FWeaponTraceVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	return false;
	
	/* UWeaponTraceComponent* TraceComponent = GetEditedTraceComponent();
	FWeaponTraceShape* TraceShape = GetEditedTraceShape();

	if (!TraceComponent || !TraceShape)
	{
		return false;
	}

	OutLocation = TraceComponent->GetTraceShapeLocation(*TraceShape);
	return true; */
}

bool FWeaponTraceVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale)
{
	return false;
	
	/* UWeaponTraceComponent* TraceComponent = GetEditedTraceComponent();
	FWeaponTraceShape* TraceShape = GetEditedTraceShape();

	if (!TraceComponent || !TraceShape)
	{
		return false;
	}

	TraceShape->Radius *= DeltaScale.Size();
	TraceShape->Location += DeltaTranslate;
	return true;*/
}


UWeaponTraceComponent* FWeaponTraceVisualizer::GetEditedTraceComponent() const
{
	return Cast<UWeaponTraceComponent>(TracePropertyPath.GetComponent());
}

FWeaponTraceShape* FWeaponTraceVisualizer::GetEditedTraceShape() const
{
	UWeaponTraceComponent* TraceComponent = GetEditedTraceComponent();
	if (!TraceComponent)
	{
		return nullptr;
	}
	if (SelectedShapeIndex < 0 || SelectedShapeIndex >= TraceComponent->TraceShapes.Num())
	{
		return nullptr;
	}
	return &TraceComponent->TraceShapes[SelectedShapeIndex];
}

void FWeaponTraceVisualizer::EndEditing()
{
	SelectedShapeIndex = INDEX_NONE;
	TracePropertyPath.Reset();
}
