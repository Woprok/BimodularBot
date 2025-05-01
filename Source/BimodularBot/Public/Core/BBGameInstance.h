// ©FIT Team

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BBGameInstance.generated.h"

// Forwards
class ABBPlayerController;
class ABBGameMode;

/**
 * GameInstance persist between levels, thus it's best play to keep functionality related to global
 * changes in the game.
 */
UCLASS()
class BIMODULARBOT_API UBBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	static TObjectPtr<UBBGameInstance> Get(TObjectPtr<UWorld> world);
	/** 
	 *	This destroyes references only in the game instance.
	 *	GameMode keeps references until the controller is logged out.
	 */
	UFUNCTION(Category = "CORE")
	void DestroyLocalPlayers();

	/** This is required to ensure we do not create additional players on leaving level. */
	UFUNCTION(Category = "CORE")
	void RegisterFirstSplitscreenPlayer(ABBPlayerController* player);
	/** This is required to ensure we do not create additional players on leaving level. */
	UFUNCTION(Category = "CORE")
	void RegisterSecondSplitscreenPlayer(ABBPlayerController* player);

	UFUNCTION(BlueprintCallable, Category = "CORE")
	ABBPlayerController* GetFirstSplitscreenPlayer();
	UFUNCTION(BlueprintCallable, Category = "CORE")
	ABBPlayerController* GetSecondSplitscreenPlayer();

	UFUNCTION(BlueprintCallable, Category = "CORE")
	bool IsFirstSplitscreenPlayer(ABBPlayerController* player);
	UFUNCTION(BlueprintCallable, Category = "CORE")
	bool IsSecondSplitscreenPlayer(ABBPlayerController* player);
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
	TObjectPtr<ABBPlayerController> FirstSplitscreenPlayer;
	TObjectPtr<ABBPlayerController> SecondSplitscreenPlayer;
};
