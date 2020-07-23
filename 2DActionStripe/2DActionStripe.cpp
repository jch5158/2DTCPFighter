// 2DActionStripe.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "protocol.h"
#include "2DActionStripe.h"
#include "CRingBuffer.h"
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

struct Session
{
    SOCKET g_Socket;
    CRingBuffer g_RecvQ;
    CRingBuffer g_SendQ;
    bool g_ConnectCheck;
};


Session session;


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


// 네트워크 관련 함수입니다.
//==================================================================

// client socket 분기문입니다.
BOOL NetworkProc(WPARAM wParam, LPARAM lParam);

// read event 를 발생시키는 함수입니다.
BOOL ReadEvent();

// send를 처리하는 함수입니다. 
BOOL SendEvent();

BOOL SendPacket(stHeader* pHeader, char* pPacket);

void SendProc();

void PackingMoveStart(stHeader* pHeader, stPacketCsMoveStart *packetCsMoveStart,BYTE dirCur, unsigned short usX, unsigned short usY);

void PackingMoveStop(stHeader* pHeader, stPacketCsMoveStop* packetCsMoveStop, BYTE dirCur, unsigned short usX, unsigned short usY);

void PackingAttack1(stHeader* pHeader, stPacketCsAttack1* packetCsAttack1, BYTE dirCur, unsigned short usX, unsigned short usY);

void PackingAttack2(stHeader* pHeader, stPacketCsAttack2* packetCsAttack2, BYTE dirCur, unsigned short usX, unsigned short usY);

void PackingAttack3(stHeader* pHeader, stPacketCsAttack3* packetCsAttack3, BYTE dirCur, unsigned short usX, unsigned short usY);


// recv 데이터를 분기하여 처리하는 함수입니다. 
void PacketProc(BYTE byPacketType, char* Packet);

bool PacketProcCreateCharacter(char* Packet);

bool PacketProcCreateOtherCharacter(char* Packet);

bool PacketProcDeleteCharacter(char* Packet);

bool PacketProcOtherCharacterMoveStart(char* Packet);

bool PacketProcOtherCharacterMoveStop(char* Packet);

bool PacketProcScAttack1(char* Packet);

bool PacketProcScAttack2(char* Packet);

bool PacketProcScAttack3(char* Packet);

bool PacketProcDamage(char* Packet);

bool PacketProcScSync(char* Packet);

// ================================================================


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


// 게임씬 바로 도입
enum e_GameScene GameState = e_GameScene::GAME;


CPlayerObject* playerObj = new CPlayerObject;

CList<CBaseObject*> objList;

// 프레임 스킵 함수를 호출하는 객체입니다.
CFrameSkip frameSkip;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    timeBeginPeriod(1);
    
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


    session.g_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (session.g_Socket == INVALID_SOCKET)
    {
        wprintf_s(L"socket error : %d\n", WSAGetLastError());
        return -1;
    }

    linger opt;
    opt.l_onoff = 1;
    opt.l_linger = 0;
    retval = setsockopt(session.g_Socket, SOL_SOCKET, SO_LINGER, (char*)&opt, sizeof(opt));
    if (retval == SOCKET_ERROR) 
    {
        wprintf_s(L"setsockopt error : %d\n", WSAGetLastError());
        return -1;
    }

    retval = WSAAsyncSelect(session.g_Socket, g_hWnd, WM_NETWORK, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
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

    retval = connect(session.g_Socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (retval == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
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
            if (session.g_ConnectCheck)
            {
               MainUpdate();
            }
        }
    }

    timeEndPeriod(1);
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

    SpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_01, "image/xSpark1.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_02, "image/xSpark2.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_03, "image/xSpark3.bmp", 71, 90);
    SpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_04, "image/xSpark4.bmp", 71, 90);

    SpriteDib.LoadDibSprite(e_SPRITE::eGUAGE_HP,           "image/HPGuage.bmp", 0, 0);
    SpriteDib.LoadDibSprite(e_SPRITE::eSHADOW,             "image/Shadow.bmp", 32, 4);
    
    
    /*playerObj->SetPosition(320, 240);

    objList.PushBack(playerObj);
*/

