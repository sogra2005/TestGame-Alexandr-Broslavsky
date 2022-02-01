/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "GlobalVar.h"
#include <cstdlib>
#include <random>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics( );
    scene->getPhysicsWorld( )->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->SetPhysicsWorld( scene->getPhysicsWorld( ));

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    auto restartItem = MenuItemImage::create("Retry Button.png",
                                             "Retry Button Clicked.png",
                                             CC_CALLBACK_1(HelloWorld::Restart_Game, this ));
    restartItem->setPosition(Point(origin.x + visibleSize.width -
                            (visibleSize.width - PIXIL_SIZE_BLOCK * SCALE_BLOCK *
                             (WIDTH + 1)) / 2, origin.y + visibleSize.height - 100));
    restartItem->setScale(0.7);

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }
    
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(restartItem, closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("A. BROSLAVSKY", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height * 2));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
    
    /* Создание спрайтов */
    
    Sprite *myBlocks[WIDTH * HEIGHT];
    
    for (int q = 0; q < 1; q++) {
        int count = 0, teg_init = 1;
        
        for (int j = 1; j < HEIGHT + 1; j++) {
            
            int local_y = origin.y + (visibleSize.height - PIXIL_SIZE_BLOCK * SCALE_BLOCK * (HEIGHT + 1)) / 2  + PIXIL_SIZE_BLOCK * SCALE_BLOCK * j;
            
            for (int i = 1; i < WIDTH + 1; i++) {
                
                int local_x = origin.x + (visibleSize.width - PIXIL_SIZE_BLOCK * SCALE_BLOCK * (WIDTH + 1)) / 2 + PIXIL_SIZE_BLOCK * SCALE_BLOCK * i;
                
                myBlocks[count] = Sprite::create("pixil.png");
                myBlocks[count]->setPosition(Vec2(local_x, local_y));
                
                int colorVar = Get_Random_Number(1, 3);
                
                if (colorVar == 1) {
                    myBlocks[count]->setColor(Color3B(255, 0, 0)); // r
                } else if (colorVar == 2) {
                    myBlocks[count]->setColor(Color3B(0, 255, 0)); // g
                } else if (colorVar == 3) {
                    myBlocks[count]->setColor(Color3B(0, 0, 255)); // b
                }
                
                myBlocks[count]->setScale(SCALE_BLOCK);
                myBlocks[count]->setTag(teg_init);
                this->addChild(myBlocks[count]);
                count++;
                teg_init++;
            }
        }
    }
     
    
    /* Обработка нажатий */
    /* Создаем прослушиватель событий OneByOne для однократного касания */
    auto myListener = EventListenerTouchOneByOne::create();
    /* Устанавливаем, следует ли проглотить событие, проглотить его, когда метод onTouchBegan возвращает true */
    myListener->setSwallowTouches(true);
    
    /* функция обратного вызова события onTouchBegan */
    myListener->onTouchBegan = [=](Touch* touch, Event* event) {
        /* Получаем цель, привязанную к событию */
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
       
        /* Получаем координаты положения кнопки относительно текущей точки щелчка */
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        Color3B color = target->getColor();
        
        /* Массив для записи спрайтов на "удаление" и переменная для количества записаных элементов в массив */
        int list_blocks[HEIGHT * WIDTH], sum_sprites = 0;
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            list_blocks[i] = 0;
        }
        
        int *pSum_sprites = &sum_sprites;
        
        
        /* Определение диапазона щелчков, определение того, содержит ли переданная координата объект, по которому щелкнули */
        if (rect.containsPoint(locationInNode)) {
            list_blocks[0] = target->getTag();
            sum_sprites += 1;
            Fiel_Analysis(this, target->getTag(), list_blocks, pSum_sprites, color);
            Change_color(this, list_blocks, pSum_sprites);
            
            return true;
        }
        
        return false;
    };
  
    /* Связываем событие для прослушивателя событий, чтобы определить, какая карта реагирует на событие */
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, myBlocks[0]);
    for (int block = 1; block < HEIGHT * WIDTH; block++) {
        _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener->clone(), myBlocks[block]);
    }
    
    
    return true;
}


void HelloWorld::Restart_Game(Ref* pSender) {
    auto scene = HelloWorld::createScene( );
    Director::getInstance( )->replaceScene( TransitionFade::create(0.5, scene ) );
}


