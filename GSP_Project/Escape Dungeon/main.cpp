#include "pch.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "RigidBody.h"
#include "Collider.h"
#include "KeyMng.h"
#include "Ground.h"
#include "BAttack.h"
#include "Coin.h"
#include "Thorn.h"
#include "Portal.h"
#include "Fire.h"
#include "mainfuntion.h"


// 기본적으로 타이머 돌면서 자동으로 되야하는거
// 우리가 건들여서 돌아야되는거
// 이동량이 0이거나 0이하면 논리좌표랑 물리좌표 옮기기

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Escape Dungeon";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
TCHAR str_time[100]{}, str_size[50]{};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;

	WndClass.cbWndExtra = 0;
	WndClass.hInstance = g_hInst;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		0, 0, 1024, 768, NULL, (HMENU)NULL, g_hInst, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}



HBRUSH arrBrushs[(int)BRUSH_TYPE::END]{};
HPEN arrPens[(int)PEN_TYPE::END]{};
void LoadMap_1(vector<Object*>& Grounds, RECT Clientrect);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hDC;
	static HDC mdc{};
	static HBITMAP hBitmap;
	static KeyMng KM{};
	static RECT Clientrect = { 0,0,1024,768 };
	
	static Player Hero{};
	static BAttack Basicattack{};
	
	static vector<Object*> grounds{}, Boss{}, coins{}, thorns{};
	static Portal PT{};
	static vector<Monster*>  monsters{};
	static CImage backimg{}, PRW{}, AttackRimg{}, AttackLimg{}, Coinimg{}, Monimg{}, Thimg{}, Portalimg{}
	, coinnum{}, Hpimg{}, GameOver{}, fireimg{}, iceimg{}, clearimg{}, clearword{};
	static vector<Fire*> fires{};
	static int backW{}, backH{}, fir_timer{ 10 }, sec_timer{ 50 }, thr{ 300 }, monint{};
	static float volum{};
	static short check_portal{0};
	static bool stage{}, blife{}, mon{}, CLEAR{};

	static FMOD::System* ssystem;
	static FMOD::Sound* backsound[4];
	static FMOD::Sound* onesound[(int)SOUNDS::END];
	static FMOD::Channel* walkchannel = 0;
	static FMOD::Channel* backchannel = 0;
	static FMOD_RESULT result;
	static void* extradriverdata = 0;

	switch (uMsg) {

	case WM_CREATE:
	{
		GetClientRect(hWnd, &Clientrect);
		

		hDC = GetDC(hWnd);
		hBitmap = CreateCompatibleBitmap(hDC, Clientrect.right, Clientrect.bottom);
		
		mdc = CreateCompatibleDC(hDC);
		SelectObject(mdc, (HBITMAP)hBitmap);
		CreateBrushPen(arrBrushs, arrPens);
	
		result = FMOD::System_Create(&ssystem); // 사운드 시스템 생성 
		if (result != FMOD_OK)
			exit(0);

		ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata); // 사운드 시스템 초기화 
		ssystem->createSound("sound\\BackSound.mp3", FMOD_LOOP_NORMAL, 0, &backsound[0]);
		ssystem->createSound("sound\\STAGE2BOSS.mp3", FMOD_LOOP_NORMAL, 0, &backsound[1]);
		ssystem->createSound("sound\\gameover.mp3", FMOD_LOOP_NORMAL, 0, &backsound[2]);
		ssystem->createSound("sound\\Clear.wav", FMOD_LOOP_NORMAL, 0, &backsound[3]);


		ssystem->createSound("sound\\hero_walk_stone.wav", FMOD_LOOP_NORMAL, 0, &onesound[(int)SOUNDS::WALK]);
		ssystem->createSound("sound\\jumpsound.mp3", FMOD_DEFAULT, 0, &onesound[(int)SOUNDS::JUMP]);
		ssystem->createSound("sound\\coinsound.wav", FMOD_DEFAULT, 0, &onesound[(int)SOUNDS::COIN]);
		ssystem->createSound("sound\\hero_attack2.wav", FMOD_DEFAULT, 0, &onesound[(int)SOUNDS::ATTACK]);
		ssystem->createSound("sound\\hero_damage.wav", FMOD_DEFAULT, 0, &onesound[(int)SOUNDS::HITTED]);
		


		volum = 0.7;
		backchannel->setVolume(volum);
		ssystem->playSound(backsound[0], 0, false, &backchannel);
		
		walkchannel->setVolume(volum);
		
		
		Hero.SetObject(Vec2(MAIN_POSX, MAIN_POSY)
			, Vec2(IMG_L, IMG_L));
		Hero.SetRECT(Clientrect);
		Hero.SetSystem(ssystem);
		Hero.SetChannel(walkchannel);
		Hero.SetSound(onesound);

		PRW.Load(TEXT("image\\Player.png"));
		Hero.Setimg(PRW);
		
		LoadMap_1(grounds, Clientrect,&iceimg);



		backimg.Load(TEXT("image\\background_1.bmp"));
		backW = backimg.GetWidth();     
		backH = backimg.GetHeight();

		Hero.Setbackwidth(backW);

		AttackRimg.Load(TEXT("image\\skill_right.bmp"));
		Basicattack.SetRimg(AttackRimg);

		AttackLimg.Load(TEXT("image\\skill_left.bmp"));
		Basicattack.SetLimg(AttackLimg);

		Coinimg.Load(TEXT("image\\coins.png"));
		MakeCoin(&Coinimg, coins);
		
		Monimg.Load(TEXT("image\\Monster.png"));
		Makemonsters(&Monimg,monsters);

		Thimg.Load(TEXT("image\\Thorn.bmp"));
		MakeThorn(&Thimg, thorns);

		Portalimg.Load(TEXT("image\\Portal.gif"));
		PT.Setimg(&Portalimg);
		PT.Setting();

		coinnum.Load(TEXT("image\\coinnum.png"));
		Hpimg.Load(TEXT("image\\HPbar.png"));
		GameOver.Load(TEXT("image\\GameOver.png"));
		fireimg.Load(TEXT("image\\fire.png"));
		iceimg.Load(TEXT("image\\iceland.png"));
		clearimg.Load(TEXT("image\\Clear.png"));
		clearword.Load(TEXT("image\\GameClear.png"));

		SetTimer(hWnd, 1, fir_timer, NULL);
		SetTimer(hWnd, 2, sec_timer, NULL);



		ReleaseDC(hWnd, hDC);
		break;
	}






	case WM_KEYUP:
	{
		
		if (VK_LEFT == wParam)
		{

			KM.offleft();
		}
		
		else if (VK_RIGHT == wParam)
		{
			
			KM.offright();
		}
		
		else if (VK_SPACE == wParam)
		{
			
			KM.offspace();
		}
		else if (VK_CONTROL == wParam)
		{
			KM.offcontrol();
		}

		else if (VK_RETURN == wParam)
		{
			KM.offenter();
		}

		InvalidateRect(hWnd, &Clientrect, FALSE);
		break;
	}

	case WM_TIMER:
	{

		switch (wParam)
		{


		case 1:		// 게임 로직 + 키 처리
		{
			// 키 타이머에서 체크
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				PostQuitMessage(0);
				return 0;
			}

			if (false == Hero.GetAlive())
			{

				if (false == blife) {
					backchannel->stop();
					ssystem->playSound(backsound[2], 0, false, &backchannel);
					blife = true;
				}
				if (GetAsyncKeyState('R') & 0x8000 || GetAsyncKeyState('r') & 0x8000)
				{
					blife = false;
					backchannel->stop();
					Hero.SetAlive(true);

					Hero.SetObject(Vec2(MAIN_POSX, MAIN_POSY)
							, Vec2(IMG_L, IMG_L));
					Hero.SetMoved(Vec2(0.f, 0.f));
					Hero.SetHP(5);
					Hero.SetState(PLAYERSTATE::STAND);
					if (false == stage) {
						ssystem->playSound(backsound[0], 0, false, &backchannel);
						

						Init_1(grounds, coins, monsters, thorns, backimg, Clientrect);
						LoadMap_1(grounds, Clientrect, &iceimg);
						MakeCoin(&Coinimg, coins);
						Makemonsters(&Monimg, monsters);
						MakeThorn(&Thimg, thorns);

						PT.SetPos(Vec2(4075.f, 515.f));
					}
					else
					{
						KillTimer(hWnd, 3);
						ssystem->playSound(backsound[1], 0, false, &backchannel);
						Init_2(grounds, fires, monsters);
						LoadMap_2(grounds, Clientrect);
						monint = 0;
						mon = false;
						SetTimer(hWnd, 3, thr, NULL);
					}
				}
					
			}

			
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8001)
			{
				Hero.onjump(KM.checkspace());

				KM.onspace();
			}

			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			{
				if (false == KM.checkenter()) {
					Hero.GetCollider()->Changedraw();
					Hero.GetBodyCol()->Changedraw();
					for (int i{}; i < grounds.size(); ++i)
					{
						grounds.at(i)->GetCollider()->Changedraw();
					}

					for (int i{}; i < monsters.size(); ++i)
					{
						monsters.at(i)->GetCollider()->Changedraw();
					}

					for (int i{}; i < fires.size(); ++i)
					{
						fires.at(i)->GetCollider()->Changedraw();

					}
					for (int i{}; i < thorns.size(); ++i)
					{
						thorns.at(i)->GetCollider()->Changedraw();
					}

					for (int i{}; i < coins.size(); ++i)
					{
						coins.at(i)->GetCollider()->Changedraw();
					}
					KM.onenter();
				}
			}


			if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8001 )
			{
				Hero.Goleft(KM.checkleft());
				KM.onleft();
			}

			if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8001)
			{
				Hero.Goright(backW, KM.checkright());
				KM.onright();
			}

			if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8001)
			{
				if (Hero.GetPortal())
				{
					if (false == stage) { // 1스테이지
						if (4 == Hero.Getcoinnum())
						{
							Hero.SetObject(Vec2(MAIN_POSX, MAIN_POSY)
								, Vec2(IMG_L, IMG_L));
							Hero.SetMoved(Vec2(0.f, 0.f));
							Hero.SetHP(5);
							PT.SetPos(Vec2(5650.f, 600.f));
							NextStage(grounds, coins, monsters, thorns, backimg, Clientrect);

							Hero.Setbackwidth(backimg.GetWidth());
							backH = backimg.GetHeight();
							backW = backimg.GetWidth();
							backchannel->stop();
							ssystem->playSound(backsound[1], 0, false, &backchannel);
							stage = true;
							SetTimer(hWnd, 3, thr, NULL);
						}

						else
						{
							check_portal = 1;

						}
					}
					else // 2스테이지
					{
						CLEAR = true;
						backchannel->stop();
						ssystem->playSound(backsound[3], 0, false, &backchannel);

						KillTimer(hWnd, 1);
						KillTimer(hWnd, 2);
						KillTimer(hWnd, 3);
					}
				}
				
			}


			if (GetAsyncKeyState(VK_CONTROL) & 0x8000 )
			{
				if (false == Basicattack.GetAlive() && false == KM.checkcontrol()) {
					Hero.MakeBAttack(Basicattack);
				}
				KM.oncontrol();
			}

			Hero.update(backW,Basicattack.GetAlive());
			Hero.finalupdate(backW);
			Basicattack.update(Hero);
			Hero.OnCollision_update(grounds, coins, monsters, thorns,PT,fires);

			

			for (int i{}; i < fires.size(); ++i)
			{
				fires.at(i)->update(backW);
				fires.at(i)->Collider_ground(grounds);
				fires.at(i)->updatePhypo(Hero.GetMoved());
			}

			for (int i{}; i < grounds.size(); ++i)
			{
				grounds.at(i)->finalupdate(backW);
				grounds.at(i)->updatePhypo(Hero.GetMoved());
			}
			
			for (int i{}; i < coins.size(); ++i)
			{
				coins.at(i)->update();
				coins.at(i)->updatePhypo(Hero.GetMoved());
			}

			for (int i{}; i < thorns.size(); ++i)
			{
				thorns.at(i)->finalupdate(backW);
				thorns.at(i)->updatePhypo(Hero.GetMoved());
			}

			for (int i{}; i < monsters.size(); ++i)
			{
				monsters.at(i)->update(backW);
				monsters.at(i)->update_state(Hero);
				monsters.at(i)->finalupdate(backW);
				monsters.at(i)->OnCollision_Ground(grounds);
				if (monsters.at(i)->OnCollider_BAttack(Basicattack) && monsters.at(i)->GetAlive())
				{
					ssystem->playSound(onesound[(int)SOUNDS::HITTED], 0, false, &walkchannel);
				}
				monsters.at(i)->updatePhypo(Hero.GetMoved());
			}
			PT.finalupdate(backW);
			PT.updatePhypo(Hero.GetMoved());
			
			
			/*for (int i{}; i < fires.size(); ++i)
			{
				
				if(!fires.at(i)->GetAlive());
				{
					delete fires.at(i);
				}
			}*/

			erase_if(fires, [](Fire* fire) {
				return fire->GetAlive() == false;
				});




			ssystem->update();
			break;
		}

		case 2:		// 애니메이션 프레임 전환
		{
			
			Hero.UpAniidx();
			for (int i{}; i < coins.size(); ++i)
			{
				coins.at(i)->Upidx();
			}
			
			for (int i{}; i < monsters.size(); ++i)
			{
				monsters.at(i)->Upidx();
			}
			for (int i{}; i < fires.size(); ++i)
			{
				fires.at(i)->Upidx();
			}

			if (check_portal > 0)
			{
				++check_portal;
				if (40 == check_portal)
				{
					check_portal = 0;
				}
			}


			break;
		}
		case 3:		// (2스테이지 전용) 불 생성 + 보스 등장
		{
			Fire* pNfire = new Fire;
			pNfire->Setimg(&fireimg);
			fires.push_back(pNfire);
			++monint;
			if (false == mon && monint > 10) {
				mon = true;
				Monster* monster = new Monster{};
				monster->SetPos(Vec2(-100.f, 340.f));
				monster->SetScale(Vec2(500.f, 400.f));
				monster->SetHp(100);
				monster->SetLR(false);
				monster->Setboss(true);
				monster->GetCollider()->SetScale(Vec2(400.f, 200.f));
				monster->GetCollider()->SetOffsetPos(Vec2(0.f, -10.f));
				monster->GetRigidbody()->SetMaxVelocity(Vec2(200.f, 500.f));
				monster->Setting();
				monster->Setimg(&Monimg);
				monsters.push_back(monster);
			}
			break;
		}



	}

		InvalidateRect(hWnd, &Clientrect, FALSE);
		break;
	}



	case WM_PAINT:
	{

		hDC = BeginPaint(hWnd, &ps);


		backimg.BitBlt(mdc, Clientrect.left
			, Clientrect.top
			, Clientrect.right
			, Clientrect.bottom
			, (int)Hero.GetMoved().x
			, backH - Clientrect.bottom
			, SRCCOPY);

		
		
		
		for (int i{}; i < grounds.size(); ++i)
		{
			grounds.at(i)->render(mdc);
		}
		for (int i{}; i < coins.size(); ++i)
		{
			coins.at(i)->render(mdc);
		}
		for (int i{}; i < monsters.size(); ++i)
		{
			monsters.at(i)->render(mdc);
		}
		for (int i{}; i < thorns.size(); ++i)
		{
			thorns.at(i)->render(mdc);
		}

		for (int i{}; i < fires.size(); ++i)
		{
			fires.at(i)->render(mdc);
		}

		PT.render(mdc);
		Hero.render(mdc);
		Basicattack.render(mdc);
	



		Hpimg.Draw(mdc, 0, 0, 300, 150
			, 0
			, 138 * (5 - Hero.GetHp())
			, 300
			, 138);

		if (0 != check_portal)
		{
			coinnum.Draw(mdc, 270, 100, 500, 300
				, 0
				, 0
				, 400
				, 400);
		}
		
		if (false == Hero.GetAlive())
		{
			GameOver.Draw(mdc, 230, 150, 600, 400,
				0,
				0,
				GameOver.GetWidth(),
				GameOver.GetHeight());
		}

		if (CLEAR)
		{
			clearimg.Draw(mdc, 0, 0, Clientrect.right, Clientrect.bottom
				, 0
				, 0
				, clearimg.GetWidth()
				, clearimg.GetHeight());
			
			clearword.Draw(mdc, 0, 100, Clientrect.right, 600,
				0,
				0,
				clearword.GetWidth(),
				clearword.GetHeight());
		}

		BitBlt(hDC, 0, 0, Clientrect.right, Clientrect.bottom, mdc, 0, 0, SRCCOPY);

	
		EndPaint(hWnd, &ps);
		break;

	}

	case WM_DESTROY:

		
		for (int i{}; i < (int)PEN_TYPE::END; ++i)
		{
			DeleteObject(arrPens[i]);
		}

		backimg.Destroy();
		PRW.Destroy();
		AttackRimg.Destroy();
		AttackLimg.Destroy();
		Coinimg.Destroy();
		Monimg.Destroy();

		DeleteDC(mdc);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}




