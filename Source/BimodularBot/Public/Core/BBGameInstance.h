// ©FIT Team

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BBGameInstance.generated.h"

// Forwards
class ABBPlayerController;
class ABBGameMode;

/**
 * 
 */
UCLASS()
class BIMODULARBOT_API UBBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	static TObjectPtr<UBBGameInstance> Get(TObjectPtr<UWorld> world);
	UFUNCTION(Category = "CORE")
	void RegisterExistingLocalPlayer(ABBPlayerController* player);
	UFUNCTION(Category = "CORE")
	void RegisterNewLocalPlayer(const ABBGameMode* gamemode);
	UFUNCTION(Category = "CORE")
	void DestroyLocalPlayers();
	UFUNCTION(BlueprintCallable, Category = "CORE")
	ABBPlayerController* GetFirstLocalPlayer();
	UFUNCTION(BlueprintCallable, Category = "CORE")
	ABBPlayerController* GetSecondLocalPlayer();
public:
	/**
	 * Quits the game.
	 */
	UFUNCTION(BlueprintCallable, Category = "CORE")
	void Quit();
	/**
	 * Travels to associated level used for game.
	 */
	UFUNCTION(BlueprintCallable, Category = "CORE")
	void OpenGameLevel(const TSoftObjectPtr<UWorld> Level, FString options);
	/**
	 * Travels to associated level used for menu / non game state.
	 */
	UFUNCTION(BlueprintCallable, Category = "CORE")
	void OpenMenuLevel(const TSoftObjectPtr<UWorld> Level, FString options);
private:
	TObjectPtr<ABBPlayerController> firstPlayer;
	TObjectPtr<ABBPlayerController> secondPlayer;
};