void HelloWorld::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();
}

/* Функция для определения возможности анализа соседних спрайтов */
void HelloWorld::Fiel_Analysis(Ref* pSender, int TagSprite,
                               int list_blocks[], int *pSum_sprites, Color3B color_1) {

    /* Определение возможности анализа спрайтов справа и сверху */
    if (TagSprite < HEIGHT * WIDTH - 2) {

        /* Определение возможности анализа спрайта справа */
        if (TagSprite % WIDTH != 0) {
            Color3B color_r = getChildByTag(TagSprite + 1)->getColor();
            int tag_s = TagSprite + 1;

            Sprite_Comparison(this, color_1, color_r, list_blocks, pSum_sprites, tag_s);

        }

        /* Определение возможности анализа спрайта сверху */
        if (WIDTH * HEIGHT - TagSprite >= WIDTH) {
            Color3B color_up = getChildByTag(TagSprite + WIDTH)->getColor();
            int tag_s = TagSprite + WIDTH;

            Sprite_Comparison(this, color_1, color_up, list_blocks, pSum_sprites, tag_s);
        }
        
    }

    /* Определение возможности анализа спрайтов слева и снизу */
    if (TagSprite > 0) {

        /* Определение возможности анализа спрайта слева */
        if (TagSprite % WIDTH != 1) {
            Color3B color_l = getChildByTag(TagSprite - 1)->getColor();
            int tag_s = TagSprite - 1;

            Sprite_Comparison(this, color_1, color_l, list_blocks, pSum_sprites, tag_s);
        }
        /* Определение возможности анализа спрайта снизу */
        if (TagSprite > WIDTH) {
            Color3B color_b = getChildByTag(TagSprite - WIDTH)->getColor();
            int tag_s = TagSprite - WIDTH;

            Sprite_Comparison(this, color_1, color_b, list_blocks, pSum_sprites, tag_s);
        }
        
    }
}



/* Функция для сравнения цвета спрайтов */
void HelloWorld::Sprite_Comparison(Ref* pSender, Color3B color_1, Color3B color_2,
                                   int *list_blocks, int *pSum_sprites, int tag_sprite) {

    if (color_1 == color_2 && Existence(list_blocks, pSum_sprites, tag_sprite) == false) {

        /* Добавление тега спрайта в список на "удаление" */
        list_blocks[*pSum_sprites] = tag_sprite;
        *pSum_sprites += 1;

        Fiel_Analysis(this, tag_sprite, list_blocks, pSum_sprites, color_2);
    }
}



/* Функция для проверки спрайта на предмет того был ли он найден раньше */
bool HelloWorld::Existence(int listBlocks[], int *pSum_sprites, int Search) {
    for( int i = 0; i < *pSum_sprites; i++) {
            if(listBlocks[i] == Search) {
                return true;
            }
    }
    return false;
}



/* Функция для изменения цвета у спрайтов */
void HelloWorld::Change_color(Ref* pSender, int listBlocks[], int *pSum_sprites) {

    /* Изменение на черный цвет окрас найденой последовательности из спрайтов */
    if (*pSum_sprites > 2) {
        for (int i = 0; i < *pSum_sprites; i++) {
            getChildByTag(listBlocks[i])->setColor(Color3B(0, 0, 0));
        }
    }

    /* Создание иллюзии падающих спрайтов путем изменения их цвета */
    for (int i = 0; i < WIDTH / 2; i++) {
        for (int j = 1; j <= WIDTH * HEIGHT - WIDTH; j++) {
            Color3B color_1 = getChildByTag(j)->getColor();
            if (color_1 == Color3B(0, 0, 0)) {
                Color3B color_2 = getChildByTag(j + WIDTH)->getColor();
                getChildByTag(j)->setColor(Color3B(color_2));
                getChildByTag(j + WIDTH)->setColor(Color3B(color_1));
            }
        }
    }
}

/* Функция генератора случайных чисел */
int HelloWorld::Get_Random_Number(int min, int max) {
    std::random_device random_device; // Источник энтропии.
    std::mt19937 generator(random_device()); // Генератор случайных чисел.
    // (Здесь берется одно инициализирующее значение, можно брать больше)
    std::uniform_int_distribution<> distribution(min, max); // Равномерное распределение
    int x = distribution(generator);
    return x;
}

