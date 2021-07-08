#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class PUZZLEGAME_ONLINE_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	explicit AMovingPlatform(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void IncreaseActiveTrigger() { ActiveTriggerCount++; }
	FORCEINLINE void DecreasesActiveTrigger() { if (ActiveTriggerCount > 0) { ActiveTriggerCount--; } }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (MakeEditWidget = true))
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bNeedActivation;

private:
	FVector StartLocationInGlobal;
	FVector TargetLocationInGlobal;
	FVector MoveDirection;

	int ActiveTriggerCount;
};
