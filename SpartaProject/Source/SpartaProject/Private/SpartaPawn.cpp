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

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box")); //�ڽ� ������Ʈ ����
    RootComponent = BoxComp; //�ڽ� ������Ʈ ��Ʈ ����

    BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //�浹 ���� Ȱ��ȭ
    BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn); //������Ʈ�� �浹 ����(ä��): Pawn
    BoxComp->SetCollisionResponseToAllChannels(ECR_Block); // �浹 ����: ������ ����


    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh")); //���̷�Ż �޽� ����
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
    if (!IsOnFoot())  // ���鿡 ��� ���� ���� �� �߷� ����
    {
        CurrentVelocity.Z += Gravity * DeltaTime;  // �߷� ����
    }

    else if (CurrentVelocity.Z < 0)  // ���� �� �Ʒ��� �������� ���� ���� �ӵ� �ʱ�ȭ
    {
        CurrentVelocity.Z = 0.0f;
    }
}


void ASpartaPawn::ApplyMovement(float DeltaTime)
{
    if (!CurrentVelocity.IsNearlyZero(0.01f))
    {
        FVector MoveDelta = CurrentVelocity * DeltaTime;  // Z �̵� ����
        AddActorWorldOffset(MoveDelta, true);

        CurrentVelocity.X = FMath::FInterpTo(CurrentVelocity.X, 0.0f, DeltaTime, DecelerationFactor);
        CurrentVelocity.Y = FMath::FInterpTo(CurrentVelocity.Y, 0.0f, DeltaTime, DecelerationFactor);
    }
}

void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA�� �������� ���� ���� ���� ���� Controller�� ASpartaPlayerController�� ĳ����
        if (ASpartaSpiderController* PlayerController = Cast<ASpartaSpiderController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
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
            //    // IA_Jump �׼� Ű�� "Ű�� ������ �ִ� ����" StartJump() ȣ��
            //    EnhancedInput->BindAction(
            //        PlayerController->JumpAction,
            //        ETriggerEvent::Triggered,
            //        this,
            //        &ASpartaPawn::StartJump
            //    );

            //    // IA_Jump �׼� Ű���� "���� �� ����" StopJump() ȣ��
            //    EnhancedInput->BindAction(
            //        PlayerController->JumpAction,
            //        ETriggerEvent::Completed,
            //        this,
            //        &ASpartaPawn::StopJump
            //    );
            //}

            if (PlayerController->LookAction)
            {
                // IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                // IA_Sprint �׼� Ű�� "������ �ִ� ����" StartSprint() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ASpartaPawn::StartSprint
                );

                // IA_Sprint �׼� Ű���� "���� �� ����" StopSprint() ȣ��
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

    // ���� ��Ʈ�ѷ��� ȸ���� ��������
    FRotator CurrentRotation = Controller->GetControlRotation();

    // ���ο� ȸ���� ���
    FRotator NewRotation = CurrentRotation;
    NewRotation.Yaw += LookInput.X;   // ���콺 X �̵����� Yaw ����
    NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookInput.Y, -45.0f, 25.0f); // ���콺 Y �̵����� Pitch ���� (Pitch�� ����)

    // ��Ʈ�ѷ� ȸ���� ����
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