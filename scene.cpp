void TransparentPNG(CImage *png);

CScene::CScene(char *bg)
{
	m_bg.Load(bg);
	m_black.Load("miwu.png");
	TransparentPNG(&m_black);
	//将数组清0,0表示为黑色迷雾状态
	memset(m_fogArray,0,sizeof(m_fogArray));
	memset(m_clickArray, 0, sizeof(m_clickArray));
}

CScene::~CScene()
{
	m_bg.Destroy();
	m_black.Destroy();
}

//绘制背景
void CScene::DrawBG(CDC &cDC)
{
	m_bg.Draw(cDC,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
}

//绘制战争迷雾
void CScene::DrawFog(CDC &cDC)
{
	for (int i = 0; i<xNum; i++)
		for(int j=0;j<yNum;j++)
		{
			//if (m_fogArray[i][j] < 15)
			{
				int a = (m_fogArray[i][j] / 4) * 40;
				int b = (m_fogArray[i][j] % 4) * 40;
				m_black.Draw(cDC, i * 40 - 40, j * 40 - 40, 40, 40, a, b, 40, 40);
				//m_black.Draw(cDC, i * 40 - 40, j * 40 - 40, 1, 40, 0, 0, 1, 40);
				//m_black.Draw(cDC, i * 40 - 40, j * 40 - 40, 40, 1, 0, 0, 40, 1);
			}
		}
}

bool CheckFog(int xBox,int yBox,int xMouse,int yMouse)
{
	//出界了返回false
	if (xBox<0 || xBox >= xNum || yBox<0 || yBox >= yNum)
		return false;
	//未出界，则距离鼠标点击中心小于一定的范围内可见
	if( (xBox-xMouse)*(xBox-xMouse) + (yBox-yMouse)*(yBox-yMouse) <=16)
		return true;
	else return false;
}

void Add(int fogArray[][yNum], int i, int j, int num){
	if (i<0 || i >= xNum || j<0 || j >= yNum)
	{
		return;
	}
	fogArray[i][j] += num;
	if (fogArray[i][j]>15)
		fogArray[i][j] = 15;
}
//更新迷雾区域
void CScene::UpdateFogArea(int x,int y)
{
	//首先计算出鼠标所在的格子
	int xPosBox=x/40 + 1;
	int yPosBox=y/40 + 1;
	//将迷雾区域复原
	//memset(m_fogArray,0,sizeof(m_fogArray));
	//设置可见区域
// 	for(int xBox=xPosBox-8;xBox<xPosBox+8;xBox++)
// 	{
// 		for(int yBox=yPosBox-8;yBox<yPosBox+8;yBox++)
// 		{
// 			if(CheckFog(xBox,yBox,xPosBox,yPosBox))
// 				m_fogArray[xBox][yBox]=1;
// 		}
// 	}
// 	for (int xBox = xPosBox - 2; xBox < xPosBox + 2; xBox++)
// 	{
// 		for (int yBox = yPosBox - 2; yBox < yPosBox + 2; yBox++)
// 		{
// 			if (CheckFog(xBox, yBox, xPosBox, yPosBox))
// 				m_fogArray[xBox][yBox] = 1;
// 		}
// 	}
	if (xPosBox > 0 || yPosBox > 0){
		int xNew = x % 40;
		int yNew = y % 40;
		if (xNew < 20 && yNew < 20){
			xPosBox--;
			yPosBox--;
		}
		else if (xNew < 20 && yNew > 20)
		{
			xPosBox--;
		}
		else if (xNew >= 20 && yNew < 20){
			yPosBox--;
		}
	}
	
	if (xPosBox < 0)
		xPosBox = 0;
	if (yPosBox < 0)
		yPosBox = 0;
	if (m_clickArray[xPosBox][yPosBox] == 0){
		Add(m_fogArray, xPosBox, yPosBox, 4);
		Add(m_fogArray, xPosBox+1, yPosBox, 8);
		Add(m_fogArray, xPosBox, yPosBox+1, 1);
		Add(m_fogArray, xPosBox+1, yPosBox+1, 2);
		m_clickArray[xPosBox][yPosBox] = 1;
	}
}
