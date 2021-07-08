#include "PlatformTrigger.h"

#include "MovingPlatform.h"

APlatformTrigger::APlatformTrigger(const FObjectInitializer& ObjectInitializer): AActor(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	RootComponent = TriggerVolume;

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::Activate);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::Deactivate);
}

void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void APlatformTrigger::Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated!"))
		for (AMovingPlatform* Platform : PlatformsToTrigger) { Platform->IncreaseActiveTrigger(); }
	}
}

void APlatformTrigger::Deactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Deactivated!"))
		for (AMovingPlatform* Platform : PlatformsToTrigger) { Platform->DecreasesActiveTrigger(); }
	}
}
