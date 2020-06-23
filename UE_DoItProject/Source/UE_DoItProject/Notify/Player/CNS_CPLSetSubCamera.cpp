#include "CNS_CPLSetSubCamera.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_BlendCameraComp.h"
#include "Camera/CPL_BlendCameraActor.h"

void UCNS_CPLSetSubCamera::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	IfNullRet(Player);

	PlayerController = Cast<APlayerController>(Player->GetController());
	IfNullRet(PlayerController);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@카메라 전환
	AActor* BlendCameraActor = Player->GetBlendCameraComp()->GetBlendCamera(BlendPosType);
	if (BlendCameraActor != nullptr)
	{
		if (bUsingTransform == true)
		{
			USceneComponent* SceneComp = BlendCameraActor->GetRootComponent();
			if (SceneComp != nullptr)
			{
				//FTransform BlendActorTransform = SceneComp->GetRelativeLocation();
				//@Get
				FVector		BlendActorLocation = SceneComp->GetRelativeLocation();
				//FRotator	BlendActorRotation = BlendActorTransform.GetRotation().Rotator();

				CLog::Print(BlendActorLocation);

				//@Calc
				BlendActorLocation += LocationOffset;
				//BlendActorRotation += RotationOffset;

				SceneComp->SetRelativeLocation(BlendActorLocation);
				//BlendCameraActor->SetActorRotation(BlendActorRotation);
			}
		}

		PlayerController->SetViewTargetWithBlend(BlendCameraActor, StartBlendTime);
	}
	else
		UE_LOG(LogTemp, Warning, L"CNS_CPLSetSubCamera - BlendCamera Null!!");

	if (bUsingGlobalTimer == true)
	{
		//@세계 시간 느리게 적용.
		UWorld* const World = Player->GetWorld();
		UGameplayStatics::SetGlobalTimeDilation(World, GlobalTimeAttack);
	}
}

void UCNS_CPLSetSubCamera::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCNS_CPLSetSubCamera::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	IfNullRet(Player);

	PlayerController = Cast<APlayerController>(Player->GetController());
	IfNullRet(PlayerController);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Camera 원상복귀
	if (bUsingEndBlendOrigin == true)
	{
		PlayerController->SetViewTargetWithBlend(Player, EndBlendTime);
	}

	if (bUsingGlobalTimer == true)
	{
		//@원상 복귀
		UWorld* const World = Player->GetWorld();
		UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);
	}

	//@Default Reset
	UCPL_BlendCameraComp* BlendCameraComp = Player->GetBlendCameraComp();
	if (BlendCameraComp != nullptr && bUsingTransform == true)
	{
		BlendCameraComp->AllResetBlendCameraDefaultValue();
	}
}
