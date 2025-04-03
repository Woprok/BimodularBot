// ©FIT Team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BIMODULARBOT_API ABBGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABBGameMode();
protected:
	virtual void BeginPlay() override;
};