//void UIHP(CImage HP, HDC mdc, int* damage)
//{
//
//	switch (*damage)
//	{
//	case 5:
//		HP.Load(TEXT("C:\\Users\\gwang\\OneDrive\\바탕 화면\\스컬\\이펙트,UI\\ui-300-138.bmp"));
//		HP.TransparentBlt(mdc, 0, 0, HP.GetWidth(), HP.GetHeight(), 0, 0, HP.GetWidth(), HP.GetHeight(), RGB(0, 211, 234));
//
//		break;
//	case 4:
//		HP.Load(TEXT("C:\\Users\\gwang\\OneDrive\\바탕 화면\\스컬\\이펙트,UI\\ui-300-138-4.bmp"));
//		HP.TransparentBlt(mdc, 0, 0, HP.GetWidth(), HP.GetHeight(), 0, 0, HP.GetWidth(), HP.GetHeight(), RGB(0, 211, 234));
//		break;
//	case 3:
//		HP.Load(TEXT("C:\\Users\\gwang\\OneDrive\\바탕 화면\\스컬\\이펙트,UI\\ui-300-138-3.bmp"));
//		HP.TransparentBlt(mdc, 0, 0, HP.GetWidth(), HP.GetHeight(), 0, 0, HP.GetWidth(), HP.GetHeight(), RGB(0, 211, 234));
//		break;
//
//	case 2:
//		HP.Load(TEXT("C:\\Users\\gwang\\OneDrive\\바탕 화면\\스컬\\이펙트,UI\\ui-300-138-2.bmp"));
//		HP.TransparentBlt(mdc, 0, 0, HP.GetWidth(), HP.GetHeight(), 0, 0, HP.GetWidth(), HP.GetHeight(), RGB(0, 211, 234));
//		break;
//
//	case 1:
//		HP.Load(TEXT("C:\\Users\\gwang\\OneDrive\\바탕 화면\\스컬\\이펙트,UI\\ui-300-138-1.bmp"));
//		HP.TransparentBlt(mdc, 0, 0, HP.GetWidth(), HP.GetHeight(), 0, 0, HP.GetWidth(), HP.GetHeight(), RGB(0, 211, 234));
//		break;
//
//	default:
//		HP.Load(TEXT("C:\\Users\\gwang\\OneDrive\\바탕 화면\\스컬\\이펙트,UI\\ui-300-138-0.bmp"));
//		HP.TransparentBlt(mdc, 0, 0, HP.GetWidth(), HP.GetHeight(), 0, 0, HP.GetWidth(), HP.GetHeight(), RGB(0, 211, 234));
//
//		break;
//
//	}
//
//}