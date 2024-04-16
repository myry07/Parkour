#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 630
#define GRAVITY 8
#define JUMP_FORCE 17

//定义结构体 渲染与界面
typedef struct Game {
    SDL_Renderer *renderer;
    SDL_Window *window;
} Game;
Game game;

//进入界面
typedef struct Interface {
    SDL_Surface *img;
    SDL_Texture *t_img;
} If;
If interface;

//背景
typedef struct Background {
    SDL_Surface *img[2];
    SDL_Texture *t_img[2];
    int img_Index[12];
} Bg;
Bg bg;

//Hero 结构体
typedef struct Hero {
    int x;
    int y;
    int hp;
    int width;
    int height;
    int speed;
    bool jumpState;
    bool isLive;
    SDL_Surface *img[12];
    SDL_Texture *t_img[12];
    int img_Index;
} Hero;
Hero hero = {300, 470, 200, 80, 90, 7, false, false};

//障碍物
typedef enum {
    TURTLE, //乌龟 0
    LION, //狮子 1
    OBSTACLE_TYPE_COUNT //可以查看 有多少种枚举类型
} Obstacle_type;

typedef struct Enemy {
    Obstacle_type type;
    int x, y;
    int width, height;
    int speed;
    int hurt;
    bool isLive;
} Enemy;

//胜利
typedef struct Res {
    int score;
    bool isWin;
    SDL_Surface *img;
    SDL_Texture *t_img;
} Res;
Res win;

TTF_Font *font = NULL;
SDL_Surface *message = NULL;
SDL_Texture *t_message = NULL;

//乌龟
SDL_Surface *turtle[6];
SDL_Texture *t_turtle[6];
Enemy t1 = {TURTLE, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 40 - 70, 50, 40, 10, 10, false};
int turtleIndex = 0;

//乌龟plus
SDL_Surface *turtle_plus;
SDL_Texture *t_turtle_plus;
Enemy t_plus = {TURTLE, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50 - 70, 60, 50, 7, 30, false};

//狮子
SDL_Surface *lion[6];
SDL_Texture *t_lion[6];
Enemy l = {LION, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100 - 70, 100, 100, 14, 40, false};
int lionIndex = 0;


void init() {
    //初始化SDL库
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL failed : %s", SDL_GetError());
        return;
    }

    // 初始化 SDL_image 库
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("SDL_image failed: %s\n", IMG_GetError());
        return;
    }

    //ttf库
    if (TTF_Init() == -1) {
        return;
    }

    // 创建游戏窗口
    game.window = SDL_CreateWindow("Parkour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   0);
    if (game.window == NULL) {
        SDL_Log("createWindow failed : %s", SDL_GetError());
        return;
    }

    // 创建游戏渲染
    game.renderer = SDL_CreateRenderer(game.window, -1, 0);
    if (game.renderer == NULL) {
        SDL_Log("createRenderer failed : %s", SDL_GetError());
        return;
    }

    //设置字体
    font = TTF_OpenFont("font.ttf", 64);
    SDL_Color color = {0, 0, 255};
    message = TTF_RenderText_Solid(font, "put your text here", color);
    t_message = SDL_CreateTextureFromSurface(game.renderer, message);

    // 初始化界面
    interface.img = IMG_Load("/Users/myry/Documents/MyCProject/Parkour/resource/interface.png");
    interface.t_img = SDL_CreateTextureFromSurface(game.renderer, interface.img);

    // 初始化背景
    bg.img[0] = IMG_Load("/Users/myry/Documents/MyCProject/Parkour/resource/back.png");
    bg.img[1] = IMG_Load("/Users/myry/Documents/MyCProject/Parkour/resource/ground.png");

    bg.t_img[0] = SDL_CreateTextureFromSurface(game.renderer, bg.img[0]);
    bg.t_img[1] = SDL_CreateTextureFromSurface(game.renderer, bg.img[1]);

    for (int i = 0; i < 12; i++) {//地面滚动
        bg.img_Index[i] = i * 200;
    }

    //初始化
    char name[64];
    ///Users/myry/Documents/MyCProject/Parkour/resource/win.png
    win.img = IMG_Load("/Users/myry/Documents/MyCProject/Parkour/resource/win.png");
    win.t_img = SDL_CreateTextureFromSurface(game.renderer, win.img);

    for (int i = 0; i < 12; i++) {
        ///Users/myry/Documents/MyCProject/Parkour/resource/boy1.png
        sprintf(name, "/Users/myry/Documents/MyCProject/Parkour/resource/boy%d.png", i + 1);
        hero.img[i] = IMG_Load(name);
        hero.t_img[i] = SDL_CreateTextureFromSurface(game.renderer, hero.img[i]);
    }

    for (int i = 0; i < 6; i++) {
        ///Users/myry/Documents/MyCProject/Parkour/resource/t1.png
        sprintf(name, "/Users/myry/Documents/MyCProject/Parkour/resource/t%d.png", i + 2);
        turtle[i] = IMG_Load(name);
        t_turtle[i] = SDL_CreateTextureFromSurface(game.renderer, turtle[i]);
    }

    turtle_plus = IMG_Load("/Users/myry/Documents/MyCProject/Parkour/resource/t1.png");
    t_turtle_plus = SDL_CreateTextureFromSurface(game.renderer, turtle_plus);

    for (int i = 0; i < 6; i++) {
        ///Users/myry/Documents/MyCProject/Parkour/resource/p1.png
        sprintf(name, "/Users/myry/Documents/MyCProject/Parkour/resource/p%d.png", i + 1);
        lion[i] = IMG_Load(name);
        t_lion[i] = SDL_CreateTextureFromSurface(game.renderer, lion[i]);
    }
}

