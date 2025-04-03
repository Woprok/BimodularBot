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

ABBGameMode::ABBGameMode()
{
	GameStateClass = ABBGameState::StaticClass();
	PlayerControllerClass = ABBPlayerController::StaticClass();
	HUDClass = ABBHUD::StaticClass();
	PlayerStateClass = ABBPlayerState::StaticClass();
	DefaultPawnClass = ABBCharacter::StaticClass();
}

void ABBGameMode::BeginPlay()
{
	Super::BeginPlay();
	// We can safely assume that 0 is local player controller as this is meant to be local coop only
	// WARNING: LOCAL COOP ONLY ASSUMPTION
	ABBPlayerController* playerController = Cast<ABBPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	UBBGameInstance::Get(GetWorld())->RegisterExistingLocalPlayer(playerController);
	UBBGameInstance::Get(GetWorld())->RegisterNewLocalPlayer(this);
}
