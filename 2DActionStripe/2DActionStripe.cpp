// 2DActionStripe.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "2DActionStripe.h"
#include "CList.h"

#include "CFrameSkip.h"
#include "CSpriteDib.h"
#include "CScreenDib.h"
#include "CBaseObject.h"
#include "CPlayerObject.h"

#define WM_NETWORK (WM_USER+1)

#define SERVERPORT 5000
#define SERVERIP L"127.0.0.1"

#define MAX_LOADSTRING 100


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
VOID                MainUpdate(void);
BOOL                InitialGame(void);
BOOL UpdateGame(void);
BOOL Render(void);
void Update(void);

void BubbleSort();

void KeyProcess();

// 윈도우 크기 렉트
RECT WindowRect;

// DC
HDC hdc;

// 윈도우 핸들
HWND g_hWnd;

// 윈도우 상태 체크
bool windowActive;

SOCKET clientSock;

// 게임씬 바로 도입
enum e_GameScene GameState = e_GameScene::GAME;

static DWORD timeCur;

CPlayerObject* playerObj = new CPlayerObject;

CList<CBaseObject*> objList;

CFrameSkip frameSkip;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2DACTIONSTRIPE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2DACTIONSTRIPE));

    MSG msg;

    AllocConsole();
    freopen("CONOUT$", "r+t", stdout);

    int retval;

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        wprintf_s(L"wsastart up error : %d\n", WSAGetLastError());
        return -1;
    }


    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock == INVALID_SOCKET)
    {
        wprintf_s(L"socket error : %d\n", WSAGetLastError());
        return -1;
    }

    linger opt;
    opt.l_onoff = 1;
    opt.l_linger = 0;
    retval = setsockopt(clientSock, SOL_SOCKET, SO_LINGER, (char*)&opt, sizeof(opt));
    if (retval == SOCKET_ERROR) 
    {
        wprintf_s(L"setsockopt error : %d\n", WSAGetLastError());
        return -1;
    }

    retval = WSAAsyncSelect(clientSock, g_hWnd, WM_NETWORK, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
    if (retval == SOCKET_ERROR)
    {
        wprintf_s(L"WSAAsync error : %d\n", WSAGetLastError());
        return -1;
    }

    SOCKADDR_IN serverAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVERPORT);
    InetPtonW(AF_INET, SERVERIP, &serverAddr.sin_addr);

    retval = connect(clientSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (retval == SOCKET_ERROR)
    {
        if (retval != WSAEWOULDBLOCK)
        {
            wprintf_s(L"connect error : %d\n", WSAGetLastError());
            return -1;
        }
    }

    InitialGame();

    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
         
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            MainUpdate();
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2DACTIONSTRIPE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

   WindowRect.top = 0;
   WindowRect.left = 0;
   WindowRect.right = 640;
   WindowRect.bottom = 480;

   AdjustWindowRectEx(&WindowRect, GetWindowStyle(g_hWnd), GetMenu(g_hWnd) != NULL, GetWindowExStyle(g_hWnd));

   int iX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (640 / 2);
   int iY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (480 / 2);


   MoveWindow
   (
       g_hWnd,
       iX,
       iY,
       WindowRect.right - WindowRect.left,
       WindowRect.bottom - WindowRect.top,
       true
   );

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

// 이미지 셋팅하기
BOOL InitialGame(void) {

    // 이미지 가져오기
    SpriteDib.LoadDibSprite(e_SPRITE::eMAP, "image/_Map.bmp", 0, 0);

    
    // 이미지 가져오기
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L01,    "image/Move_L_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L02,    "image/Move_L_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L03,    "image/Move_L_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L04,    "image/Move_L_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L05,    "image/Move_L_05.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L06,    "image/Move_L_06.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L07,    "image/Move_L_07.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L08,    "image/Move_L_08.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L09,    "image/Move_L_09.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L10,    "image/Move_L_10.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L11,    "image/Move_L_11.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L12,    "image/Move_L_12.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R01,    "image/Move_R_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R02,    "image/Move_R_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R03,    "image/Move_R_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R04,    "image/Move_R_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R05,    "image/Move_R_05.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R06,    "image/Move_R_06.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R07,    "image/Move_R_07.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R08,    "image/Move_R_08.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R09,    "image/Move_R_09.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R10,    "image/Move_R_10.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R11,    "image/Move_R_11.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R12,    "image/Move_R_12.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L01,   "image/Stand_L_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L02,   "image/Stand_L_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L03,   "image/Stand_L_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L04,   "image/Stand_L_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L05,   "image/Stand_L_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R01,   "image/Stand_R_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R02,   "image/Stand_R_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R03,   "image/Stand_R_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R04,   "image/Stand_R_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R05,   "image/Stand_R_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L01, "image/Attack1_L_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L02, "image/Attack1_L_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L03, "image/Attack1_L_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L04, "image/Attack1_L_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R01, "image/Attack1_R_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R02, "image/Attack1_R_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R03, "image/Attack1_R_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R04, "image/Attack1_R_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L01, "image/Attack2_L_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L02, "image/Attack2_L_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L03, "image/Attack2_L_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L04, "image/Attack2_L_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R01, "image/Attack2_R_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R02, "image/Attack2_R_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R03, "image/Attack2_R_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R04, "image/Attack2_R_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L01, "image/Attack3_L_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L02, "image/Attack3_L_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L03, "image/Attack3_L_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L04, "image/Attack3_L_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L05, "image/Attack3_L_05.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L06, "image/Attack3_L_06.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R01, "image/Attack3_R_01.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R02, "image/Attack3_R_02.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R03, "image/Attack3_R_03.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R04, "image/Attack3_R_04.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R05, "image/Attack3_R_05.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R06, "image/Attack3_R_06.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::eGUAGE_HP,           "image/HPGuage.bmp", 0, 0);
    SpriteDib.LoadDibSprite(e_SPRITE::eSHADOW,             "image/Shadow.bmp", 32, 4);
    
    
    //playerObj->SetHp(70);
    playerObj->SetPosition(320, 240);

    objList.PushBack(playerObj);

    CBaseObject* playerObjEnemy; 
    playerObjEnemy = new CPlayerObject(); 
    playerObjEnemy->SetPosition(150, 150);
    objList.PushBack(playerObjEnemy);

    
 
    playerObjEnemy = new CPlayerObject();
    playerObjEnemy->SetPosition(500, 150);
    objList.PushBack(playerObjEnemy);

    playerObjEnemy = new CPlayerObject();
    playerObjEnemy->SetPosition(400, 300);
    objList.PushBack(playerObjEnemy);

    playerObjEnemy = new CPlayerObject();
    playerObjEnemy->SetPosition(150, 400);
    objList.PushBack(playerObjEnemy);




    return true;
}

