#pragma once
#pragma once
class CRigidBody;
class CRigidBodyDraw;
class IRigidBodyDraw;
class CCamera;

/*
物理ワールド
*/
class CPhysicsWorld : Uncopyable
{
	std::unique_ptr<btDefaultCollisionConfiguration>		m_pCollisionConfig;
	std::unique_ptr<btCollisionDispatcher>					m_pCollisionDispatcher;		//衝突解決処理
	std::unique_ptr<btBroadphaseInterface>					m_pOverlappingPairCache;		//ブロードフェーズ。
	std::unique_ptr<btSequentialImpulseConstraintSolver>	m_pConstraintSolver;			//コンストレイントソルバー。拘束条件の解決処理
	std::unique_ptr<btDiscreteDynamicsWorld>				m_pDynamicWorld;				//ワールド
	std::unique_ptr<IRigidBodyDraw>							m_pRigidBodyDraw;
	const CCamera*												m_pCamera;
public:
	//コンストラクタ
	CPhysicsWorld();

	//デストラクタ
	~CPhysicsWorld();

	//初期化関数
	void Init();

	//更新関数
	void Update();

	void Draw();

	//ダイナミックワールドを取得。
	const btDiscreteDynamicsWorld* GetDynamicWorld() const
	{
		return m_pDynamicWorld.get();
	}
	//剛体を物理ワールドに追加
	void AddRigidBody(btRigidBody* rb);

	//剛体を物理ワールドから削除
	void RemoveRigidBody(btRigidBody* rb);

	//剛体描画用のカメラを設定
	void SetCamera(const CCamera* camera)
	{
		m_pCamera = camera;
	}

	/*
	物理ワールドでレイを飛ばす
	castShape			剛体
	convexFromWorld		レイの始点
	convexToWorld		レイの終点
	resultCallback		コールバック
	*/
	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f)
	{
		m_pDynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}

	/*
	物理ワールドで剛体が何かに当たってるいかテスト
	collisionObject		剛体
	resultCallback		コールバック
	*/
	void ContactTest(
		const btCollisionObject* collisionObject,
		btCollisionWorld::ContactResultCallback& resultCallback)
	{
		m_pDynamicWorld->contactTest(const_cast<btCollisionObject*>(collisionObject), resultCallback);
	}

	/*
	剛体をライン描画
	worldTrans		ワールド行列
	colShape		剛体
	*/
	void  DebugDraw(
		const btTransform& worldTrans,
		const btCollisionShape* colShape)
	{
		m_pDynamicWorld->debugDrawObject(worldTrans, colShape, { 0.0f, 0.0f, 0.0f });
	}

	IRigidBodyDraw& GetRigidBodyDraw()
	{
		return *m_pRigidBodyDraw;
	}
};