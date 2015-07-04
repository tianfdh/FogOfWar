void TransparentPNG(CImage *png)
{
	for(int i = 0; i <png->GetWidth(); i++)
	{
		for(int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(png->GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}
void CParticle::Init()
{
	//加载雪花图像
	char buf[20];
	for(int i=0;i<7;i++) 
	{
		sprintf(buf,"Snow//%d.png",i);
		m_snowMap[i].Load(buf);
		TransparentPNG(&m_snowMap[i]);
	}
	//初始化雪花粒子
	for(int i=0;i<m_number;i++)
	{
		m_pSnow[i].x=rand()% WINDOW_WIDTH;   //最初雪花在水平方向上随机出现
		m_pSnow[i].y=rand()% WINDOW_HEIGHT; //垂直方向上也是随机出现
		m_pSnow[i].number=rand()%7;         //七种雪花中的一种
		m_pSnow[i].speed=(rand()%5+1)/20.0;
	}
}

void CParticle::Draw(CDC &cDC)
{
	//绘制雪花粒子
	for(int i=0;i<m_number;i++)
		m_snowMap[m_pSnow[i].number].Draw(cDC,m_pSnow[i].x,m_pSnow[i].y,32,32);
}

void CParticle::Update(float time)
{
	for(int i=0;i<m_number;i++)
	{
		m_pSnow[i].y+=time*m_pSnow[i].speed;
		if(m_pSnow[i].y>WINDOW_HEIGHT)
			m_pSnow[i].y=-32;  
	}
}
