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

void UBBGameInstance::RegisterFirstSplitscreenPlayer(ABBPlayerController* player)
{
	FirstSplitscreenPlayer = player;
}
void UBBGameInstance::RegisterSecondSplitscreenPlayer(ABBPlayerController* player)
{
	SecondSplitscreenPlayer = player;
}

void UBBGameInstance::DestroyLocalPlayers()
{
	if (IsValid(SecondSplitscreenPlayer))
		UGameplayStatics::RemovePlayer(SecondSplitscreenPlayer, true);
	FirstSplitscreenPlayer = nullptr;
	SecondSplitscreenPlayer = nullptr;
}

ABBPlayerController* UBBGameInstance::GetFirstSplitscreenPlayer()
{
	return FirstSplitscreenPlayer;
}

ABBPlayerController* UBBGameInstance::GetSecondSplitscreenPlayer()
{
	return SecondSplitscreenPlayer;
}

bool UBBGameInstance::IsFirstSplitscreenPlayer(ABBPlayerController* player)
{
	return FirstSplitscreenPlayer == player;
}

bool UBBGameInstance::IsSecondSplitscreenPlayer(ABBPlayerController* player)
{
	return SecondSplitscreenPlayer == player;
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