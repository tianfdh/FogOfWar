BOOL PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	//-----------------------------------游戏数据初始化部分-------------------------
	//打开音乐文件
	mciSendString("open background.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);

	//雪花
	m_snow=new CParticle(200);
	m_snow->Init();

	//场景
	m_scene=new CScene("bg.png");


	return TRUE;
}
void OnPaint() 
{
	static float lastTime=timeGetTime();    
	static float currentTime=timeGetTime();
	//获取窗口DC指针
	CDC *cDC=this->GetDC();
	//获取窗口大小
	GetClientRect(&m_client);
	//创建缓冲DC
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC,m_client.Width(),m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	//————————————————————开始绘制——————————————————————
	//贴背景,现在贴图就是贴在缓冲DC：m_cache中了
	m_scene->DrawBG(m_cacheDC);
	
	//贴雪花
	m_snow->Draw(m_cacheDC);
	//更新雪花
	currentTime=timeGetTime();
	m_snow->Update(currentTime-lastTime);
	lastTime=currentTime;

	//画出战争迷雾
	m_scene->DrawFog(m_cacheDC);

	m_cacheDC.SetTextAlign(GetTextAlign(m_cacheDC) | TA_CENTER | VTA_CENTER);
	SetTextColor(m_cacheDC, RGB(100, 255, 0));
	SetBkMode(m_cacheDC, TRANSPARENT);
	m_cacheDC.TextOutA(WINDOW_WIDTH / 2, 50, "点击鼠标左键消除迷雾");

	//最后将缓冲DC内容输出到窗口DC中
	cDC->BitBlt(0,0,m_client.Width(),m_client.Height(),&m_cacheDC,0,0,SRCCOPY);

	//————————————————————绘制结束—————————————————————
	
	//在绘制完图后,使窗口区有效
	ValidateRect(&m_client);
	//释放缓冲DC
	m_cacheDC.DeleteDC();
	//释放对象
	m_cacheCBitmap.DeleteObject();
	//释放窗口DC
	ReleaseDC(cDC);
}
void OnTimer(UINT_PTR nIDEvent)
{
	OnPaint();
}


int OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//创建一个10毫秒产生一次消息的定时器
	SetTimer(TIMER_PAINT,10,NULL);

	return 0;
}