//    CBaseObject* playerObjEnemy; 

    //// 캐릭터 오브젝트를 생성한다.
    //playerObjEnemy = new CPlayerObject(); 
   
    //// 생성한 캐릭터의 좌표를 설정한다.
    //playerObjEnemy->SetPosition(150, 150);
   
    //// 이터레이터에 생성한 캐릭터 오브젝트를 푸쉬한다.
    //objList.PushBack(playerObjEnemy); 

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

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        iter->Render(); 
    } 

    ScreenDib.Filp(g_hWnd);

    return true;
}

void Update(void) {

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        iter->Update();
    }

    if (playerObj->actionCheck)
    {
        SendProc();
    }

    // 로직 프레임 텍스트 출력
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

    // KeyProcess() 에서 입력값을 받습니다.
    // 어떤 액션을 할지 캐릭터 객체 멤버변수에 셋팅해준다. 
    playerObj->ActionInput(dwAction);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_NETWORK:

        if (!NetworkProc(wParam, lParam))
        {
            MessageBox(hWnd, L"접속이 종료되었습니다.", L"끊겼지롱", MB_OK); 
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

    // 키 입력
    if (windowActive)
    {
        KeyProcess();
    }

    // 업데이트
    Update();


    // 렌더
    if (frameSkip.FrameSkip())
    {
        Render();
    } 

     Sleep(0);
     
     return true;
}


// send 파트 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


