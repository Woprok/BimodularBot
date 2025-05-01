// ©FIT Team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBUtils.h"
#include "BBPlayerController.generated.h"

/**
 * Default PC
 */
UCLASS()
class BIMODULARBOT_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "CORE")
	bool IsFirstSplitScreenPlayer() { return SplitScreenPlayer == EBBSplitScreenPlayer::FirstPlayer; };
	UFUNCTION(BlueprintCallable, Category = "CORE")
	bool IsSecondSplitScreenPlayer() { return SplitScreenPlayer == EBBSplitScreenPlayer::SecondPlayer; };

	UFUNCTION(BlueprintCallable, Category = "CORE")
	EBBSplitScreenPlayer GetSplitScreenPlayer() { return SplitScreenPlayer; }
	void SetSplitScreenPlayer(EBBSplitScreenPlayer InPlayer) { SplitScreenPlayer = InPlayer; };
private:
	EBBSplitScreenPlayer SplitScreenPlayer;
};
