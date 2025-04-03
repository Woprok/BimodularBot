// ©FIT Team


#include "Core/BBActor.h"

// Sets default values
ABBActor::ABBActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABBActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABBActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