void SendProc()
{
    stHeader header;

    stPacketCsMoveStart moveStart;
    stPacketCsMoveStop moveStop;

    stPacketCsAttack1 attack1;
    stPacketCsAttack2 attack2;
    stPacketCsAttack3 attack3;

    switch (playerObj->m_dwActionCur)
    {
    case eACTION_MOVE_LL:

        PackingMoveStart(&header, &moveStart,playerObj->m_dwDirCur,playerObj->m_iXpos,playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_MOVE_LU:

        PackingMoveStart(&header, &moveStart, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_MOVE_UU:
        PackingMoveStart(&header, &moveStart, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_MOVE_RU:

        PackingMoveStart(&header, &moveStart, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_MOVE_RR:

        PackingMoveStart(&header, &moveStart, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_MOVE_RD:

        PackingMoveStart(&header, &moveStart, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_MOVE_DD:

        PackingMoveStart(&header, &moveStart, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_MOVE_LD:

        PackingMoveStart(&header, &moveStart, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStart);

        break;
    case eACTION_ATTACK1:

        PackingAttack1(&header, &attack1, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&attack1);

        break;
    case eACTION_ATTACK2:

        PackingAttack2(&header, &attack2, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&attack2);

        break;
    case eACTION_ATTACK3:

        PackingAttack3(&header, &attack3, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&attack3);

        break;

    case eACTION_STAND:

        PackingMoveStop(&header, &moveStop, playerObj->m_dwDirCur, playerObj->m_iXpos, playerObj->m_iYpos);
        SendPacket(&header, (char*)&moveStop);

        break;
    default:
        break;
    }

    return;
}

BOOL SendPacket(stHeader* pHeader, char* pPacket)
{
    int retval;

    retval = session.g_SendQ.Enqueue((char*)pHeader, sizeof(stHeader));
    if (retval != sizeof(stHeader))
    {
        printf_s("send enqueue error");
        closesocket(session.g_Socket);
        return false;
    }

    retval = session.g_SendQ.Enqueue((char*)pPacket, pHeader->bySize);
    if (retval != pHeader->bySize)
    {
        printf_s("send enqueue error");
        closesocket(session.g_Socket);
        return false;
    }

    return SendEvent();
}

BOOL SendEvent()
{
    int retval;

    char buffer[9900];

    while (1)
    {
        retval = session.g_SendQ.GetUseSize();
        if (retval == 0)
        {
            return true;
        }
        
        if (session.g_SendQ.Peek(buffer, sizeof(stHeader)) < 3)
        {
            printf_s("peek error\n");
            closesocket(session.g_Socket);
            return false;
        }

        session.g_SendQ.MoveFront(sizeof(stHeader));
        
        retval = session.g_SendQ.Dequeue(&buffer[3], buffer[1]);
        if (retval != buffer[1])
        {
            printf_s("send Dequeue error\n");
            closesocket(session.g_Socket);
            return false;
        }

        retval = send(session.g_Socket, buffer, buffer[1] + sizeof(stHeader), 0);
        if (retval == SOCKET_ERROR)
        {
            printf_s("send socket error\n");
            closesocket(session.g_Socket);
            return false;
        }

    }

    return true;
}


void PackingMoveStart(stHeader* pHeader, stPacketCsMoveStart* packetCsMoveStart, BYTE dirCur, unsigned short usX, unsigned short usY)
{
    pHeader->byCode = dfNETWORK_PACKET_CODE;
    pHeader->bySize = sizeof(stPacketCsMoveStart);
    pHeader->byType = dfPACKET_CS_MOVE_START;

    packetCsMoveStart->byDirection = playerObj->m_dwActionCur;
    packetCsMoveStart->usX = usX;
    packetCsMoveStart->usY = usY;

    return;
}

void PackingMoveStop(stHeader* pHeader, stPacketCsMoveStop* packetCsMoveStop, BYTE dirCur, unsigned short usX, unsigned short usY)
{
    pHeader->byCode = dfNETWORK_PACKET_CODE;
    pHeader->bySize = sizeof(stPacketCsMoveStop);
    pHeader->byType = dfPACKET_CS_MOVE_STOP;

    packetCsMoveStop->byDirection = dirCur;
    packetCsMoveStop->usX = usX;
    packetCsMoveStop->usY = usY;

    return;
}

void PackingAttack1(stHeader* pHeader, stPacketCsAttack1* packetCsAttack1, BYTE dirCur, unsigned short usX, unsigned short usY)
{
    pHeader->byCode = dfNETWORK_PACKET_CODE;
    pHeader->bySize = sizeof(packetCsAttack1);
    pHeader->byType = dfPACKET_CS_ATTACK1;

    packetCsAttack1->byDirection = playerObj->m_dwDirCur;
    packetCsAttack1->usX = usX;
    packetCsAttack1->usY = usY;

    return;
}

void PackingAttack2(stHeader* pHeader, stPacketCsAttack2* packetCsAttack2, BYTE dirCur, unsigned short usX, unsigned short usY)
{
    pHeader->byCode = dfNETWORK_PACKET_CODE;
    pHeader->bySize = sizeof(packetCsAttack2);
    pHeader->byType = dfPACKET_CS_ATTACK2;


    packetCsAttack2->byDirection = playerObj->m_dwDirCur;
    packetCsAttack2->usX = usX;
    packetCsAttack2->usY = usY;

    return;
}

void PackingAttack3(stHeader* pHeader, stPacketCsAttack3* packetCsAttack3, BYTE dirCur, unsigned short usX, unsigned short usY)
{
    pHeader->byCode = dfNETWORK_PACKET_CODE;
    pHeader->bySize = sizeof(packetCsAttack3);
    pHeader->byType = dfPACKET_CS_ATTACK3;


    packetCsAttack3->byDirection = playerObj->m_dwDirCur;
    packetCsAttack3->usX = usX;
    packetCsAttack3->usY = usY;
    
    return;
}



// send 파트 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



// 네트워크 파트 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

BOOL NetworkProc(WPARAM wParam, LPARAM lParam)
{
    if (WSAGETSELECTERROR(lParam))
    {
        closesocket(wParam);
        session.g_ConnectCheck = false;
        return false;
    }

    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_CONNECT: 
        session.g_ConnectCheck = true;

        return true;
    case FD_CLOSE:
        
        closesocket(wParam);
        session.g_ConnectCheck = false;

        return true;
    case FD_READ:
        
        ReadEvent();

        return true;
    case FD_WRITE:

        SendEvent();
    
        return true;
    }

    return true;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>




// recv 파트 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
BOOL ReadEvent()
{
    int retval;

    int bufferRetval;

    char buffer[9900];

    retval = recv(session.g_Socket, buffer, sizeof(buffer), 0);
    if (retval == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf_s("recv error : %d\n", WSAGetLastError());
            session.g_ConnectCheck = false;
            closesocket(session.g_Socket);
            return false;
        }
        return true;
    }


    bufferRetval = session.g_RecvQ.Enqueue(buffer, retval);
    if (bufferRetval != retval)
    {
        printf_s("recv enqueue size error \n");
        session.g_ConnectCheck = false;
        closesocket(session.g_Socket);
        return false;
    }

    char msgBuffer[100];

    while (1)
    {

        if (3 > session.g_RecvQ.GetUseSize())
        {
            return true;
        }

        bufferRetval = session.g_RecvQ.Peek(msgBuffer, 3);
        if (bufferRetval != 3)
        {
            printf_s("peek error\n");
            session.g_ConnectCheck = false;
            closesocket(session.g_Socket);
            return false;
        }

        if ((BYTE)msgBuffer[0] != 0x89)
        {
            printf_s("header code error : %d\n", msgBuffer[0]);
            session.g_ConnectCheck = false;
            closesocket(session.g_Socket);
            return false;
        }

        if (msgBuffer[1] > session.g_RecvQ.GetUseSize())
        {
            return true;
        }

        session.g_RecvQ.MoveFront(3);

        bufferRetval = session.g_RecvQ.Dequeue(&msgBuffer[3], msgBuffer[1]);
        if (bufferRetval != msgBuffer[1])
        {
            printf_s("header code error\n");
            session.g_ConnectCheck = false;
            closesocket(session.g_Socket);
            return false;
        }

        PacketProc(msgBuffer[2], &msgBuffer[3]);
    }
    return true;
}

void PacketProc(BYTE byPacketType, char* Packet)
{
    switch (byPacketType)
    {
    case dfPACKET_SC_CREATE_MY_CHARACTER:

        PacketProcCreateCharacter(Packet);

        break;
    case dfPACKET_SC_CREATE_OTHER_CHARACTER:

        PacketProcCreateOtherCharacter(Packet);

        break;
    case dfPACKET_SC_DELETE_CHARACTER:

        PacketProcDeleteCharacter(Packet);

        break;
    case dfPACKET_SC_MOVE_START:

        PacketProcOtherCharacterMoveStart(Packet);

        break;
    case dfPACKET_SC_MOVE_STOP:

        PacketProcOtherCharacterMoveStop(Packet);

        break;
    case dfPACKET_SC_ATTACK1:

        PacketProcScAttack1(Packet);

        break;
    case dfPACKET_SC_ATTACK2:

        PacketProcScAttack2(Packet);

        break;
    case dfPACKET_SC_ATTACK3:

        PacketProcScAttack3(Packet);

        break;
    case dfPACKET_SC_DAMAGE:

        PacketProcDamage(Packet);

        break;
    case dfPACKET_SC_SYNC:

        PacketProcScSync(Packet);

        break;
    default:
        break;
    }

}

bool PacketProcCreateCharacter(char* Packet)
{
    stPacketCreateMyCharacter *CreateMyCharacter = (stPacketCreateMyCharacter*)Packet;

    // ID 셋팅
    playerObj->m_dwObjectID = CreateMyCharacter->dwID;
    
    // 체력 셋팅
    playerObj->m_chHP = CreateMyCharacter->byHP;

    // 방향 셋팅
    playerObj->m_dwDirOld = CreateMyCharacter->byDirection;
    playerObj->m_dwDirCur = CreateMyCharacter->byDirection;

    // 좌표 셋팅
    playerObj->m_iXpos = CreateMyCharacter->usX;
    playerObj->m_iYpos = CreateMyCharacter->usY;
    
    // 이터레이터 푸쉬
    objList.PushBack(playerObj);

    return true;
}

bool PacketProcCreateOtherCharacter(char* Packet)
{
    CPlayerObject *enemyPlayer = new CPlayerObject;

    stPacketCreateOtherCharacter *CreateMyCharacter = (stPacketCreateOtherCharacter*)Packet;

    // 아이디 셋팅
    enemyPlayer->m_dwObjectID = CreateMyCharacter->dwID;
    
    // 체력 셋팅
    enemyPlayer->m_chHP = CreateMyCharacter->byHP;

    // 방향 셋팅
    enemyPlayer->m_dwDirOld = CreateMyCharacter->byDirection;
    enemyPlayer->m_dwDirCur = CreateMyCharacter->byDirection;

    // 좌표 셋팅
    enemyPlayer->m_iXpos = CreateMyCharacter->usX;
    enemyPlayer->m_iYpos = CreateMyCharacter->usY;

    // 이터레이터 푸쉬
    objList.PushBack(enemyPlayer);

    return true;
}

bool PacketProcDeleteCharacter(char* Packet)
{
    stPacketDeleteCharacter *deleteCharacter = (stPacketDeleteCharacter*)Packet;

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == deleteCharacter->dwID)
        {
            delete (*iter)->data;
            objList.erase(iter);
            return true;
        }
    } 
}

bool PacketProcOtherCharacterMoveStart(char* Packet)
{
    stPacketScMoveStart *PacketScMoveStart = (stPacketScMoveStart*)Packet;

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == PacketScMoveStart->dwID)
        {
            iter->m_ActionInput = PacketScMoveStart->byDirection;
            
            iter->m_iXpos = PacketScMoveStart->usX;
            iter->m_iYpos = PacketScMoveStart->usY;

            return true;
        }
    }    
}

bool PacketProcOtherCharacterMoveStop(char* Packet)
{

    stPacketScMoveStop *PacketScMoveStop = (stPacketScMoveStop*)Packet;

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == PacketScMoveStop->dwID)
        {
            iter->m_ActionInput = KeyList::eACTION_STAND;        
            iter->m_iXpos = PacketScMoveStop->usX;
            iter->m_iYpos = PacketScMoveStop->usY;

            return true;
        }
    }
}

