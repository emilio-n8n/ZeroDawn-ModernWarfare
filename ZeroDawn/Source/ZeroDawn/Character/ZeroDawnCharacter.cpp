#include "ZeroDawnCharacter.h"
#include "../Weapon/ZeroDawnWeapon.h"
#include "../WeaponSystem/ZeroDawnWeaponComponent.h"
#include "../WeaponSystem/ZeroDawnWeaponInventory.h"
#include "../WeaponSystem/ZeroDawnWeaponInspect.h"
#include "../WeaponSystem/ZeroDawnCameraShake.h"
#include "../Progression/ZeroDawnKillstreakManager.h"
#include "../Progression/ZeroDawnProgressionManager.h"
#include "../Progression/ZeroDawnPerkSystem.h"
#include "../Multiplayer/ZeroDawnPlayerState.h"
#include "../UI/ZeroDawnHUD.h"
#include "../UI/ZeroDawnHitmarker.h"
#include "../Interactive/ZeroDawnInteractable.h"
#include "../Interactive/ZeroDawnBomb.h"
#include "DrawDebugHelpers.h"

AZeroDawnCharacter::AZeroDawnCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 48.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCapsuleComponent()->InitCapsuleSize(34.0f, 88.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_No;

	WeaponComponent = CreateDefaultSubobject<UZeroDawnWeaponComponent>(TEXT("WeaponComponent"));
	WeaponInventory = CreateDefaultSubobject<UZeroDawnWeaponInventory>(TEXT("WeaponInventory"));

	KillstreakManager = CreateDefaultSubobject<UZeroDawnKillstreakManager>(TEXT("KillstreakManager"));
	ProgressionManager = CreateDefaultSubobject<UZeroDawnProgressionManager>(TEXT("ProgressionManager"));
	PerkSystem = CreateDefaultSubobject<UZeroDawnPerkSystem>(TEXT("PerkSystem"));

	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 100.0f;
	NetPriority = 3.0f;

	// Load input assets via ConstructorHelpers (must be in constructor for packaged builds)
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_FPS(TEXT("/Game/ZeroDawn/Input/IMC_FPS.IMC_FPS"));
	if (IMC_FPS.Succeeded())
	{
		InputMapping = IMC_FPS.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Move(TEXT("/Game/ZeroDawn/Input/Actions/IA_Move.IA_Move"));
	if (IA_Move.Succeeded()) MoveAction = IA_Move.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Look(TEXT("/Game/ZeroDawn/Input/Actions/IA_Look.IA_Look"));
	if (IA_Look.Succeeded()) LookAction = IA_Look.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Sprint(TEXT("/Game/ZeroDawn/Input/Actions/IA_Sprint.IA_Sprint"));
	if (IA_Sprint.Succeeded()) SprintAction = IA_Sprint.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Crouch(TEXT("/Game/ZeroDawn/Input/Actions/IA_Crouch.IA_Crouch"));
	if (IA_Crouch.Succeeded()) CrouchAction = IA_Crouch.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Jump(TEXT("/Game/ZeroDawn/Input/Actions/IA_Jump.IA_Jump"));
	if (IA_Jump.Succeeded()) JumpAction = IA_Jump.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Slide(TEXT("/Game/ZeroDawn/Input/Actions/IA_Slide.IA_Slide"));
	if (IA_Slide.Succeeded()) SlideAction = IA_Slide.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ADS(TEXT("/Game/ZeroDawn/Input/Actions/IA_ADS.IA_ADS"));
	if (IA_ADS.Succeeded()) ADSAction = IA_ADS.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Fire(TEXT("/Game/ZeroDawn/Input/Actions/IA_Fire.IA_Fire"));
	if (IA_Fire.Succeeded()) FireAction = IA_Fire.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Reload(TEXT("/Game/ZeroDawn/Input/Actions/IA_Reload.IA_Reload"));
	if (IA_Reload.Succeeded()) ReloadAction = IA_Reload.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Interact(TEXT("/Game/ZeroDawn/Input/Actions/IA_Interact.IA_Interact"));
	if (IA_Interact.Succeeded()) InteractAction = IA_Interact.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Melee(TEXT("/Game/ZeroDawn/Input/Actions/IA_Melee.IA_Melee"));
	if (IA_Melee.Succeeded()) MeleeAction = IA_Melee.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Inspect(TEXT("/Game/ZeroDawn/Input/Actions/IA_Inspect.IA_Inspect"));
	if (IA_Inspect.Succeeded()) InspectAction = IA_Inspect.Object;
}

void AZeroDawnCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentArmor = 0.0f;
	bIsDead = false;

	if (IsLocallyControlled())
	{
		CacheInputActions();
		WeaponComponent->SpawnDefaultWeapons();
	}

	if (HasAuthority())
	{
		SetTeam(TeamType);
	}
}

void AZeroDawnCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSliding)
	{
		float SlideElapsed = GetWorld()->GetTimeSeconds() - SlideStartTime;
		if (SlideElapsed >= SlideDuration)
		{
			StopCrouch();
		}
	}
}

void AZeroDawnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;

	SetupInputs();
}

void AZeroDawnCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AZeroDawnCharacter, CurrentHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AZeroDawnCharacter, CurrentArmor, COND_OwnerOnly);
	DOREPLIFETIME(AZeroDawnCharacter, TeamType);
	DOREPLIFETIME(AZeroDawnCharacter, Kills);
	DOREPLIFETIME(AZeroDawnCharacter, Deaths);
	DOREPLIFETIME(AZeroDawnCharacter, Assists);
	DOREPLIFETIME(AZeroDawnCharacter, Score);
	DOREPLIFETIME(AZeroDawnCharacter, CurrentKillstreak);
	DOREPLIFETIME(AZeroDawnCharacter, bIsSliding);
	DOREPLIFETIME_CONDITION(AZeroDawnCharacter, KillerActor, COND_OwnerOnly);
}

float AZeroDawnCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead || !HasAuthority()) return 0.0f;

	float ActualDamage = DamageAmount;

	if (CurrentArmor > 0.0f)
	{
		float ArmorAbsorb = FMath::Min(CurrentArmor, ActualDamage * 0.7f);
		CurrentArmor -= ArmorAbsorb;
		ActualDamage -= ArmorAbsorb;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		KillerActor = DamageCauser ? DamageCauser->GetOwner() : nullptr;

		AZeroDawnCharacter* AttackerChar = Cast<AZeroDawnCharacter>(EventInstigator ? EventInstigator->GetPawn() : nullptr);
		if (AttackerChar && AttackerChar != this)
		{
			AttackerChar->AddKill();
		}

		Die();
	}

	return ActualDamage;
}

void AZeroDawnCharacter::Die()
{
	if (bIsDead) return;
	bIsDead = true;

	AddDeath();

	// Drop the bomb if carrying it, and cancel any defuse in progress
	if (HasAuthority())
	{
		for (TActorIterator<AZeroDawnBomb> It(GetWorld()); It; ++It)
		{
			AZeroDawnBomb* Bomb = *It;
			if (!Bomb) continue;

			if (Bomb->CarriedByCharacter == this)
			{
				Bomb->DropBomb();
			}
			if (Bomb->InteractingDefuser == this)
			{
				Bomb->CancelDefuse();
			}
		}
	}

	MulticastOnDeath();
	MulticastPlayDeathEffect();

	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AZeroDawnCharacter::RequestRespawn, RespawnDelay, false);
}

void AZeroDawnCharacter::MulticastOnDeath_Implementation()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (WeaponComponent)
	{
		WeaponComponent->HideWeapons();
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		UZeroDawnCameraShake::PlayExplosionShake(PC);
	}

	if (FirstPersonCameraComponent)
	{
		FirstPersonCameraComponent->bUsePawnControlRotation = false;
	}
}

void AZeroDawnCharacter::RequestRespawn()
{
	if (HasAuthority())
	{
		ServerRequestRespawn();
	}
	else
	{
		ServerRequestRespawn();
	}
}

bool AZeroDawnCharacter::ServerRequestRespawn_Validate() { return true; }
void AZeroDawnCharacter::ServerRequestRespawn_Implementation()
{
	AZeroDawnPlayerState* PS = GetPlayerState<AZeroDawnPlayerState>();
	if (PS)
	{
		PS->RequestRespawn();
	}
}

void AZeroDawnCharacter::OnRep_CurrentHealth()
{
	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		Die();
	}

	if (HitSound && CurrentHealth > 0.0f)
	{
		UGameplayStatics::PlaySound2D(this, HitSound);
	}
}

void AZeroDawnCharacter::AddHealth(float Amount)
{
	if (HasAuthority())
	{
		CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	}
}

void AZeroDawnCharacter::AddArmor(float Amount)
{
	if (HasAuthority())
	{
		CurrentArmor = FMath::Clamp(CurrentArmor + Amount, 0.0f, MaxArmor);
	}
}