void deinit() {
    SDL_FreeSurface(message);
    SDL_DestroyTexture(t_message);

    SDL_FreeSurface(interface.img);
    SDL_DestroyTexture(interface.t_img);

    SDL_FreeSurface(win.img);
    SDL_DestroyTexture(win.t_img);

    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(bg.img[i]);
        SDL_DestroyTexture(bg.t_img[i]);
    }

    for (int i = 0; i < 12; i++) {
        SDL_FreeSurface(hero.img[i]);
        SDL_DestroyTexture(hero.t_img[i]);
    }

    for (int i = 0; i < 6; i++) {
        SDL_FreeSurface(turtle[i]);
        SDL_DestroyTexture(t_turtle[i]);
    }

    SDL_FreeSurface(turtle_plus);
    SDL_DestroyTexture(t_turtle_plus);

    for (int i = 0; i < 6; i++) {
        SDL_FreeSurface(lion[i]);
        SDL_DestroyTexture(t_lion[i]);
    }

    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
}

void drawInterface() {
    SDL_Rect r = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(game.renderer, interface.t_img, NULL, &r);
}

void drawBg() {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game.renderer);

    // 渲染背景
    SDL_RenderCopy(game.renderer, bg.t_img[0], NULL, NULL);

    for (int i = 0; i < 12; i++) {
        SDL_Rect r = {bg.img_Index[i], SCREEN_HEIGHT - 70, 200, 70};
        SDL_RenderCopy(game.renderer, bg.t_img[1], NULL, &r);
    }
}

void drawScore() {
    SDL_Rect r0 = {1000, 100, 100, 100};
    SDL_RenderCopy(game.renderer, t_message, NULL, &r0);
}

void drawBoy() {
    SDL_Rect r = {hero.x, hero.y, hero.width, hero.height};
    if (hero.y == SCREEN_HEIGHT - 70 - hero.height) {
        SDL_RenderCopy(game.renderer, hero.t_img[hero.img_Index], NULL, &r);
    } else {
        SDL_RenderCopy(game.renderer, hero.t_img[0], NULL, &r);
    }

    if (hero.img_Index != 11) {
        hero.img_Index++;
        SDL_Delay(10);
    } else {
        hero.img_Index = 0;
    }
}

void drawTurtle(int num) {
    SDL_Rect r = {t1.x, t1.y, t1.width, t1.height};
    if (num == 3) {
        t1.isLive = true;
    }

    if (t1.isLive) {
        SDL_RenderCopy(game.renderer, t_turtle[turtleIndex], NULL, &r);
    }

    if (turtleIndex != 6) {
        turtleIndex++;
        SDL_Delay(5);
    } else {
        turtleIndex = 0;
    }
}

void drawTurtle_Plus(int num) {
    SDL_Rect r1 = {t_plus.x, t_plus.y, t_plus.width, t_plus.height};
    if (num == 7) {
        t_plus.isLive = true;
    }

    if (t_plus.isLive) {
        SDL_RenderCopy(game.renderer, t_turtle_plus, NULL, &r1);
    }
}

void drawLion(int num) {
    SDL_Rect r = {l.x, l.y, l.width, l.height};
    if (num == 12) {
        l.isLive = true;
    }

    if (l.isLive) {
        SDL_RenderCopy(game.renderer, t_lion[lionIndex], NULL, &r);
    }

    if (lionIndex != 5) {
        lionIndex++;
        SDL_Delay(5);
    } else {
        lionIndex = 0;
    }

}

