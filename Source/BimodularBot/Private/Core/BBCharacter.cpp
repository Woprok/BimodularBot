// ©FIT Team


#include "Core/BBCharacter.h"
#include "Core/BBPlayerController.h"
#include "BBUtils.h"

#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABBCharacter::ABBCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));
	MuzzleArrow->SetupAttachment(GetMesh());

	// Camera Setup	
	SetupCamera();
}

// Called when the game starts or when spawned
void ABBCharacter::BeginPlay()
{
	Super::BeginPlay();
	SecondPlayerCharacter = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
	FirstPlayerCharacter = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	GetWorld()->GetTimerManager().SetTimer(
		MergeCheckHandle,
		this,
		&ABBCharacter::CheckPlayerDistance,
		0.2f,
		true
	);
}

// Called every frame
void ABBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABBCharacter::CheckPlayerDistance()
{
	if (!FirstPlayerCharacter || !SecondPlayerCharacter) return;

	float Distance = FVector::Dist(FirstPlayerCharacter->GetActorLocation(), SecondPlayerCharacter->GetActorLocation());
	CanMerge = Distance <= MaxDistance;
}

void ABBCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

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
		if (ABBPlayerController* PlayerController = Cast<ABBPlayerController>(Controller))
		{
			//UE_LOG(BimodularBotLog, Log, TEXT("Player Controller to Character Setup %s."), *GetNameSafe(this));
			
			// Shared Actions
			// Mouse & Keyboard is exclusive to 1st player.
			// Streaming comes in when Mouse & Keyboard control the 2nd player.
			// Controller controls are shared between both players as there is no need to differentiate them on action level.
			// We can easily differentiate on character instance level between them.
			
			// Divide & Join
			EnhancedInputComponent->BindAction(DivideAndJoinAction, ETriggerEvent::Started, this, &ABBCharacter::DivideAndJoinStarted);
			EnhancedInputComponent->BindAction(DivideAndJoinAction, ETriggerEvent::Ongoing, this, &ABBCharacter::DivideAndJoinOngoing);
			EnhancedInputComponent->BindAction(DivideAndJoinAction, ETriggerEvent::Completed, this, &ABBCharacter::DivideAndJoinCompleted);
			EnhancedInputComponent->BindAction(DivideAndJoinAction, ETriggerEvent::Canceled, this, &ABBCharacter::DivideAndJoinCompleted);
			EnhancedInputComponent->BindAction(DivideAndJoinAction, ETriggerEvent::Triggered, this, &ABBCharacter::DivideAndJoinTriggered);
			// Rotate Action
			EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABBCharacter::RotateTriggered);
			// Pause Action
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ABBCharacter::PauseTriggered);


			// First Character
			if (PlayerController->IsFirstSplitScreenPlayer())
			{
				//UE_LOG(BimodularBotLog, Log, TEXT("First Player %s."), *GetNameSafe(this));
				// Move Action
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABBCharacter::MoveTriggered);
				// Jump Action
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABBCharacter::JumpStarted);
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABBCharacter::JumpCompleted);



				// Rotate Action
				EnhancedInputComponent->BindAction(RotateActionStreaming, ETriggerEvent::Triggered, this, &ABBCharacter::RotateStreamingTriggered); // RotateSecondStreaming

				// PickUp Action
				EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &ABBCharacter::PickUp);

				EnhancedInputComponent->BindAction(DivideAndJoinActionStreaming, ETriggerEvent::Started, this, &ABBCharacter::DivideAndJoinStreamingStarted);
				EnhancedInputComponent->BindAction(DivideAndJoinActionStreaming, ETriggerEvent::Ongoing, this, &ABBCharacter::DivideAndJoinStreamingOngoing);
				EnhancedInputComponent->BindAction(DivideAndJoinActionStreaming, ETriggerEvent::Completed, this, &ABBCharacter::DivideAndJoinStreamingCompleted);
				EnhancedInputComponent->BindAction(DivideAndJoinActionStreaming, ETriggerEvent::Canceled, this, &ABBCharacter::DivideAndJoinStreamingCompleted);
				EnhancedInputComponent->BindAction(DivideAndJoinActionStreaming, ETriggerEvent::Triggered, this, &ABBCharacter::DivideAndJoinStreamingTriggered);

				// Debug Action
				EnhancedInputComponent->BindAction(DebugActionFirstCharacter, ETriggerEvent::Triggered, this, &ABBCharacter::DebugButtonFirst);
				EnhancedInputComponent->BindAction(DebugActionSecondCharacterStreaming, ETriggerEvent::Triggered, this, &ABBCharacter::DebugButtonSecondStreaming); // DebugButtonSecondStreaming

				// Use
				EnhancedInputComponent->BindAction(UseActionStreaming, ETriggerEvent::Triggered, this, &ABBCharacter::UseStreaming); // UseStreaming
			}
			
			// Second Character
			else
			{
				//UE_LOG(BimodularBotLog, Log, TEXT("Second Player %s."), *GetNameSafe(this));
				// Use Action - splitted for interacting, IsMerged for fireing
				EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &ABBCharacter::Use);
				
				// Debug Action
				EnhancedInputComponent->BindAction(DebugActionSecondCharacter, ETriggerEvent::Triggered, this, &ABBCharacter::DebugButtonSecond);
			}
		}
	}
	else
	{
		UE_LOG(BimodularBotLog, Error, TEXT("'%s' Failed to find an Enhanced Input Component! You have done something really bad."), *GetNameSafe(this));
	}
}

