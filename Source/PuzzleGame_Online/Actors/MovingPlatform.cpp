#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform(const FObjectInitializer& ObjectInitializer) :
	AStaticMeshActor(ObjectInitializer),
	Speed(100.0f),
	bNeedActivation(false),
	ActiveTriggerCount(0)
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);

		StartLocationInGlobal = GetActorLocation();
		TargetLocationInGlobal = GetTransform().TransformPosition(TargetLocation);
		MoveDirection = (TargetLocationInGlobal - StartLocationInGlobal).GetSafeNormal();
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		const bool bShouldMove = !bNeedActivation || (ActiveTriggerCount > 0);
		if (bShouldMove)
		{
			FVector Location = GetActorLocation();

			const float JourneyLength = (TargetLocationInGlobal - StartLocationInGlobal).Size();
			const float JourneyTraveled = (Location - StartLocationInGlobal).Size();
			if (JourneyTraveled > JourneyLength)
			{
				MoveDirection = -MoveDirection;
				Swap(StartLocationInGlobal, TargetLocationInGlobal);
			}

			Location += MoveDirection * Speed * DeltaTime;
			SetActorLocation(Location);
		}
	}
}