void AZeroDawnCharacter::AddKill()
{
	if (!HasAuthority()) return;
	Kills++;
	CurrentKillstreak++;
	Score += 100;

	if (KillstreakManager)
	{
		KillstreakManager->CheckKillstreaks(CurrentKillstreak);
	}

	UZeroDawnHitmarker* Hitmarker = FindComponentByClass<UZeroDawnHitmarker>();
	if (Hitmarker)
	{
		Hitmarker->MulticastKillConfirmation();
	}
}

void AZeroDawnCharacter::AddDeath()
{
	if (!HasAuthority()) return;
	Deaths++;
	CurrentKillstreak = 0;
}

void AZeroDawnCharacter::AddAssist()
{
	if (!HasAuthority()) return;
	Assists++;
	Score += 25;
}

void AZeroDawnCharacter::AddScore(int32 Amount)
{
	if (!HasAuthority()) return;
	Score += Amount;
}

void AZeroDawnCharacter::MulticastPlayHitEffect_Implementation(FVector HitLocation, FRotator HitRotation)
{
	if (BloodImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, HitLocation, HitRotation, true);
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitLocation);
	}
}

void AZeroDawnCharacter::MulticastPlayDeathEffect_Implementation()
{
	FVector DeathLocation = GetActorLocation();
	FRotator DeathRotation = GetActorRotation();

	if (BloodImpactEffect)
	{
		FVector BloodLocation = DeathLocation + FVector(0.0f, 0.0f, 50.0f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, BloodLocation, DeathRotation, true);
	}

	if (DeathExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathExplosionEffect, DeathLocation, DeathRotation, true);
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, DeathLocation);
	}
}

void AZeroDawnCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AZeroDawnCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AZeroDawnCharacter::CacheInputActions()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem) return;

	if (InputMapping)
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}
}

void AZeroDawnCharacter::SetupInputs()
{
	if (!EnhancedInputComponent) return;

	if (MoveAction) EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZeroDawnCharacter::Move);
	if (LookAction) EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZeroDawnCharacter::Look);
	if (SprintAction)
	{
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AZeroDawnCharacter::StopSprint);
	}
	if (CrouchAction)
	{
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AZeroDawnCharacter::StopCrouch);
	}
	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AZeroDawnCharacter::StopJump);
	}
	if (SlideAction) EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::StartSlide);
	if (ADSAction)
	{
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::StartADS);
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Completed, this, &AZeroDawnCharacter::StopADS);
	}
	if (FireAction)
	{
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AZeroDawnCharacter::StopFire);
	}
	if (ReloadAction) EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::Reload);
	if (InteractAction) EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::Interact);
	if (MeleeAction) EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::Melee);
	if (InspectAction) EnhancedInputComponent->BindAction(InspectAction, ETriggerEvent::Started, this, &AZeroDawnCharacter::InspectWeapon);
}

void AZeroDawnCharacter::Move(const FInputActionValue& Value)
{
	if (bIsDead) return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), MovementVector.Y);
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), MovementVector.X);
	}
}

