#include "GameLayer.h"
#include "HudLayer.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#define random_range(low,high) (rand()%(high-low+1))+low
#define frandom (float)rand()/UINT64_C(0x100000000)
#define frandom_range(low,high) ((high-low)*frandom)+low

GameLayer::GameLayer(void)
{
	this->_hero=NULL;
	this->_robots=NULL;
}


GameLayer::~GameLayer(void)
{
}

bool GameLayer::init()
{
	if(!CCLayer::init()){
		return false;
	}

	//Ԥ�������ֺ���Ч
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/arena.mp3");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/arena.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("pd_hit0.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("pd_hit1.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("pd_herodeath.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("pd_botdeath.wav");


	this->initTiledmap();


	//���ؾ����
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");

	_actors=CCSpriteBatchNode::create("sprites.png");
	_actors->getTexture()->setAliasTexParameters();

	this->addChild(_actors);

	this->initHero();

	
	this->setTouchEnabled(true);

	this->scheduleUpdate();

	this->initRobots();

	return true;
}

void GameLayer::initRobots()
{
	this->_robots=CCArray::create();
	this->_robots->retain();

	for(int i=0;i<50;i++){
		Robot* robot=Robot::create();

		this->_actors->addChild(robot);
		this->_robots->addObject(robot);

		//��ʼ��������λ��
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		int minX=winSize.width+robot->_centerToSides;
		int maxX=this->_tilemp->getMapSize().width*this->_tilemp->getTileSize().width-robot->_centerToSides;
		int minY=robot->_centerToBottom;
		int maxY=3*this->_tilemp->getTileSize().height+robot->_centerToBottom;
		robot->setScaleX(-1);
		robot->setPosition(ccp((rand()%(maxX-minX+1))+minX,(rand()%(maxY-minY+1))+minY));
		robot->_desiredPosition=robot->getPosition();
		robot->idle();
	}

}

void GameLayer::initTiledmap()
{
	_tilemp=CCTMXTiledMap::create("pd_tilemap.tmx");

	CCObject*  obj=NULL;
	CCARRAY_FOREACH(_tilemp->getChildren(),obj){
		CCTMXLayer* _child=(CCTMXLayer*)obj;

		_child->getTexture()->setAliasTexParameters();  //�رտ����
	}

	this->addChild(_tilemp);

}

void GameLayer::initHero()
{
	this->_hero=Hero::create();
	this->_actors->addChild(this->_hero);

	this->_hero->setPosition(ccp(this->_hero->_centerToSides,80));

	this->_hero->_desiredPosition=this->_hero->getPosition();

	this->_hero->idle();
}

void GameLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	this->_hero->attack();

	CCObject* object=NULL;
	CCARRAY_FOREACH(this->_robots,object){
		Robot* robot=(Robot*)object;

		if(robot->_actionState!=kActionDead){
			if(fabsf(_hero->getPositionY()-robot->getPositionY())<10){
				if(_hero->_attackBox.actual.intersectsRect(robot->_hitBox.actual)){
					robot->hurtWithDamage(_hero->_damage);
				}
			}
		}
	}
}
void GameLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);  //ע���㴥��
}

//ִ�� ���߶���
void GameLayer::didChangeDirectorTo(SimpleDPad* simpleDPad,cocos2d::CCPoint direction)
{
	this->_hero->walkWithDirection(direction);
}

void GameLayer::isHoldingDirector(SimpleDPad* simpleDPad,cocos2d::CCPoint direction)
{
	this->_hero->walkWithDirection(direction);
}

//����������������õĺ���
void GameLayer::simpleDPadTouchEnded(SimpleDPad* simpleDpad)
{
	if(this->_hero->_actionState==kActionStateWalk){
		this->_hero->idle();
	}
}

