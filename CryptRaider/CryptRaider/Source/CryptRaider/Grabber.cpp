// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return;

	//틱마다 잡는물건의 위치를 갱신해주기 => 잡고 움직이는것처럼 구현
	//새위치 = 컴포넌트위치(카메라위치)+약간앞에, 컴포넌트(카메라)회전값
	//(들고있는 물체가 있는경우에만 작동)
	if (PhysicsHandle->GetGrabbedComponent()) {
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

	
}

void UGrabber::Release() {

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return;

	//들고있는 물체가 있는경우
	if (PhysicsHandle->GetGrabbedComponent()) {
		//가고일 놓으면 태그삭제하기
		//(가고일이 액터임)
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");

		PhysicsHandle->ReleaseComponent(); //물체놓기
	}

}

void UGrabber::Grab() {

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return;

	FHitResult HitResult;
	bool HasHit = GetGrabableInReach(HitResult);
	

	if (HasHit) {
		UE_LOG(LogTemp, Display, TEXT("on hit"));

		//문제 : 바닥에 놓인물체는 물체 리지드바디가 꺼짐(성능절약) => 물체 안집힘
		//해결 : 꺠워주기
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		HitComponent->WakeAllRigidBodies();

		//문제 : 황금조각상이랑 받침대랑 붙어잇어서 그랩이안됨
		//해결: 1.잡은애 물리반응살리기
		//2.액터 랑 조각받침대 분리하기
		//3. 그랩중 태그 추가
		//4. 분리하기
		HitComponent->SetSimulatePhysics(true);
		AActor* HitActor=HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//유저가 잡으면 태그추가.
		HitResult.GetActor()->Tags.Add("Grabbed");


		//손에잡힌 물체잡는함수
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation());
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("No hit"));
	}

	
}


UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	//물건잡는 컴포넌트(손) 찾아오기.
	//BP_Player에서 컴포넌트 추가해줘야함.
	UPhysicsHandleComponent* Result=GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr) {
		UE_LOG(LogTemp,Warning,TEXT("피직스핸들컴포넌트가 필요합니다"))
	}
	return Result;
}

/*
* 잡힌물체 리턴
*/
bool UGrabber::GetGrabableInReach(FHitResult& OutHitResult) const
{
	//빨간 디버깅 선 만들기
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape SPhere = FCollisionShape::MakeSphere(GrabRadius);
	//FHitResult HitResult;	//함수안에서 부딪힌 결과물 넣어줌

	//부딪힌 결과를 OutHirResult에 넣어줌
	bool HasHit = GetWorld()->SweepSingleByChannel(
		OutHitResult, Start, End, FQuat::Identity,
		ECC_GameTraceChannel2, SPhere);


	return HasHit;
}