bool PacketProcScAttack1(char* Packet)
{
    stPacketScAttack1 *PacketAttack1 = (stPacketScAttack1*)Packet;

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == PacketAttack1->dwID)
        {
            iter->m_ActionInput = KeyList::eACTION_ATTACK1;

            iter->m_iXpos = PacketAttack1->usX;
            iter->m_iYpos = PacketAttack1->usY;
            return true;
        }
    }
}

bool PacketProcScAttack2(char* Packet)
{
    stPacketScAttack2 *PacketAttack2 = (stPacketScAttack2*)Packet;

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == PacketAttack2->dwID)
        {
            iter->m_ActionInput = KeyList::eACTION_ATTACK2;
            iter->m_iXpos = PacketAttack2->usX;
            iter->m_iYpos = PacketAttack2->usY;
            return true;
        }
    }
}

bool PacketProcScAttack3(char* Packet)
{
    stPacketScAttack3 *PacketAttack3 = (stPacketScAttack3*)Packet;

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == PacketAttack3->dwID)
        {
            iter->m_ActionInput = KeyList::eACTION_ATTACK3;
            iter->m_iXpos = PacketAttack3->usX;
            iter->m_iYpos = PacketAttack3->usY;
            return true;
        }
    }
}

bool PacketProcDamage(char* Packet)
{

    stPacketScDamage* PacketScDamage = (stPacketScDamage*)Packet;

    DWORD dwAttackerID;
    DWORD victimID;
    BYTE byDamageHP;


    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == PacketScDamage->victimID)
        {
            iter->m_chHP = PacketScDamage->byDamageHP;
            return true;
        }
    }


    return true;
}

bool PacketProcScSync(char* Packet)
{
    stPacketScSync *PacketSync = (stPacketScSync*)Packet;

    CList<CBaseObject*>::Iterator iterE = objList.end();

    for (CList<CBaseObject*>::Iterator iter = objList.begin(); iter != iterE; ++iter)
    {
        if (iter->m_dwObjectID == PacketSync->dwID)
        {
            iter->m_iXpos = PacketSync->usX;
            iter->m_iYpos = PacketSync->usY;
            return true;
        }
    }
}

// recv 파트 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void BubbleSort()
{

    CList<CBaseObject*>::Iterator iterE = objList.end();

    --iterE;

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