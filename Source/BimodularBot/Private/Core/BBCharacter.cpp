// ©FIT Team


#include "Core/BBCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		// Move Action
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABBCharacter::Move);
		// Rotate Action
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABBCharacter::Rotate);
		// Jump Action
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		GetCharacterMovement()->JumpZVelocity = JumpHight;
		GetCharacterMovement()->AirControl = JumpControl;
		// Use Action
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &ABBCharacter::Use);
		// Debug Action
		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Triggered, this, &ABBCharacter::DebugButton);
	}
	else
	{
		UE_LOG(BimodularBotLog, Error, TEXT("'%s' Failed to find an Enhanced Input Component! You have done something really bad."), *GetNameSafe(this));
	}
}

void ABBCharacter::Move(const FInputActionValue& Value)
{
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABBCharacter::Rotate(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

/*void ABBCharacter::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump action pressed!"));
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		UE_LOG(LogTemp, Warning, TEXT("Jumping!"));
		AddMovementInput(GetActorUpVector(), JumpHight);
	}
}*/

void ABBCharacter::Use(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Use action pressed!"));
}

void ABBCharacter::DebugButton(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Debug action pressed!"));
	OnDebugPressed_BP();
}