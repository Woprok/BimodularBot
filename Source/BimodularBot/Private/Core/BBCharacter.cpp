// ©FIT Team


#include "Core/BBCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BBUtils.h"

// Sets default values
ABBCharacter::ABBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABBCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ABBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Debug Action
		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Triggered, this, &ABBCharacter::Debug);
	}
	else
	{
		UE_LOG(BimodularBotLog, Error, TEXT("'%s' Failed to find an Enhanced Input Component! You have done something really bad."), *GetNameSafe(this));
	}
}

void ABBCharacter::Debug(const FInputActionValue& Value)
{

}