void ABBCharacter::Use(const FInputActionValue& Value)
{
	if (!IsMerged)
	{
		UE_LOG(BimodularBotLog, Warning, TEXT("Using!"));
	}
	else
	{
		UE_LOG(BimodularBotLog, Warning, TEXT("Fireing?"));
		if (ABBCharacter* FirstPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			if (FirstPlayer->ActualProjectiles > 0)
			{
				OnFireRequest();
			}
			else
			{
				UE_LOG(BimodularBotLog, Warning, TEXT("Not Enough Projectiles!"));
			}
		}
		else
		{
			UE_LOG(BimodularBotLog, Warning, TEXT("Second Player Doesnt Exist!"));
		}
	}
}

// source: https://dev.epicgames.com/documentation/en-us/unreal-engine/implementing-projectiles-in-unreal-engine
bool ABBCharacter::Fire() 
{
	if (ABBCharacter* FirstPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		FirstPlayer->ActualProjectiles--;
	}

	if (!ProjectileClass)
	{
		//UE_LOG(BimodularBotLog, Warning, TEXT("Projectiles not set in BP!"));
		//UE_LOG(BimodularBotLog, Warning, TEXT("Shoot not succesfull"));
		return false;
	}
	if (UWorld* World = GetWorld())
	{
		FVector MuzzleLocation = MuzzleArrow->GetComponentLocation() + MuzzleOffset;
		//UE_LOG(BimodularBotLog, Warning, TEXT("Location: %f %f %f"), MuzzleLocation.X, MuzzleLocation.Y, MuzzleLocation.Z);
		FRotator MuzzleRotation = GetControlRotation();
		//UE_LOG(BimodularBotLog, Warning, TEXT("Rotation: %f %f %f"), MuzzleRotation.Pitch, MuzzleRotation.Yaw, MuzzleRotation.Roll);
		FVector SpawnLocation = MuzzleLocation + MuzzleRotation.Vector() * 300.0f;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
		{
			FVector LaunchDirection = MuzzleRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
			OnFire();
			//UE_LOG(BimodularBotLog, Warning, TEXT("Shoot succesfull"));
			return true;
		}
	}
	//UE_LOG(BimodularBotLog, Warning, TEXT("Shoot not succesfull"));
	return false;
}

void ABBCharacter::PickUp(const FInputActionValue& Value)
{
	UE_LOG(BimodularBotLog, Warning, TEXT("PickUp action pressed!"));
	FVector Location;
	FRotator Rotation;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		Location = GetActorLocation();
		Rotation = GetActorRotation() + PickUpRotationOffset;
	}
	else
	{
		UE_LOG(BimodularBotLog, Warning, TEXT("PickUp action failed!"));
		return;
	}
	FVector Start = Location;
	const int32 UpNumRays = 15;
	const int32 LeftNumRays = 7;
	const float VerticalStep = 10.0f;
	const float HorizontalStep = 15.0f;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	for (int32 j = -LeftNumRays / 2; j <= LeftNumRays / 2; ++j)
	{
		for (int32 i = -UpNumRays / 2; i <= UpNumRays / 2; ++i)
		{
			FRotator CopyRotation = Rotation;
			float PitchOffset = i * VerticalStep;
			float YawOffset = j * HorizontalStep;
			FRotator AdjustedRot = CopyRotation.Add(PitchOffset, YawOffset, 0.f);
			FVector Direction = AdjustedRot.Vector();
			FVector End = Start + Direction * PickUpDistance;
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f);
			if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)) {
				if (Hit.GetActor())
				{
					if (AProjectile* Projectile = Cast<AProjectile>(Hit.GetActor()))
					{
						if (!IsMerged)
						{
							UE_LOG(BimodularBotLog, Warning, TEXT("You can PickUp bullets only if joined!"));
							return;
						}
						if (MaxProjectiles <= ActualProjectiles) {
							UE_LOG(BimodularBotLog, Warning, TEXT("You can PickUp only %d bullets!"), MaxProjectiles);
							return;
						}
						Projectile->OnPickedUp();
						OnPick();
						ActualProjectiles++;
						return;
					}
				}
			}
		}
	}
}