void GameLayer::update(float dt)
{
	this->_hero->updateDesiredPosition(dt);


	//����Ӣ��λ��
	float posx=MIN(this->_tilemp->getMapSize().width*this->_tilemp->getTileSize().width-this->_hero->_centerToSides,MAX(this->_hero->_centerToSides,this->_hero->_desiredPosition.x));
	float posY=MIN(3*this->_tilemp->getTileSize().height+this->_hero->_centerToBottom,MAX(_hero->_centerToBottom,_hero->_desiredPosition.y));

	this->_hero->setPosition(ccp(posx,posY));

	//��ͼ����
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	int x=MAX(_hero->getPositionX(),winSize.width/2);
	int y=MAX(_hero->getPositionY(),winSize.height/2);
	x=MIN(x,(this->_tilemp->getMapSize().width*this->_tilemp->getTileSize().width)-winSize.width/2);
	y=MIN(y,(this->_tilemp->getMapSize().height*this->_tilemp->getTileSize().height)-winSize.height/2);

	CCPoint actualPosition=ccp(x,y);
	CCPoint centerOfView=ccp(winSize.width/2,winSize.height/2);
	CCPoint viewPoint=ccpSub(centerOfView,actualPosition);
	this->setPosition(viewPoint);
	

	//�ı�����˺�Ӣ�۵�z��˳��
	CCObject* object=NULL;
	CCARRAY_FOREACH(this->_actors->getChildren(),object){
		this->_actors->reorderChild((CCNode*)object,1000-((ActionSprite*)object)->getPositionY());
	}

	this->updateRobots(dt);

	//�޶�������ֻ���ڵ�ͼ��Χ���˶�
	CCObject* pObject=NULL;
	CCARRAY_FOREACH(this->_robots,pObject){
		Robot* robot=(Robot*)pObject;

		posx=MIN(this->_tilemp->getMapSize().width*this->_tilemp->getTileSize().width-robot->_centerToSides,MAX(robot->_centerToSides,robot->_desiredPosition.x));
		posY=MIN(3*this->_tilemp->getTileSize().height+robot->_centerToBottom,MAX(robot->_centerToBottom,robot->_desiredPosition.y));

		robot->setPosition(ccp(posx,posY));
	}
}

static float GetCurTime()
{
	timeval time;
	gettimeofday(&time,NULL);
	unsigned long millisecs=(time.tv_sec*1000)+(time.tv_usec)/1000;

	return (float)millisecs;
}

void GameLayer::updateRobots(float dt)
{
	int alive=0; //��ǰ���ŵĻ����˵�����
	int distanceSQ;
	int randomChoice=0;
	
	CCObject* pObject=NULL;
	CCARRAY_FOREACH(this->_robots,pObject){
		Robot* robot=(Robot*)pObject;

		

		if(robot->_actionState != kActionDead){
			alive++;

			if(::GetCurTime()>robot->_nextDecisionTime){
				distanceSQ=ccpDistanceSQ(robot->getPosition(),this->_hero->getPosition());

				if(distanceSQ<=50*50){
					robot->_nextDecisionTime=::GetCurTime()+frandom_range(0.1,0.5)*1000;
					randomChoice=random_range(0,1);

					if(randomChoice==0){   //������ִ�й�������
						if(this->_hero->getPositionX()>robot->getPositionX()){
							robot->setScaleX(1.0f);
						}else{
							robot->setScaleX(-1.0f);
						}

						robot->_nextDecisionTime=robot->_nextDecisionTime+frandom_range(0.1,0.5)*2000;

						robot->attack();

						//���������Ƿ񹥻���Ӣ��
						if(fabsf(this->_hero->getPositionY()-robot->getPositionY())<10){
							if(_hero->_hitBox.actual.intersectsRect(robot->_attackBox.actual)){
								_hero->hurtWithDamage(robot->_damage);

								if(_hero->_actionState==kActionDead && (((GameScene*)(this->getParent()))->_hudLayer->getActionByTag(537)==NULL)){
									this->endGame();
								}
							}
						}
					}else{   //ִ�п��ж���
						robot->idle();
					}
				}else if(distanceSQ <= CCDirector::sharedDirector()->getWinSize().width*CCDirector::sharedDirector()->getWinSize().width){
					robot->_nextDecisionTime=::GetCurTime()+frandom_range(0.5,1.0)*1000;
					randomChoice=random_range(0,2);

					if(randomChoice==0){  //������ִ�����߶���
						CCPoint moveDirection=ccpNormalize(ccpSub(_hero->getPosition(),robot->getPosition()));  //���׼������

						robot->walkWithDirection(moveDirection);
						robot->updateDesiredPosition(dt*20);
						//robot->setPosition(robot->_desiredPosition);
		

					}else{  //ִ�п��ж���
						robot->idle();
					}
				}
			}
		}
	}

	if(alive==0 && ((GameScene*)(this->getParent()))->_hudLayer->getChildByTag(537)==NULL){
		this->endGame();
	}
}



void GameLayer::endGame()
{
	CCPoint origin=CCDirector::sharedDirector()->getVisibleOrigin();  //��ȡ��Ļ������������
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();  //��ȡ��Ļ���������С

	CCLabelTTF* label=CCLabelTTF::create("restart","Arial",40);
	CCMenuItemLabel* restartItem=CCMenuItemLabel::create(label,this,callfuncO_selector(GameLayer::restartGame));
	
	
	CCMenu* menu=CCMenu::create(restartItem,NULL);
	menu->setPosition(ccp(origin.x+size.width/2,origin.y+size.height/2));
	menu->setTag(537);
	
	if(((GameScene*)(this->getParent()))->_hudLayer !=NULL){
		((GameScene*)(this->getParent()))->_hudLayer->addChild(menu);
	}

}

void GameLayer::restartGame(CCObject* pSender)
{

	CCDirector::sharedDirector()->replaceScene((CCScene*)GameScene::create());

}