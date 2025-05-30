// ©FIT Team

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(BimodularBotLog, Log, All);

/**
 * Determines SplitScreenPlayer.
 */
UENUM(BlueprintType)
enum class EBBSplitScreenPlayer : uint8
{
	/**
	 *
	 */
	Undefined = 0,
	/**
	 *
	 */
	FirstPlayer = 1,
	/**
	 *
	 */
	SecondPlayer = 2,
};

/**
 * Determines SplitScreenPlayer character state.
 */
UENUM(BlueprintType)
enum class EBBPlayerCharacterState : uint8
{
	/**
	 *
	 */
	Undefined = 0,
	/**
	 *
	 */
	BottomBot = 1,
	/**
	 *
	 */
	TopBot = 2,
	/**
	 *
	 */
	CombinedBot = 3,
	/**
	 *
	 */
	CombinedBottomBot = 4,
	/**
	 *
	 */
	CombinedTopBot = 4,
};

/**
 * 
 */
class BIMODULARBOT_API BBUtils
{
public:
	BBUtils();
	~BBUtils();
};