void ABBCharacter::UseStreaming(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Warning, TEXT("Streaming Use action pressed!"));
	if (ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1)))
	{
		SecondPlayer->Use(Value);
	}
}

#pragma region Movement & Jump

void ABBCharacter::MoveTriggered(const FInputActionValue& Value)
{
	if (Controller == nullptr)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ABBCharacter::JumpStarted(const FInputActionValue& Value)
{
	if (!IsMerged)
	{
		ACharacter::Jump();
	}
}

void ABBCharacter::JumpCompleted(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}

#pragma endregion

#pragma region Rotate

void ABBCharacter::RotateTriggered(const FInputActionValue& Value)
{
	if (Controller == nullptr)
		return;

	// Figure out who receives this in MergedMode
	if (IsMerged)
	{
		if (ABBPlayerController* PlayerController = Cast<ABBPlayerController>(Controller))
		{
			if (PlayerController->IsSecondSplitScreenPlayer())
			{
				// Input is consumed only for 2nd Player
				// Additionally this input goes to 1st Player
				// This is  too many ifs 
				if (ABBCharacter* FirstPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
				{
					FirstPlayer->SetRotationInput(Value);
				}
				// Same added to 2nd player, so they are in sync
				SetRotationInput(Value);
			}
		}
	}
	// Consume 
	else
	{
		SetRotationInput(Value);
	}
}

void ABBCharacter::SetRotationInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);

	FRotator ControlRotation = GetControlRotation();
	float NormalizedPitch = FRotator::NormalizeAxis(ControlRotation.Pitch);
	float NewPitch = FMath::Clamp(NormalizedPitch - LookAxisVector.Y, LowerBorder, UpperBorder);
	//UE_LOG(BimodularBotLog, Warning, TEXT("Location: %f %f %f"), NormalizedPitch, NewPitch, NormalizedPitch + LookAxisVector.Y);
	FRotator NewControlRotation = FRotator(NewPitch, ControlRotation.Yaw, ControlRotation.Roll);
	GetController()->SetControlRotation(NewControlRotation);
}

void ABBCharacter::RotateStreamingTriggered(const FInputActionValue& Value)
{
	if (Controller == nullptr)
		return;
	// Pass input to real owner = 2nd player
	if (ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1)))
	{
		SecondPlayer->RotateTriggered(Value);
	}
}


void ABBCharacter::SetupCamera()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));

	CameraSpringArm->SetupAttachment(RootComponent);
	// The camera follows at this distance behind the character
	CameraSpringArm->TargetArmLength = DefaultCameraDistance;
	// Rotate the arm based on the controller
	CameraSpringArm->bUsePawnControlRotation = true; 


	// Create a follow camera
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CharacterCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName); 
	// Camera does not rotate relative to arm
	CharacterCamera->bUsePawnControlRotation = false;

	// Setup Default orientation ?
	// CameraSpringArm->AddRelativeRotation(InitialRotation);
	CameraSpringArm->AddRelativeLocation(InitialDistance);
}

#pragma endregion

#pragma region Pause
void ABBCharacter::PauseTriggered(const FInputActionValue& Value)
{
	// This can be called only on first player, no matter who does it as it needs full authority over the game
	if (ABBCharacter* FirstPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		FirstPlayer->OnPause();
	}

}
#pragma endregion


#pragma region Divide & Join
void ABBCharacter::DivideAndJoinStarted(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Attempting Divide & Join %s."), *GetNameSafe(this));

	StartMergeAndSplit();
}

