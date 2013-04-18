#ifndef __BASIC_H__
#define __BASIC_H__   

#define CCSetString CCUserDefault::sharedUserDefault()->setStringForKey  
  
#define CCSetInt CCUserDefault::sharedUserDefault()->setIntegerForKey  
  
#define CCSetBool CCUserDefault::sharedUserDefault()->setBoolForKey  
  
#define CCGetString CCUserDefault::sharedUserDefault()->getStringForKey  
  
#define CCGetInt CCUserDefault::sharedUserDefault()->getIntegerForKey  
  
#define CCGetBool CCUserDefault::sharedUserDefault()->getBoolForKey  

#define PIC_MAIN_BACKGROUND "main_background.png"
#define PIC_MAIN_TITLE "main_title.png"
#define PIC_CHESSBOARD1 "chessboard1.png"
#define PIC_CHESSMAN1 "chessman1.png"
#define PIC_CHESSMAN2 "chessman2.png"
#define PIC_CHESSMAN1S "chessman1select.png"
#define PIC_CHESSMAN2S "chessman2select.png"
#define PIC_SELECT_CIRCLE1 "selectCircle1.png"

#define PIC_ROUND "round.png"
#define PIC_BUTTON_BACK "backButton.png"
#define PIC_BUTTON_BACK_SELECTED "backButtonSelected.png"
#define PIC_STOPBUTTON "stopButton.png"
#define PIC_STOPBUTTON_SELECTED "stopButtonSelected.png"
#define PIC_MUSIC_ON "buttonMusic1.png"
#define PIC_MUSIC_OFF "buttonMusic2.png"
#define PIC_RESUME "resumeButton.png"
#define PIC_RESUME_SELECTED "resumeButtonSelected.png"
#define PIC_QUIT "quitButton.png"
#define PIC_QUIT_SELECTED "quitButtonSelected.png"

#define PIC_BTN_2PLAYER1 "btn_2player1.png"
#define PIC_BTN_2PLAYER2 "btn_2player2.png"
#define PIC_BTN_COMPUTER1 "btn_vsComputer1.png"
#define PIC_BTN_COMPUTER2 "btn_vsComputer2.png"
#define PIC_BTN_ONLINE1 "btn_online1.png"
#define PIC_BTN_ONLINE2 "btn_online2.png"
#define PIC_BTN_LEADBOARD1 "btn_leadboard1.png"
#define PIC_BTN_LEADBOARD2 "btn_leadboard2.png"

const int s_pics_num = 27;
const std::string s_pics[s_pics_num] = {
	PIC_MAIN_BACKGROUND,
	PIC_MAIN_TITLE,
	PIC_CHESSBOARD1,
	PIC_CHESSMAN1,
	PIC_CHESSMAN2,
	PIC_CHESSMAN1S,
	PIC_CHESSMAN2S,
	PIC_SELECT_CIRCLE1,
	PIC_ROUND,
	PIC_BUTTON_BACK,
	PIC_BUTTON_BACK_SELECTED,
	PIC_STOPBUTTON,
	PIC_STOPBUTTON_SELECTED,
	PIC_MUSIC_ON,
	PIC_MUSIC_OFF,
	PIC_RESUME,
	PIC_RESUME_SELECTED,
	PIC_QUIT,
	PIC_QUIT_SELECTED,
	PIC_BTN_2PLAYER1,
	PIC_BTN_2PLAYER2,
	PIC_BTN_COMPUTER1,
	PIC_BTN_COMPUTER2,
	PIC_BTN_ONLINE1,
	PIC_BTN_ONLINE2,
	PIC_BTN_LEADBOARD1,
	PIC_BTN_LEADBOARD2
};

enum GAME_STATE
{
	GAME_STATE_NORMAL,
	GAME_STATE_WIN,
	GAME_STATE_LOSE,
	GAME_STATE_PROP,
	GAME_STATE_STOP,
	GAME_STATE_FOUND,
	GAME_STATE_FIRSTPLAY,
	GAME_STATE_NEWTHING,
};

struct GameProcess
{
	bool win;
	bool lose;
	GAME_STATE state;
};


#endif