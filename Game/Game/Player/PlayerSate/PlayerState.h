#pragma once


class CPlayerState 
{
public:
	enum EnPlayerState 
	{
		enPlayerStand,		//�ҋ@�A�j���[�V����
		enPlayerWalk,		//���s�A�j���[�V����
		enPlayerRun,		//����A�j���[�V����
		enPlayerJump,		//�W�����v�A�j���[�V����
		enPlayerAttack,		//�U���A�j���[�V����
		enPlayerDamage,		//�_���[�W�A�j���[�V����
		enPlayerAvoidance,	//����A�j���[�V����
		enPlayerDied,		//���S�A�j���[�V����

		enPlayerArroAttack,	//�|�̃A�j���[�V����
		enPlayerLongSwordAttack,//�匕�̍U���A�j���[�V����
		enPlayerTwinSwordAttack,//�񓁗��̍U���A�j���[�V����
		enPlayerNum	,		//�A�j���[�V�����̐�
		enState_Invald,		//�����Ȃ�
	};

	enum EnPlayerWepon
	{
		enSword,			//�Ў茕
		enLongSword,		//���茕
		enArro,				//�|��
		enTwinSword,		//��

	};
};