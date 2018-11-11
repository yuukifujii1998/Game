#include "NavigationMesh.h"
#include "Graphics/SkinModel.h"

void CNavigationMesh::Init(CSkinModel* skinModel)
{
	std::vector<CVector3> vertexBufferVector;
	std::vector<unsigned int> indexBufferVector;
	DirectX::Model* model = skinModel->GetBody();
	for (auto& mesh : model->meshes)
	{
		for (auto& meshPart : mesh->meshParts)
		{
			ID3D11Buffer* vertexBuffer = meshPart->vertexBuffer.Get();
			D3D11_BUFFER_DESC vertexDesc;
			vertexBuffer->GetDesc(&vertexDesc);


			int vertexCount = vertexDesc.ByteWidth / meshPart->vertexStride;


			D3D11_MAPPED_SUBRESOURCE subresource;
			Engine().GetDeviceContext()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			char* pData = (char*)subresource.pData;
			for (int i = 0; i < vertexCount; i++)
			{
				CVector3 vertexPos = *((CVector3*)pData);
				vertexBufferVector.push_back(vertexPos);
				pData += meshPart->vertexStride;
			}
			Engine().GetDeviceContext()->Unmap(vertexBuffer, 0);


			ID3D11Buffer* indexBuffer = meshPart->indexBuffer.Get();
			D3D11_BUFFER_DESC indexDesc;
			indexBuffer->GetDesc(&indexDesc);
			HRESULT hr = Engine().GetDeviceContext()->Map(indexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			pData = (char*)subresource.pData;
			int indexStride = 0;
			switch (meshPart->indexFormat)
			{
			case DXGI_FORMAT_R16_UINT:
				indexStride = 2;
				break;
			case DXGI_FORMAT_R32_UINT:
				indexStride = 4;
				break;
			}
			vertexCount = indexDesc.ByteWidth / indexStride;
			for (int i = 0; i < vertexCount; i++)
			{
				unsigned int value = 0;
				if (indexStride == 2)
				{
					value = (unsigned int)*((unsigned short*)pData);
					indexBufferVector.push_back(value);
				}
				else if (indexStride == 4)
				{
					value = *((unsigned int*)pData);
					indexBufferVector.push_back(value);
				}
				pData += indexStride;
			}
		}
	}

	for (int i = 0; i < indexBufferVector.size(); i += 3)
	{
		CVector3 poligonPos = CVector3::Zero;

		poligonPos += vertexBufferVector[indexBufferVector[i + 0]];
		poligonPos += vertexBufferVector[indexBufferVector[i + 1]]; 
		poligonPos += vertexBufferVector[indexBufferVector[i + 2]];
		poligonPos.Div(3.0f);
		m_poligonPos.push_back({ poligonPos,  vertexBufferVector[indexBufferVector[i + 0]], vertexBufferVector[indexBufferVector[i + 1]], vertexBufferVector[indexBufferVector[i + 2]] });
	}
}