BOOL Render(void) 
{
    
    // 백 버퍼를 받는다.
    BYTE* pDestDib = ScreenDib.GetDibBuffer();

    // 백 버퍼의 가로길이
    int DestWidth = ScreenDib.GetWidth();

    // 백 버퍼의 세로길이
    int DestHeight = ScreenDib.GetHeight();

    // 백 버퍼의 피치
    int pitch = ScreenDib.GetPitch();


    // 백버퍼에 그리기
    SpriteDib.DrawImage(0, 0, 0, pDestDib, DestWidth, DestHeight, pitch);

    BubbleSort();
    
    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; iter++)
    {
        iter->Render(); 
    } 

    ScreenDib.Filp(g_hWnd);

    return true;
}

void Update(void) {

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; iter++)
    {
        iter->Update();
    }

    frameSkip.UpdateCheck(g_hWnd);

}

void KeyProcess() {
    
    DWORD dwAction = KeyList::eACTION_STAND;

    if (GetAsyncKeyState(VK_UP))
    {
        dwAction = KeyList::eACTION_MOVE_UU;
    }

    if (GetAsyncKeyState(VK_DOWN))
    {
        dwAction = KeyList::eACTION_MOVE_DD;
    }

    if (GetAsyncKeyState(VK_RIGHT))
    { 
        dwAction = KeyList::eACTION_MOVE_RR;
    }

    if (GetAsyncKeyState(VK_LEFT))
    {    
        dwAction = KeyList::eACTION_MOVE_LL;
    }

    if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))
    {
        dwAction = KeyList::eACTION_MOVE_LU;
    }


    if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
    {        
        dwAction = KeyList::eACTION_MOVE_RU;
    }


    if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT))
    {    
        dwAction = KeyList::eACTION_MOVE_RD;
    }

    if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
    {        
        dwAction = KeyList::eACTION_MOVE_LD;
    }


    if (GetAsyncKeyState(0x5A))
    {
        dwAction = KeyList::eACTION_ATTACK1;
    }

    if (GetAsyncKeyState(0x58))
    {
        dwAction = KeyList::eACTION_ATTACK2;
    }

    if (GetAsyncKeyState(0x43))
    {
        dwAction = KeyList::eACTION_ATTACK3;
    }

    playerObj->ActionInput(dwAction);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_NETWORK:

        if (WSAGETSELECTERROR(lParam))
        {
            closesocket(wParam);
            return -1;
        }

        switch (WSAGETSELECTEVENT(lParam))
        {
        case FD_CONNECT:
            
            break;
        case FD_READ:
            
            break;
        case FD_WRITE:
            
            break;
        case FD_CLOSE:

            break;
        }

        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_ACTIVATEAPP:
        windowActive = (bool)wParam;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

VOID MainUpdate(void)
{ 
    switch (GameState)
    {
    case TITLE:
        break;

    case GAME:
        UpdateGame();
        break;

    default:

        break;
    }

}

BOOL UpdateGame(void)
{

    if (windowActive)
    {
        KeyProcess();
    }

    Update();

    if (frameSkip.FrameSkip()){
    Render();
    } 

    //백버퍼에 그려진 것은 filp한다.
     


     Sleep(0);
     
     return true;
}

void BubbleSort() 
{
    
    CList<CBaseObject*>::Iterator iterE = objList.end().node->prev;
    
    for (int iCnt = 0; iCnt < objList.listLength; iCnt++)
    {
        for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
        {

            CList<CBaseObject*>::Iterator iterN = iter.node->next;

            if (iter->m_iYpos > iterN->m_iYpos)
            {
                objList.DataSwap(iter, iterE);
            }
        }

        --iterE;
    }

}