void AZeroDawnCharacter::Look(const FInputActionValue& Value)
{
	if (bIsDead) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AZeroDawnCharacter::StartSprint()
{
	if (bIsDead || bIsADS || bIsCrouching) return;

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	ServerSetSprinting(true);
}

void AZeroDawnCharacter::StopSprint()
{
	bIsSprinting = false;
	LastSprintEndTime = GetWorld()->GetTimeSeconds();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	ServerSetSprinting(false);
}

void AZeroDawnCharacter::StartCrouch()
{
	if (bIsDead) return;

	if (bIsSliding) return;

	if (!bIsCrouching && GetCharacterMovement()->IsMovingOnGround() && bIsSprinting)
	{
		StartSlide();
		return;
	}

	bIsCrouching = true;
	ACharacter::Crouch();
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	ServerSetCrouching(true);
}

void AZeroDawnCharacter::StopCrouch()
{
	bIsCrouching = false;
	bIsSliding = false;
	ACharacter::UnCrouch();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	ServerSetCrouching(false);
	ServerSetSliding(false);
}

void AZeroDawnCharacter::StartJump()
{
	if (bIsDead) return;
	ACharacter::Jump();
}

void AZeroDawnCharacter::StopJump()
{
	ACharacter::StopJumping();
}

void AZeroDawnCharacter::StartSlide()
{
	if (!bCanSlide || bIsDead || !GetCharacterMovement()->IsMovingOnGround()) return;

	if (!bIsCrouching)
	{
		ACharacter::Crouch();
	}

	bIsSliding = true;
	bCanSlide = false;
	SlideStartTime = GetWorld()->GetTimeSeconds();
	GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;

	FVector SlideDirection = GetLastMovementInputVector();
	if (SlideDirection.IsNearlyZero())
	{
		SlideDirection = GetActorForwardVector();
	}
	GetCharacterMovement()->AddImpulse(SlideDirection * 300.0f, true);

	ServerSetSliding(true);

	FTimerHandle SlideCooldownHandle;
	GetWorldTimerManager().SetTimer(SlideCooldownHandle, [this]()
	{
		bCanSlide = true;
	}, SlideCooldown, false);
}

void AZeroDawnCharacter::StartADS()
{
	if (bIsDead || bIsSprinting) return;
	bIsADS = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * ADSSpeedMultiplier;
	ServerSetADS(true);
}

void AZeroDawnCharacter::StopADS()
{
	bIsADS = false;
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
	ServerSetADS(false);
}

void AZeroDawnCharacter::Fire()
{
	if (bIsDead) return;

	if (bIsSprinting)
	{
		StopSprint();
	}

	float TimeSinceSprintEnd = GetWorld()->GetTimeSeconds() - LastSprintEndTime;
	if (TimeSinceSprintEnd < SprintToFireTime) return;

	ServerFire();
}

void AZeroDawnCharacter::StopFire()
{
	ServerStopFire();
}

void AZeroDawnCharacter::Reload()
{
	if (bIsDead) return;
	ServerReload();
}

void AZeroDawnCharacter::Interact()
{
	if (bIsDead) return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * InteractionRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UZeroDawnInteractable::StaticClass()))
		{
			IZeroDawnInteractable::Execute_Interact(HitActor, this);
		}
	}

	// Debug line trace (visible in editor with "showdebug traces" or similar)
	#if !UE_BUILD_SHIPPING
	DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, HitResult.bBlockingHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 1.0f);
	#endif
}

void AZeroDawnCharacter::InspectWeapon()
{
	if (bIsDead) return;
	UZeroDawnWeaponInspect* Inspect = FindComponentByClass<UZeroDawnWeaponInspect>();
	if (Inspect)
	{
		Inspect->StartInspect();
	}
}

void AZeroDawnCharacter::Melee()
{
	if (bIsDead) return;
	ServerMelee();
}

bool AZeroDawnCharacter::ServerSetSprinting_Validate(bool bNewSprinting) { return true; }
void AZeroDawnCharacter::ServerSetSprinting_Implementation(bool bNewSprinting)
{
	bIsSprinting = bNewSprinting;
}

bool AZeroDawnCharacter::ServerSetADS_Validate(bool bNewADS) { return true; }
void AZeroDawnCharacter::ServerSetADS_Implementation(bool bNewADS)
{
	bIsADS = bNewADS;
}

bool AZeroDawnCharacter::ServerSetCrouching_Validate(bool bNewCrouching) { return true; }
void AZeroDawnCharacter::ServerSetCrouching_Implementation(bool bNewCrouching)
{
	bIsCrouching = bNewCrouching;
}

bool AZeroDawnCharacter::ServerSetSliding_Validate(bool bNewSliding) { return true; }
void AZeroDawnCharacter::ServerSetSliding_Implementation(bool bNewSliding)
{
	bIsSliding = bNewSliding;
	if (bNewSliding)
	{
		SlideStartTime = GetWorld()->GetTimeSeconds();
	}
}

bool AZeroDawnCharacter::ServerFire_Validate() { return true; }
void AZeroDawnCharacter::ServerFire_Implementation()
{
	if (WeaponComponent)
	{
		WeaponComponent->StartFire();
	}
}

bool AZeroDawnCharacter::ServerStopFire_Validate() { return true; }
void AZeroDawnCharacter::ServerStopFire_Implementation()
{
	if (WeaponComponent)
	{
		WeaponComponent->StopFire();
	}
}

bool AZeroDawnCharacter::ServerReload_Validate() { return true; }
void AZeroDawnCharacter::ServerReload_Implementation()
{
	if (WeaponComponent)
	{
		WeaponComponent->ReloadCurrentWeapon();
	}
}

bool AZeroDawnCharacter::ServerMelee_Validate() { return true; }
void AZeroDawnCharacter::ServerMelee_Implementation()
{
	if (WeaponComponent)
	{
		WeaponComponent->MeleeAttack();
	}
}

void AZeroDawnCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}
