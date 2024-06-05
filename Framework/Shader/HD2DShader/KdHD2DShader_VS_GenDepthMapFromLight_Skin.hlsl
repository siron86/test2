#include "inc_KdHD2DShader.hlsli"
#include "../inc_KdCommon.hlsli"

//================================
// ���_�V�F�[�_�F�e�[�x
//================================
VSOutputGenShadow main(
	float4 pos : POSITION,			// ���_���W
	float2 uv : TEXCOORD0,			// �e�N�X�`��UV���W
	float4 color : COLOR,			// ���_�J���[
	float3 normal : NORMAL,			// �@��
	float3 tangent : TANGENT,		// �ڐ�
	uint4 skinIndex : SKININDEX,	// �X�L�����b�V���̃{�[���C���f�b�N�X(���Ԗڂ̃{�[���ɉe�����Ă��邩?�̃f�[�^(�ő�4�Ԃ�))
	float4 skinWeight : SKINWEIGHT	// �{�[���̉e���x(���̃{�[���ɁA�ǂꂾ���e�����Ă��邩?�̃f�[�^(�ő�4�Ԃ�))
)
{
	// �X�L�j���O---------------->
	// �s�������
	row_major float4x4 mBones = 0;	// �s���0����
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		mBones += g_mBones[skinIndex[i]] * skinWeight[i];
	}

	// ���W�Ɩ@���ɓK�p
	pos = mul(pos, mBones);
	normal = mul(normal, (float3x3)mBones);
	// <----------------�X�L�j���O

	VSOutputGenShadow Out;

	// �L�����N�^�[�̍��W�ϊ� : ���[�J�����W�n -> ���[���h���W�n�֕ϊ�
	Out.Pos = mul(pos, g_mWorld);

	// �J�����̋t�����ɕϊ� : ���[���h���W�n -> �r���[���W�n -> �ˉe���W�n�֕ϊ�
	Out.Pos = mul(Out.Pos, g_DL_mLightVP);

	// �ˉe�s���ϊ�����Ȃ��悤�ɕۑ�
	Out.pPos = Out.Pos;

	// UV���W
	Out.UV = uv;

	Out.Color = color;

	// �o��
	return Out;
}