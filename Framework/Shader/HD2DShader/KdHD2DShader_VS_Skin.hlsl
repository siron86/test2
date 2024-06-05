#include "inc_KdHD2DShader.hlsli"
#include "../inc_KdCommon.hlsli"

//================================
// ���_�V�F�[�_
//================================
VSOutput main(
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

	VSOutput Out;

	// ���W�ϊ�
	Out.Pos = mul(pos, g_mWorld);	 // ���[�J�����W�n	-> ���[���h���W�n�֕ϊ�
	Out.wPos = Out.Pos.xyz;			 // ���[���h���W��ʓr�ۑ�
	Out.Pos = mul(Out.Pos, g_mView); // ���[���h���W�n	-> �r���[���W�n�֕ϊ�
	Out.Pos = mul(Out.Pos, g_mProj); // �r���[���W�n	-> �ˉe���W�n�֕ϊ�

	// ���_�F
	Out.Color = color;

	// �@��
	Out.wN = normalize(mul(normal, (float3x3) g_mWorld));
	// �ڐ�
	Out.wT = normalize(mul(tangent, (float3x3) g_mWorld));
	// �]�ڐ�
	float3 binormal = cross(normal, tangent);
	Out.wB = normalize(mul(binormal, (float3x3) g_mWorld));

	// UV���W
	Out.UV = uv * g_UVTiling + g_UVOffset;

	// �o��
	return Out;
}