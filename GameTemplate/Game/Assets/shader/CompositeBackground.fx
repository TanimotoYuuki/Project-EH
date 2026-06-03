Texture2D<float4> bgTexture : register(t0);
Texture2D<float> depthTexture : register(t1);
SamplerState Sampler : register(s0);

cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};
struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}
float4 PSMain(PSInput input) : SV_Target
{
    // 深度値を取得
    float depth = depthTexture.Sample(Sampler, input.uv).r;
    
    // 深度が1.0未満 ＝ 手前に3Dモデルが描画されている
    if (depth < 1.0f)
    {
        // 描画を破棄し、m_mainRenderTarget（ポストエフェクト結果）の色を残す
        discard;
    }
    
    float4 bgColor = bgTexture.Sample(Sampler, input.uv);
    
    if(bgColor.a <= 0.0f)
    {
        discard;
    }
    
    bgColor.rgb /= max(bgColor.a, 0.0001f);
    
    // 3Dモデルがない部分だけ、背景UIの色を出力する
    return bgColor;
}