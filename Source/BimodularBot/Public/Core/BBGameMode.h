// ©FIT Team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameMode.generated.h"

// Forward Declarations
class ABBPlayerController;

/**
 * 
 */
UCLASS()
class BIMODULARBOT_API ABBGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABBGameMode();
	/** Redefines how the post login is done. */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/** Redefines how the post login is done. */
	virtual void Logout(AController* Exiting) override;
	/** Redefines pawn used by players. */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	/** The default pawn class used by SplitScreenPlayer 1. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<APawn> FirstPawnClass;
	/** The default pawn class used by SplitScreenPlayer 2. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<APawn> SecondPawnClass;
	
	/** Get first player */
	UFUNCTION(BlueprintCallable, Category = "CORE")
	ABBPlayerController* GetFirstSplitScreenPlayer() { return FirstSplitscreenPlayer; };
	/** Get first player */
	UFUNCTION(BlueprintCallable, Category = "CORE")
	ABBPlayerController* GetSecondSplitScreenPlayer() { return SecondSplitscreenPlayer; };
protected:
	virtual void BeginPlay() override;
	virtual void CreateLocalPlayer();
private:
	TObjectPtr<ABBPlayerController> FirstSplitscreenPlayer = nullptr;
	TObjectPtr<ABBPlayerController> SecondSplitscreenPlayer = nullptr;
};
