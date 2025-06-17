// Copyright (C) 2023 Starbrew Games - All Rights Reserved


#include "ScaleAnimationComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UScaleAnimationComponent::UScaleAnimationComponent(): AnimationDuration(0.25f), AnimationTime(0), ScaleAnimationStarted(false), MeshToScale(nullptr), ShouldScaleBack(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UScaleAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	
	for(UActorComponent* Comp : GetOwner()->GetComponents())
	{
		if(Comp->GetFName() == MeshToScaleName)
		{
			MeshToScale = Cast<UMeshComponent>(Comp);
			InitialScale = MeshToScale->GetComponentScale();
			break;
		}
	}
}

void UScaleAnimationComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ScaleAnimationStarted) TickScaleAnimation(DeltaTime);
}

void UScaleAnimationComponent::StartScaleAnimation(const float ScaleToPercentage, float& OutDuration, const bool AutoScaleBack)
{
	if (!MeshToScale) return;
	StartScale = MeshToScale->GetComponentScale();
	TargetScale = CalculatedTargetScale(ScaleToPercentage);

	ShouldScaleBack = AutoScaleBack;
	
	ScaleAnimationStarted = true;
	SetComponentTickEnabled(true);
	AnimationTime = 0.0f;

	OutDuration = AnimationDuration;
}

void UScaleAnimationComponent::TickScaleAnimation(const float DeltaTime)
{
	if (AnimationTime >= AnimationDuration)
	{
		if (ShouldScaleBack)
		{
			StartScaleAnimation(100, AnimationDuration, false);
			return;
		}
		ScaleAnimationStarted = false;
		SetComponentTickEnabled(false);
		return;
	}

	AnimationTime += DeltaTime;
	const float AnimationPercentage = FMath::Clamp(AnimationTime / AnimationDuration, 0.0f, 1.0f);
	const float ScaleAlpha = ScaleEaseCurve ? ScaleEaseCurve->GetFloatValue(AnimationPercentage) : AnimationPercentage;
	const FVector NewScale = FMath::Lerp(StartScale, TargetScale, ScaleAlpha);
	MeshToScale->SetWorldScale3D(NewScale);
}

FVector UScaleAnimationComponent::CalculatedTargetScale(const float ScaleToPercentage) const
{
	FVector ReturnScale;
	ReturnScale.X = InitialScale.X * (ScaleToPercentage * 0.01); // doing * 0.01 instead of / 100 to prevent dividing 0
	ReturnScale.Y = InitialScale.Y * (ScaleToPercentage * 0.01);
	ReturnScale.Z = InitialScale.Z * (ScaleToPercentage * 0.01);
	return ReturnScale;
}

#if WITH_EDITOR
EDataValidationResult UScaleAnimationComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
	
	if (MeshToScaleName == "")
	{
		Result = EDataValidationResult::Invalid;
		Context.AddError((INVTEXT("The Scale Animation Component Name is empty")));
	}
	return Result;
}
#endif