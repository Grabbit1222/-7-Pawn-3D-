#include "SpartaPawn.h"
#include "SpartaSpiderController.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ASpartaPawn::ASpartaPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box")); //박스 컴포넌트 생성
    RootComponent = BoxComp; //박스 컴포넌트 루트 설정

    BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //충돌 설정 활성화
    BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn); //오브젝트의 충돌 유형(채널): Pawn
    BoxComp->SetCollisionResponseToAllChannels(ECR_Block); // 충돌 응답: 움직임 차단


    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh")); //스켈레탈 메쉬 생성
    SkeletalMeshComp->SetupAttachment(BoxComp);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = true;



    bUseControllerRotationYaw = true;

    Gravity = -980.0f;

    NormalSpeed = 600.0f;
    CurrentSpeed = NormalSpeed;
    SprintSpeedMultiplier = 2.0f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    DecelerationFactor = 3.0f;    
}

void ASpartaPawn::BeginPlay()
{
    Super::BeginPlay();
    bIsFlying = false;
}

void ASpartaPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateRotation(DeltaTime);
    ApplyGravity(DeltaTime);
    ApplyMovement(DeltaTime);
}

void ASpartaPawn::UpdateRotation(float DeltaTime)
{
    if (IsOnFoot())
    {
        bIsFlying = false;
        TargetRotation = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
    }
    else
    {
        bIsFlying = true;
        TargetRotation.Pitch = FMath::FInterpTo(TargetRotation.Pitch, 0.0f, DeltaTime, 3.0f);
        TargetRotation.Roll = FMath::FInterpTo(TargetRotation.Roll, 0.0f, DeltaTime, 3.0f);
    }

    SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f));
}

void ASpartaPawn::ApplyGravity(float DeltaTime)
{
    if (!IsOnFoot())  // 지면에 닿아 있지 않을 때 중력 적용
    {
        CurrentVelocity.Z += Gravity * DeltaTime;  // 중력 적용
    }

    else if (CurrentVelocity.Z < 0)  // 착지 시 아래로 떨어지는 중일 때만 속도 초기화
    {
        CurrentVelocity.Z = 0.0f;
    }
}


void ASpartaPawn::ApplyMovement(float DeltaTime)
{
    if (!CurrentVelocity.IsNearlyZero(0.01f))
    {
        FVector MoveDelta = CurrentVelocity * DeltaTime;  // Z 이동 포함
        AddActorWorldOffset(MoveDelta, true);

        CurrentVelocity.X = FMath::FInterpTo(CurrentVelocity.X, 0.0f, DeltaTime, DecelerationFactor);
        CurrentVelocity.Y = FMath::FInterpTo(CurrentVelocity.Y, 0.0f, DeltaTime, DecelerationFactor);
    }
}

void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (ASpartaSpiderController* PlayerController = Cast<ASpartaSpiderController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::Move
                );
            }

            if (PlayerController->AscendAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->AscendAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::Ascend
                );
            }

            if (PlayerController->DescendAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->DescendAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::Descend
                );
            }

            /*if (PlayerController->RollAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->RollAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::Roll
                );
            }*/

            //if (PlayerController->JumpAction)
            //{
            //    // IA_Jump 액션 키를 "키를 누르고 있는 동안" StartJump() 호출
            //    EnhancedInput->BindAction(
            //        PlayerController->JumpAction,
            //        ETriggerEvent::Triggered,
            //        this,
            //        &ASpartaPawn::StartJump
            //    );

            //    // IA_Jump 액션 키에서 "손을 뗀 순간" StopJump() 호출
            //    EnhancedInput->BindAction(
            //        PlayerController->JumpAction,
            //        ETriggerEvent::Completed,
            //        this,
            //        &ASpartaPawn::StopJump
            //    );
            //}

            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                // IA_Sprint 액션 키를 "누르고 있는 동안" StartSprint() 호출
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::StartSprint
                );

                // IA_Sprint 액션 키에서 "손을 뗀 순간" StopSprint() 호출
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &ASpartaPawn::StopSprint
                );
            }
        }
    }
}


void ASpartaPawn::MoveForward(float value, float Speed)
{
    FVector ForwardVector = GetActorForwardVector();
    FVector MoveDirection = ForwardVector * value * Speed;

    CurrentVelocity.X = FMath::FInterpTo(CurrentVelocity.X, MoveDirection.X, GetWorld()->GetDeltaSeconds(), 2.0f);
    CurrentVelocity.Y = FMath::FInterpTo(CurrentVelocity.Y, MoveDirection.Y, GetWorld()->GetDeltaSeconds(), 2.0f);

    TargetRotation.Pitch = FMath::Clamp(value * -25.0f, -25.0f, 25.0f);
}

void ASpartaPawn::MoveRight(float Value, float Speed)
{
    FVector RightVector = GetActorRightVector();
    FVector MoveDirection = RightVector * Value * Speed;

    CurrentVelocity.X = FMath::FInterpTo(CurrentVelocity.X, MoveDirection.X, GetWorld()->GetDeltaSeconds(), 2.0f);
    CurrentVelocity.Y = FMath::FInterpTo(CurrentVelocity.Y, MoveDirection.Y, GetWorld()->GetDeltaSeconds(), 2.0f);

    TargetRotation.Roll = FMath::Clamp(Value * 25.0f, -90.0f, 90.0f);
}


void ASpartaPawn::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    MoveForward(MoveInput.X, CurrentSpeed);
    MoveRight(MoveInput.Y, CurrentSpeed);
}


void ASpartaPawn::StartSprint(const FInputActionValue& value)
{
    if (!bIsFlying)
    {
        CurrentSpeed = SprintSpeed;
    }
}

void ASpartaPawn::StopSprint(const FInputActionValue& value)
{
    
    CurrentSpeed = NormalSpeed;
}

void ASpartaPawn::Ascend(const FInputActionValue& value)
{
    if (!Controller) return;

    CurrentVelocity.Z = FMath::FInterpTo(CurrentVelocity.Z, value.Get<float>() * CurrentSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void ASpartaPawn::Descend(const FInputActionValue& value)
{
    if (!Controller) return;

    CurrentVelocity.Z = FMath::FInterpTo(CurrentVelocity.Z, -value.Get<float>() * CurrentSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}


void ASpartaPawn::Look(const FInputActionValue& value)
{
    if (!Controller) return;

    FVector2D LookInput = value.Get<FVector2D>();

    // 현재 컨트롤러의 회전값 가져오기
    FRotator CurrentRotation = Controller->GetControlRotation();

    // 새로운 회전값 계산
    FRotator NewRotation = CurrentRotation;
    NewRotation.Yaw += LookInput.X;   // 마우스 X 이동으로 Yaw 변경
    NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookInput.Y, -45.0f, 25.0f); // 마우스 Y 이동으로 Pitch 변경 (Pitch는 제한)

    // 컨트롤러 회전값 설정
    Controller->SetControlRotation(NewRotation);
}

bool ASpartaPawn::IsOnFoot()
{
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0.0f, 0.0f, TraceLength);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 2.0f);
    return GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
}