void ABBCharacter::DivideAndJoinOngoing(const FInputActionInstance& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Preparing Divide & Join %s."), *GetNameSafe(this));
	//UE_LOG(LogTemp, Log, TEXT("Input value: %f, Elapsed: %f, Triggered: %f"),
	//	Value.GetValue().Get<float>(), Value.GetElapsedTime(), Value.GetTriggeredTime());

	OnMergeAndSplitActivating(Value.GetElapsedTime());
}

void ABBCharacter::DivideAndJoinCompleted(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Cancelled Divide & Join %s."), *GetNameSafe(this));

	CancelMergeAndSplit();
}

void ABBCharacter::DivideAndJoinTriggered(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Ready Divide & Join %s."), *GetNameSafe(this));

	ReadyMergeAndSplit();
	TryMergeAndSplit();
}

void ABBCharacter::DivideAndJoinStreamingStarted(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Streaming Attempting Divide & Join %s."), *GetNameSafe(this));

	ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
	if (!SecondPlayer)
		return;
	SecondPlayer->DivideAndJoinStarted(Value);
}

void ABBCharacter::DivideAndJoinStreamingOngoing(const FInputActionInstance& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Streaming Preparing Divide & Join %s."), *GetNameSafe(this));

	ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
	if (!SecondPlayer)
		return;
	SecondPlayer->DivideAndJoinOngoing(Value);
}

void ABBCharacter::DivideAndJoinStreamingCompleted(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Streaming Cancelled Divide & Join %s."), *GetNameSafe(this));

	ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
	if (!SecondPlayer)
		return;
	SecondPlayer->DivideAndJoinCompleted(Value);
}

void ABBCharacter::DivideAndJoinStreamingTriggered(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Streaming Ready Divide & Join %s."), *GetNameSafe(this));

	ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
	if (!SecondPlayer)
		return;
	SecondPlayer->DivideAndJoinTriggered(Value);
}

void ABBCharacter::StartMergeAndSplit()
{
	IsDivideAndJoinEnabled = true;
	IsDivideAndJoinActive = false;
	OnMergeAndSplitActivating(0.0f);
}

void ABBCharacter::ReadyMergeAndSplit()
{
	IsDivideAndJoinActive = true;
	OnMergeAndSplitActivated();
}

void ABBCharacter::CancelMergeAndSplit()
{
	IsDivideAndJoinActive = false;
	OnMergeAndSplitCancelled();
}

void ABBCharacter::TryMergeAndSplit()
{
	// This needs to execute everything from both player perspectives

	// Both players must be ready
	ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));
	if (!SecondPlayer)
		return;

	ABBCharacter* FirstPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!FirstPlayer)
		return;

	if (!CanMerge)
	{
		//UE_LOG(BimodularBotLog, Log, TEXT("Players have big distance between them."));
		return;
	}

	if (!FirstPlayer->IsDivideAndJoinActive || !SecondPlayer->IsDivideAndJoinActive)
		return;

	// As the trigger is fired continuously, this prevents additional swaps from merge to split to merge...
	if (!FirstPlayer->IsDivideAndJoinEnabled || !SecondPlayer->IsDivideAndJoinEnabled)
		return;
	// Thus also for both it must be deactivated.
	SecondPlayer->IsDivideAndJoinEnabled = false;
	FirstPlayer->IsDivideAndJoinEnabled = false;

	// This executes merge and split
	if (IsMerged)
	{
		FirstPlayer->SplitBots();
		SecondPlayer->SplitBots();
	}
	else
	{
		FirstPlayer->MergeBots();
		SecondPlayer->MergeBots();
	}
}

void ABBCharacter::MergeBots()
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Merge started on %s."), *GetNameSafe(this));

	IsMerged = true;
	OnMerge();
}

void ABBCharacter::SplitBots()
{
	//UE_LOG(BimodularBotLog, Log, TEXT("Split started on %s."), *GetNameSafe(this));

	IsMerged = false;
	OnSplit();
}
#pragma endregion



void ABBCharacter::DebugButtonFirst(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Warning, TEXT("Debug action 1 pressed!"));
	OnDebugPressedFirst_BP();
}

void ABBCharacter::DebugButtonSecond(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Warning, TEXT("Debug action 2 pressed!"));
	OnDebugPressedSecond_BP();
}

void ABBCharacter::DebugButtonSecondStreaming(const FInputActionValue& Value)
{
	//UE_LOG(BimodularBotLog, Warning, TEXT("Streaming Debug Action 2 Preessed!"));
	if (ABBCharacter* SecondPlayer = Cast<ABBCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1)))
	{
		SecondPlayer->DebugButtonSecond(Value);
	}
}