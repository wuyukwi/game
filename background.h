#ifndef _BACKGROUND_H_		//このマクロ定義がされてなかったら
#define _BACKGROUND_H_		//２重インクルード防止のマクロ定義
#include "main.h"
#include "input.h"

//プロトタイプ宣言
void InitBackground(void);
void UninitBackground(void);
void UpdateBackground(void);
void DrawBackground(void);

#endif // !_BACKGROUND_H_
