// ©FIT Team


#include "Core/BBGameMode.h"
#include "Core/BBCharacter.h"
#include "Core/BBGameInstance.h"
#include "Core/BBGameState.h"
#include "Core/BBHUD.h"
#include "Core/BBPawn.h"
#include "Core/BBPlayerController.h"
#include "Core/BBPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "BBUtils.h"

ABBGameMode::ABBGameMode()
{
	GameStateClass = ABBGameState::StaticClass();
	PlayerControllerClass = ABBPlayerController::StaticClass();
	HUDClass = ABBHUD::StaticClass();
	PlayerStateClass = ABBPlayerState::StaticClass();
	DefaultPawnClass = ABBCharacter::StaticClass();
}

void ABBGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABBPlayerController* playerController = Cast<ABBPlayerController>(NewPlayer);
	if (!playerController)
		return;

	// Simply assign unique value to each player, so we have full control and double checking what is being done.

	if (FirstSplitscreenPlayer == nullptr)
	{
		FirstSplitscreenPlayer = playerController;
		playerController->SetSplitScreenPlayer(EBBSplitScreenPlayer::FirstPlayer);
		UBBGameInstance::Get(GetWorld())->RegisterFirstSplitscreenPlayer(playerController);
	}
	else if (SecondSplitscreenPlayer == nullptr)
	{
		SecondSplitscreenPlayer = playerController;
		playerController->SetSplitScreenPlayer(EBBSplitScreenPlayer::SecondPlayer);
		UBBGameInstance::Get(GetWorld())->RegisterSecondSplitscreenPlayer(playerController);
	}

	Super::PostLogin(NewPlayer);
}

void ABBGameMode::Logout(AController* Exiting)
{
	ABBPlayerController* playerController = Cast<ABBPlayerController>(Exiting);

	if (playerController->IsFirstSplitScreenPlayer())
	{
		UE_LOG(BimodularBotLog, Log, TEXT("Reset of First Bimodular Player."));
		FirstSplitscreenPlayer = nullptr;
	}
	else if (playerController->IsSecondSplitScreenPlayer())
	{
		UE_LOG(BimodularBotLog, Log, TEXT("Reset of Second Bimodular Player."));
		SecondSplitscreenPlayer = nullptr;
	}

	Super::Logout(Exiting);
}

UClass* ABBGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	ABBPlayerController* playerController = Cast<ABBPlayerController>(InController);
	if (playerController->IsFirstSplitScreenPlayer())
	{
		UE_LOG(BimodularBotLog, Log, TEXT("Setup of First Bimodular Player."));
		return FirstPawnClass;
	}
	if (playerController->IsSecondSplitScreenPlayer())
	{
		UE_LOG(BimodularBotLog, Log, TEXT("Setup of Second Bimodular Player."));
		return SecondPawnClass;
	}

	UE_LOG(BimodularBotLog, Error, TEXT("Failed to setup Bimodular Player Character."));
	return DefaultPawnClass;
}

void ABBGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateLocalPlayer();
}

void ABBGameMode::CreateLocalPlayer()
{
	UE_LOG(BimodularBotLog, Log, TEXT("Creating additional player for local co-op."));
	if (UGameplayStatics::CreatePlayer(this) == nullptr)
	{
		UE_LOG(BimodularBotLog, Error, TEXT("Failed to create additional player for local co-op."));
	}
}