void drawHp() {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_Rect back = {50, 50, 200, 20};
    SDL_RenderDrawRect(game.renderer, &back);
    SDL_SetRenderDrawColor(game.renderer, 192, 192, 192, 255);
    SDL_RenderFillRect(game.renderer, &back);
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    SDL_Rect red = {50, 50, hero.hp, 20};
    SDL_RenderFillRect(game.renderer, &red);

    if (hero.hp <= 0) {
        hero.isLive = false;
    }
}

void drawWin() {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game.renderer);

    SDL_Rect rs = {0, 0, 800, 390};
    SDL_RenderCopy(game.renderer, win.t_img, &rs, NULL);
}

void move() {
    //背景移动
    for (int i = 0; i < 12; i++) {
        bg.img_Index[i] -= hero.speed;
        if (bg.img_Index[i] < -SCREEN_WIDTH) {
            for (int j = 0; j < 12; j++) {//地面滚动
                bg.img_Index[j] = j * 200;
            }
        }
    }

    //乌龟移动
    if (t1.isLive) {
        t1.x -= t1.speed;
        if (t1.x + t1.width < 0) {
            t1.isLive = false;
            t1.x = SCREEN_WIDTH;

            win.score++;
            printf("score: %d\n", win.score);
        }
    }

    //乌龟plus移动
    if (t_plus.isLive) {
        t_plus.x -= t_plus.speed;
        if (t_plus.x + t_plus.width < 0) {
            t_plus.isLive = false;
            t_plus.x = SCREEN_WIDTH;

            win.score += 2;
            printf("score: %d\n", win.score);
        }
    }

    //狮子移动
    if (l.isLive) {
        l.x -= l.speed;
        if (l.x + l.width < 0) {
            l.isLive = false;
            l.x = SCREEN_WIDTH;

            win.score += 3;
            printf("score: %d\n", win.score);
        }
    }

    if (win.score >= 20) {
        win.isWin = true;
        hero.isLive = false;
    }
}

void jump() {
    //增加重力
    int y = SCREEN_HEIGHT - 70 - hero.height;
    hero.y += GRAVITY;
    if (hero.y >= y) {
        hero.y = y;
    }

    if (hero.jumpState) {
        hero.y -= JUMP_FORCE;
        hero.x += 2;
    }

    if (hero.y > 300 && hero.y < y) {
        hero.x += 2;
    }

    if (hero.x != 300) {
        hero.x--;
    }

    if (hero.y < 300) {
        hero.jumpState = false;
    }
}

void hit() {
    //乌龟碰撞
    if (t1.isLive && hero.x + hero.width >= t1.x && hero.x <= t1.x + t1.width && hero.y + 50 >= t1.y && hero.isLive) {
        hero.hp -= t1.hurt;
        t1.isLive = false;
        t1.x = SCREEN_WIDTH;
        printf("hp: %d\n", hero.hp);
    }

    //乌龟plus碰撞
    if (t_plus.isLive && hero.x + hero.width >= t_plus.x && hero.x <= t_plus.x + t_plus.width &&
        hero.y + 50 >= t_plus.y && hero.isLive) {
        hero.hp -= t_plus.hurt;
        t_plus.isLive = false;
        t_plus.x = SCREEN_WIDTH;
        printf("hp: %d\n", hero.hp);
    }

    //狮子碰撞
    if (l.isLive && hero.x + hero.width >= l.x && hero.x <= l.x + l.width && hero.y + 50 >= l.y && hero.isLive) {
        hero.hp -= l.hurt;
        l.isLive = false;
        l.x = SCREEN_WIDTH;
        printf("hp: %d\n", hero.hp);
    }
}

void event_loop() {
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    while (1) {
        Uint32 startTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            // 处理事件
            switch (event.type) {
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_1 && !hero.isLive) {
                        hero.isLive = true;
                    }
                    if (event.key.keysym.sym == SDLK_w && hero.y == SCREEN_HEIGHT - 70 - hero.height) {
                        hero.jumpState = true;
                    }
                    break;
            }
        }

        // 定义范围
        int min = 1;
        int max = 17;

        int num = rand() % (max - min + 1) + min;

        if (!hero.isLive && !win.isWin) {
            drawInterface();
        }

        if (win.isWin) {
            drawWin();
        }

        if (hero.isLive) {
            drawBg();
            drawBoy();
            drawScore();
            drawTurtle(num);
            drawLion(num);
            drawTurtle_Plus(num);
            drawHp();

            move();
            jump();
            hit();
        }

        // 控制帧率
        Uint32 elapsedTime = SDL_GetTicks() - lastTime;
        if (elapsedTime < frameDelay) {
            SDL_Delay(frameDelay - elapsedTime);
        }
        lastTime = SDL_GetTicks();

        SDL_RenderPresent(game.renderer);
    }
}


int main() {
    srand(time(NULL));//设置随机种子
    init();
    event_loop();
    deinit();
    return 0;
}
