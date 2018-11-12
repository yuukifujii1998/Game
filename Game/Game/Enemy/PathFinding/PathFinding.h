/*
*	経路探索クラス
*/

#pragma once

#include "MapData.h"


#define LINK_NODE_MAX 4		//隣接ノードの最大数
struct SNode {
	std::vector<SNode*>		linkNode;	//隣接ノード。NULLであれば隣接ノードなし。
	SNode*		parentNode;					//親のノード
	CVector3	position;					//ノードの座標
	int			moveCost;					//移動コスト
	bool		isDone;						//調査済みフラグ
};

class CPathFinding {
public:
	CPathFinding();
	~CPathFinding();
	//ノードの構築
	void BuildNodes();
	//ルートの検索
	//root			ルートの座標を格納するリスト
	//startNumber	スタートのポイントの番号		
	//targetNumber	ゴールのポイントの番号
	void FindRoot(std::vector<CVector3>& root, CVector3 stratPos, CVector3 targetPos);

	void Init(CSkinModel* model)
	{
		m_naviMesh.Init(model);
	}
private:
	std::vector<SNode>		m_nodes;	//ノードの配列
	const float m_gridSize = 3.0f;
	CNavigationMesh	m_naviMesh;
};

extern CPathFinding g_pathFinding;