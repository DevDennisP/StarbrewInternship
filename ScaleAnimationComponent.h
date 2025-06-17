// Copyright (C) 2023 Starbrew Games - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScaleAnimationComponent.generated.h"

/**
 * A component which handles a scale animation
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName="Scale Animation Component")
class DUCKSIDEGAME_API UScaleAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//Name of the MeshComponent you want to effect. If you need to work on two different meshes, add another ScaleAnimationComponent.
	UPROPERTY(EditAnywhere)
	FName MeshToScaleName;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> ScaleEaseCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimationDuration;
	float AnimationTime;

	UPROPERTY(BlueprintReadOnly)
	bool ScaleAnimationStarted;

	UScaleAnimationComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay = "2"))
	void StartScaleAnimation(float ScaleToPercentage, float& OutDuration, bool AutoScaleBack);

private:
	UPROPERTY()
	UMeshComponent* MeshToScale;
	
	FVector InitialScale;
	FVector TargetScale;
	FVector StartScale;

	bool ShouldScaleBack;

	void TickScaleAnimation(float DeltaTime);

	FVector CalculatedTargetScale(float ScaleToPercentage) const;
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
