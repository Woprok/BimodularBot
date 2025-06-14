// ©FIT Team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "BBCharacter.generated.h"

// Forward Classes
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
struct FInputActionValue;
struct FInputActionInstance;

UCLASS()
class BIMODULARBOT_API ABBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABBCharacter();

private:
	ABBCharacter* SecondPlayerCharacter;
	ABBCharacter* FirstPlayerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void CheckPlayerDistance();
	FTimerHandle MergeCheckHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* MuzzleArrow;


	// Use
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseActionStreaming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FVector MuzzleOffset = FVector(0.0f,0.0f,0.0f);

	// PickUp
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float PickUpDistance = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FRotator PickUpRotationOffset = FRotator(0, 0, 0);



	// Jump
		
	// Debug First
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugActionFirstCharacter;
	UFUNCTION(BlueprintImplementableEvent, Category = "Debug")
	void OnDebugPressedFirst_BP();

	// Debug Second
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugActionSecondCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugActionSecondCharacterStreaming;
	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
	void OnDebugPressedSecond_BP();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	int MaxProjectiles = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	int ActualProjectiles = 0;
	UFUNCTION(BlueprintImplementableEvent, Category = "Shoot")
	void OnFireRequest();
	UFUNCTION(BlueprintImplementableEvent, Category = "Shoot")
	void OnFire();
	UFUNCTION(BlueprintCallable, Category = "Shoot")
	bool Fire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPick();

#pragma region Movement & Jump
public:
	// Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	// Jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
protected:
	void MoveTriggered(const FInputActionValue& Value);
	void JumpStarted(const FInputActionValue& Value);
	void JumpCompleted(const FInputActionValue& Value);
#pragma endregion

#pragma region Camera & Rotate
public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCamera;

	/** Rotate for both players */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateAction;
	/** Rotate Streaming to 2nd Player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateActionStreaming;

	/** Returns CameraSpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraSpringArm; }
	/** Returns CharacterCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CharacterCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DefaultCameraDistance = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MergedCameraDistance = 1000.f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//FRotator InitialRotation = FRotator(-35.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector InitialDistance = FVector(0.0f, 0.0f, 45.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float UpperBorder = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LowerBorder = -30.0f;
protected:
	/** Main action for both players */
	void RotateTriggered(const FInputActionValue& Value);
	void RotateStreamingTriggered(const FInputActionValue& Value);
private:
	void SetupCamera();
	void SetRotationInput(const FInputActionValue& Value);
#pragma endregion

#pragma region Pause
public:
	/** Anyone can start pause */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;
	/** Event called on pause started. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Divide & Join")
	void OnPause();
protected:
	void PauseTriggered(const FInputActionValue& Value);
#pragma endregion

#pragma region Divide & Join
public:
	/** Main action for both players */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DivideAndJoinAction;

	/** Main action for second player, if controlled by mouse */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DivideAndJoinActionStreaming;

public:
	/** Get the merged state. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Divide & Join", meta = (AllowPrivateAccess = "true"))
	bool IsMerged = false;
	/** Is bot prepared to be merge with other. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Divide & Join", meta = (AllowPrivateAccess = "true"))
	bool IsDivideAndJoinActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Divide & Join", meta = (AllowPrivateAccess = "true"))
	bool IsDivideAndJoinEnabled = false;
	/** Max distance in which can players be merged together */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Divide & Join", meta = (AllowPrivateAccess = "true"))
	float MaxDistance = 1000.0f;
	/** Boolean which says about posibility to be merged */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Divide & Join", meta = (AllowPrivateAccess = "true"))
	bool CanMerge = false;
	/** Event called on merge. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Divide & Join")
	void OnMerge();
	/** Event called on split. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Divide & Join")
	void OnSplit();
	/** Event called on merge and split input being used. (Range is 0.0 - to not max of progress needed) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Divide & Join")
	void OnMergeAndSplitActivating(float TimeElapsed);
	/** Event called on merge and split input ready to be used. (Range is max) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Divide & Join")
	void OnMergeAndSplitActivated();
	/** Event called on merge and split input cancelled. (Range is -1.0) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Divide & Join")
	void OnMergeAndSplitCancelled();
protected:
	// Base actions that execute all the logic
	void DivideAndJoinStarted(const FInputActionValue& Value);
	void DivideAndJoinOngoing(const FInputActionInstance& Value);
	void DivideAndJoinCompleted(const FInputActionValue& Value);
	void DivideAndJoinTriggered(const FInputActionValue& Value);
	// Streaming actions are just calling the base
	void DivideAndJoinStreamingStarted(const FInputActionValue& Value);
	void DivideAndJoinStreamingOngoing(const FInputActionInstance& Value);
	void DivideAndJoinStreamingCompleted(const FInputActionValue& Value);
	void DivideAndJoinStreamingTriggered(const FInputActionValue& Value);

	virtual void StartMergeAndSplit();
	virtual void ReadyMergeAndSplit();
	virtual void CancelMergeAndSplit();
	virtual void TryMergeAndSplit();
	virtual void MergeBots();
	virtual void SplitBots();

#pragma endregion

private:
	// Pick == Red Robot Ammo Collection
	void PickUp(const FInputActionValue& Value);
	// Use == Shoot
	void UseStreaming(const FInputActionValue& Value);
public:
	// Use == Shoot
	void Use(const FInputActionValue& Value);

private:
	// Debug
	void DebugButtonFirst(const FInputActionValue& Value);
	void DebugButtonSecondStreaming(const FInputActionValue& Value);
public:
	// Debug
	void DebugButtonSecond(const FInputActionValue& Value);

};
