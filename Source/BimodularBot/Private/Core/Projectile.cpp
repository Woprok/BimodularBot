// ©FIT Team

// source: https://dev.epicgames.com/documentation/en-us/unreal-engine/implementing-projectiles-in-unreal-engine

#include "Core/Projectile.h"
#include "BBUtils.h"
#include <Kismet/GameplayStatics.h>

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }
    if (!CollisionComponent)
    {
        // Use a sphere as a simple collision representation.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // Set the sphere's collision radius.
        CollisionComponent->InitSphereRadius(10.0f);
        // Set the root component to be the collision component.
        RootComponent = CollisionComponent;
    }
    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->MaxSpeed = MaxSpeed;
        ProjectileMovementComponent->bRotationFollowsVelocity = bRotationFollowsVelocity;
        ProjectileMovementComponent->bShouldBounce = bShouldBounce;
        ProjectileMovementComponent->Bounciness = Bounciness;
        ProjectileMovementComponent->ProjectileGravityScale = ProjectileGravityScale;
    }
    if (!ProjectileMeshComponent)
    {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
        if (Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
            ProjectileMeshComponent->SetupAttachment(RootComponent);
        }
    }
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function that initializes the projectile's velocity in the shoot direction.
void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
    if (Mesh && Mesh->IsSimulatingPhysics())
    {
        Mesh->AddImpulse(ShootDirection * InitialSpeed, NAME_None, true);
    }

    //ProjectileMovementComponent->Velocity = ShootDirection * InitialSpeed;
}

void AProjectile::OnPickedUp()
{
    Destroy();
}
