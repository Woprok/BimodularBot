// ©FIT Team


#include "Core/BBGameInstance.h"
#include "Core/BBGameMode.h"
#include "Core/BBPlayerController.h"
#include "Kismet/GameplayStatics.h"

TObjectPtr<UBBGameInstance> UBBGameInstance::Get(TObjectPtr<UWorld> world)
{
	TObjectPtr<UGameInstance> gameInstance = UGameplayStatics::GetGameInstance(world);
	check(gameInstance != nullptr);
	return CastChecked<UBBGameInstance>(gameInstance);
}

void UBBGameInstance::RegisterExistingLocalPlayer(ABBPlayerController* player)
{
	firstPlayer = player;
}

void UBBGameInstance::RegisterNewLocalPlayer(const ABBGameMode* gamemode)
{
	secondPlayer = Cast<ABBPlayerController>(UGameplayStatics::CreatePlayer(gamemode));
}

void UBBGameInstance::DestroyLocalPlayers()
{
	if (IsValid(secondPlayer))
		UGameplayStatics::RemovePlayer(secondPlayer, true);
	secondPlayer = nullptr;
	firstPlayer = nullptr;
}

ABBPlayerController* UBBGameInstance::GetFirstLocalPlayer()
{
	return firstPlayer;
}

ABBPlayerController* UBBGameInstance::GetSecondLocalPlayer()
{
	return secondPlayer;
}

void UBBGameInstance::Quit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UBBGameInstance::OpenGameLevel(const TSoftObjectPtr<UWorld> Level, FString options)
{
	// We need to destroy all remaining local players.
	DestroyLocalPlayers();
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level, true, options);
}

void UBBGameInstance::OpenMenuLevel(const TSoftObjectPtr<UWorld> Level, FString options)
{
	// We need to destroy all remaining local players.
	DestroyLocalPlayers();
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level, true, options);
}