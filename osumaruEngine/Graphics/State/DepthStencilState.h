#pragma once
enum EnDepthStencilState
{
	enDepthStencilState2D,		//2D�p
	enDepthStencilState3D,		//3D�p
	enDepthStencilStateNum,
};

//�[�x�ݒ���Ǘ�����N���X
class CDepthStencilState : Uncopyable
{
public:

	//�f�X�g���N�^
	~CDepthStencilState();

	//������
	void Init(ID3D11Device* device);

	/*
	�[�x�ݒ��ύX
	deviceContext		�O���t�B�b�N�f�o�C�X
	depthStencilState	�[�x�ݒ�
	*/
	void SetDepthStencilState(ID3D11DeviceContext* deviceContext, EnDepthStencilState depthStencilState)
	{
		m_currentState = depthStencilState;
		deviceContext->OMSetDepthStencilState(m_depthStencilState[m_currentState], 0);
	}

	//���݂̐ݒ���擾
	EnDepthStencilState GetCurrentDepthState()
	{
		return m_currentState;
	}

private:
	ID3D11DepthStencilState* m_depthStencilState[enDepthStencilStateNum] = {nullptr};	//�[�x�X�e�[�g
	EnDepthStencilState		m_currentState = enDepthStencilState2D;						//���݂̐ݒ�
};