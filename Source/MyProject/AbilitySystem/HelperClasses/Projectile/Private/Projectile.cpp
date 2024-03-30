// Fill out your copyright notice in the Description page of Project Settings.



#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/Projectile.h"

#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyProject/Characters/Public/BaseCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	

	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent) {
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Pawn"));

		if (HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("Server: Projectile spawned on server."));
			CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
		}
		{
			UE_LOG(LogTemp, Warning, TEXT("Client: Projectile spawned on client"));
		}
		
		CollisionComponent->InitSphereRadius(15.0f);

		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent) {
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 0.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent) {
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Static/Sphere.Sphere'"));
		if (Mesh.Succeeded()) {
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}

		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Static/SphereMaterial.SphereMaterial'"));
		if (Material.Succeeded()) {
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		}
		ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
		ProjectileMeshComponent->SetupAttachment(RootComponent);
	}

	if (!ProjectileParticleSystemComponent) {
		ProjectileParticleSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileParticleSystemComponent"));
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem>ParticleSystem(TEXT("'/Game/Static/NewNiagaraSystem.NewNiagaraSystem'"));
		if (ParticleSystem.Succeeded()) {
			ProjectileParticleSystemComponent->SetAsset(ParticleSystem.Object);
		}

		ProjectileParticleSystemComponent->SetupAttachment(RootComponent);
	}

	
	if (!ProjectileCollisionParticleSystem) {
		static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleSystem(TEXT("'/Game/Static/DefaultContent/StarterContent/Particles/P_Explosion.P_Explosion'"));
		if (ParticleSystem.Succeeded()) {
			ProjectileCollisionParticleSystem = ParticleSystem.Object;
		}
	}

	InitialLifeSpan = 60.0f;
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

void AProjectile::SetupSpawn(APawn* InstigatingPawn, UWeapon* NewWeapon, UStaticMesh* Mesh, UMaterial* Material,
	UNiagaraSystem* NiagaraSystem, UParticleSystem* ParticleSystem, float Gravity, float HitParticleLifeSpan)
{
	SetInstigator(InstigatingPawn);
	
	AttachToActor(GetInstigator(), FAttachmentTransformRules::KeepWorldTransform);

	if (ProjectileMovementComponent != nullptr)
	{
		ProjectileMovementComponent->ProjectileGravityScale = Gravity;
	}
	
	if (NewWeapon != nullptr)
	{
		Weapon = NewWeapon;
	}
	
	if (Mesh != nullptr)
	{
		ProjectileMeshComponent->SetStaticMesh(Mesh);
	}
	
	if (Material != nullptr)
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material, ProjectileMeshComponent);
		ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	}

	if (NiagaraSystem != nullptr)
	{
		ProjectileParticleSystemComponent->SetAsset(NiagaraSystem);
	}

	if (ParticleSystem != nullptr)
	{
		ProjectileCollisionParticleSystem = ParticleSystem;
	}
}


void AProjectile::Fire(FVector Velocity, float ProjectileLifeSpan) {
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	SetLifeSpan(ProjectileLifeSpan);
	ProjectileMovementComponent->Velocity = Velocity;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	
	/*
	if (ADissolveActor* DissolveActor = Cast<ADissolveActor>(OtherActor)) { // OtherComponent->IsSimulatingPhysics()
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *OtherActor->GetName()));
		DissolveActor->Dissolve();
		// OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	} */

	if (OtherActor != nullptr && OtherActor != this && OtherActor != GetInstigator()) {
		if (OtherComponent->IsSimulatingPhysics())
		{
			OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *OtherActor->GetName()));
		if (ABaseCharacter* EnemyHit = Cast<ABaseCharacter>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *EnemyHit->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("stat modifier not null")));
			EnemyHit->HandleHit(Hit.ImpactPoint + Hit.ImpactNormal, Hit.ImpactPoint, EAttackLevelEnum::AE_HeavyAttack, Weapon);
		}
	}

	ProjectileMovementComponent->Velocity = FVector(0.0f);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileCollisionParticleSystem, Hit.Location, FRotator::ZeroRotator, FVector(2.0f));

	ProjectileMeshComponent->DestroyComponent();

	ProjectileParticleSystemComponent->Deactivate();

	Destroy();
}
