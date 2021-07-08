#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

class AMovingPlatform;

UCLASS()
class PUZZLEGAME_ONLINE_API APlatformTrigger : public AActor
{
	GENERATED_BODY()

public:
	explicit APlatformTrigger(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Deactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "Triggering")
	TArray<AMovingPlatform *> PlatformsToTrigger;
};
