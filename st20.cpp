#include "st20.h"
#include <cstdlib>

static inline int max(int a, int b){
    return (a>b)?a : b;
}

static inline int min(int a, int b){
    return (a<b)?a : b;
}

static inline int ManDis(int x1, int y1, int x2, int y2){
    return abs(x1-x2) + abs(y1-y2);
}

const int st20::dx[4] = {1,0,-1,0};
const int st20::dy[4] = {0,1,0,-1};

int st20::GridEvaluator::iPropertyCostForHPCoefficient = 0;
int st20::GridEvaluator::iPropertyCostForSPCoefficient = 0;
int st20::GridEvaluator::iPropertyCostForSTCoefficient = 0;
int st20::GridEvaluator::iETInRangeEnemyCoefficient = 0;
int st20::GridEvaluator::iETNotInRangeEnemyCoefficient = 0;
int st20::GridEvaluator::iFoodBenefitCoefficient = 0;
int st20::GridEvaluator::iOneHitKillBenefitCoefficient = 0;
int st20::GridEvaluator::iAttackBenefitCoefficient = 0;
int st20::GridEvaluator::iMaxHPAdded = 0;
int st20::GridEvaluator::iMaxSTAdded = 0;

void st20::GridEvaluator::Evaluate(st20& mySelf){
    iEvaluationResult = iAddSpeedBeforeAction * iPropertyCostForSPCoefficient;
    int iPropertyPointLeft = mySelf.myRemainingPoint - iAddSpeedBeforeAction;
    iAddHPBeforeAction = iAddStrengthBeforeAction = 0;
    for (int i = 1; i<=N; ++i)
    for (int j = 1; j<=M; ++j)
    if ((mySelf.mapc[i][j]!=EMPTY) && (mySelf.mapc[i][j]!=FOOD) && (mySelf.mapc[i][j]!=mySelf.myId))
        iEvaluationResult += ((ManDis(iMoveX, iMoveY, i, j) <= mySelf.ETSpeed[mySelf.idTable[mySelf.mapc[i][j]]])
            ? iETInRangeEnemyCoefficient : iETNotInRangeEnemyCoefficient);
    {
        int maxV = -2000000000;
        iTargetX = -1;
        iTargetY = -1;
        for (int k=0; k<4; ++k){
            int tx = iMoveX + mySelf.dx[k];
            int ty = iMoveY + mySelf.dy[k];
            if (tx>=1 && tx<=N && ty>=1 && ty<=M){
                int target = mySelf.mapc[tx][ty];
                if (target!=EMPTY){
                    if (target == FOOD){
                        int l =0, r = min(iPropertyPointLeft, iMaxHPAdded);
                        int ihp = 0;
                        while (l<=r){
                            int mid = (l+r) >> 1;
                            if (2*mid + mySelf.myHP + max(2, (mySelf.myMaxHP+2*mid)/10) >= HPGoal){
                                ihp = mid;
                                r = mid-1;
                            } else {
                                l = mid+1;
                            }
                        }
                        int V = ihp * iPropertyCostForHPCoefficient + iFoodBenefitCoefficient;
                        if (V>maxV){
                            iAddHPBeforeAction = ihp;
                            iAddStrengthBeforeAction = 0;
                            iTargetX = tx;
                            iTargetY = ty;
                            maxV = V;
                        }
                    }
                    else if (target!=mySelf.myId){
                        int ist = max(0, mySelf.askHP(target) - mySelf.myST);
                        ist = (ist>min(iMaxSTAdded, iPropertyPointLeft)) ? 0 : ist;
                        int V = - iETInRangeEnemyCoefficient + ist * iPropertyCostForSTCoefficient;
                        if (mySelf.askHP(target) > mySelf.myST + ist){
                            V += iAttackBenefitCoefficient;
                        } else {
                            V += iOneHitKillBenefitCoefficient;
                        }
                        if (V>maxV){
                            iAddHPBeforeAction = 0;
                            iAddStrengthBeforeAction = ist;
                            iTargetX = tx;
                            iTargetY = ty;
                            maxV = V;
                        }
                    }
                }
            }
        }
        if (iTargetX!=-1) iEvaluationResult += maxV;
    }
}

void st20::init(){
    //set property
    for (int i=0; i<5; ++i) increaseHealth();
    for (int i=0; i<2; ++i) increaseStrength();
    for (int i=0; i<3; ++i) increaseSpeed();
    myId = getID();
    //get map
    {
        for (int i = 1; i<=N; ++i)
        for (int j = 1; j<=M; ++j){
            mapc[i][j] = askWhat(i, j);
            if (mapc[i][j]!=FOOD && mapc[i][j]!=EMPTY){
                idTable[mapc[i][j]] = playerCount++;
                ETMaxHP.push_back(askHP(mapc[i][j]));
                ETSpeed.push_back(1);
                ifDead.push_back(false);
                PosX.push_back(i);
                PosY.push_back(j);
            }
        }
    }
}

void st20::play(){
    //Update info
    ++CurRound;
    if (DataValid){
        for (int i = 0; i<playerCount; ++i){
            ifDead[i] = true;
        }
        for (int i = 1; i<=N; ++i)
        for (int j = 1; j<=M; ++j){
            int now = askWhat(i, j);
            if (now!=FOOD && now!=EMPTY){
                if (idTable.find(now)==idTable.end()){
                    idTable[now] = playerCount++;
                    ETMaxHP.push_back(askHP(now));
                    ETSpeed.push_back(1);
                    ifDead.push_back(false);
                    PosX.push_back(i);
                    PosY.push_back(j);
                } else{
                    int id = idTable[now];
                    ETMaxHP[id] = max(ETMaxHP[id], askHP(now));
                    ETSpeed[id] = max(ETSpeed[id], ManDis(PosX[id], PosY[id], i, j));
                    ifDead[id] = false;
                    PosX[id] = i;
                    PosY[id] = j;
                }
            }
            mapc[i][j] = now;
        }
    }
    else{
        for (int i = 0; i<playerCount; ++i){
            ifDead[i] = true;
        }
        for (int i = 1; i<=N; ++i)
        for (int j = 1; j<=M; ++j){
            int now = askWhat(i, j);
            if (now!=FOOD && now!=EMPTY){
                if (idTable.find(now)==idTable.end()){
                    idTable[now] = playerCount++;
                    ETMaxHP.push_back(askHP(now));
                    ETSpeed.push_back(1);
                    ifDead.push_back(false);
                    PosX.push_back(i);
                    PosY.push_back(j);
                } else{
                    int id = idTable[now];
                    ETMaxHP[id] = max(ETMaxHP[id], askHP(id));
                    ifDead[id] = false;
                    PosX[id] = i;
                    PosY[id] = j;
                }
            }
            mapc[i][j] = now;
        }
        DataValid = true;
    }

    //Decide where to move and attack
    {
        mySP = getSp();
        myST = getAtt();
        myHP = getHP();
        myMaxHP = getMaxHP();
        myRemainingPoint = getPoint();
        int iMaxSPAdded = 0;
        int myLevel = getLevel();
        if (myHP <= EmergencyPercentage * myMaxHP / 100 && myHP <= EmergencyAbsoluteValue ){
            GridEvaluator::SetCoefficients(iEmergencyPropertyCostForHPCoefficient,
                                          iEmergencyPropertyCostForSPCoefficient,
                                          iEmergencyPropertyCostForSTCoefficient,
                                          iEmergencyETInRangeEnemyCoefficient,
                                          iEmergencyETNotInRangeEnemyCoefficient,
                                          iEmergencyFoodBenefitCoefficient,
                                          iEmergencyOneHitKillBenefitCoefficient,
                                          iEmergencyAttackBenefitCoefficient,
                                          iEmergencyMaxHPAdded,
                                          iEmergencyMaxSTAdded);
            iMaxSPAdded = iEmergencyMaxSPAdded;
        }   //emergency. priority is food. only using remaining property points for HP is allowed
        else if (myHP <= NormalPercentage * myMaxHP / 100 && myHP <= NormalAbsoluteValue){
            GridEvaluator::SetCoefficients(iNormalPropertyCostForHPCoefficient,
                                          iNormalPropertyCostForSPCoefficient,
                                          iNormalPropertyCostForSTCoefficient,
                                          iNormalETInRangeEnemyCoefficient,
                                          iNormalETNotInRangeEnemyCoefficient,
                                          iNormalFoodBenefitCoefficient,
                                          iNormalOneHitKillBenefitCoefficient,
                                          iNormalAttackBenefitCoefficient,
                                          iNormalMaxHPAdded,
                                          iNormalMaxSTAdded);
            iMaxSPAdded = iNormalMaxSPAdded;
        }   //normal, priority is one-hit-kill fish. any use of remaing property points is not allowed
        else // if (myHP<=RadicalPercentage * myMaxHP / 100 && myHP <= RadicalAbsoluteValue)
        {
            GridEvaluator::SetCoefficients(iRadicalPropertyCostForHPCoefficient,
                                          iRadicalPropertyCostForSPCoefficient,
                                          iRadicalPropertyCostForSTCoefficient,
                                          iRadicalETInRangeEnemyCoefficient,
                                          iRadicalETNotInRangeEnemyCoefficient,
                                          iRadicalFoodBenefitCoefficient,
                                          iRadicalOneHitKillBenefitCoefficient,
                                          iRadicalAttackBenefitCoefficient,
                                          iRadicalMaxHPAdded,
                                          iRadicalMaxSTAdded);
            iMaxSPAdded = iRadicalMaxSPAdded;
        }   //radical, priority is fish, only use of remaining property points for Strength or Speed is allowed.

        int iInitialX = getX(), iInitialY = getY();
        GridEvaluator BestChoice;
        BestChoice.HPGoal = (myHP <= EmergencyPercentage * myMaxHP / 100 && myHP <= EmergencyAbsoluteValue) ?
                            min(EmergencyPercentage * myMaxHP / 100, EmergencyAbsoluteValue) : 0;
        BestChoice.iMoveX = iInitialX;
        BestChoice.iMoveY = iInitialY;
        BestChoice.iAddSpeedBeforeAction = 0;
        BestChoice.Evaluate(*this);
        GridEvaluator NextChoice;
        NextChoice.HPGoal = BestChoice.HPGoal;
        NextChoice.iAddSpeedBeforeAction = 0;
        for (int dis = 1; dis<=mySP; ++dis)
        for (int x = iInitialX - dis; x<=iInitialX+dis; ++x)
        if (x>=1 && x<=N){
            int y = iInitialY + dis - abs(x-iInitialX);
            if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                NextChoice.iMoveX = x;
                NextChoice.iMoveY = y;
                NextChoice.Evaluate(*this);
                if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                    BestChoice = NextChoice;
            }
            if (abs(x-iInitialX)!=dis){
                y = iInitialY - dis + abs(x-iInitialX);
                if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                    NextChoice.iMoveX = x;
                    NextChoice.iMoveY = y;
                    NextChoice.Evaluate(*this);
                    if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                        BestChoice = NextChoice;
                }
            }
        }
        for (int spAdded = 1; spAdded<= min(iMaxSPAdded, myRemainingPoint); ++spAdded){
            NextChoice.iAddSpeedBeforeAction = spAdded;
            int curSP = mySP + spAdded;
            for (int x = iInitialX - curSP; x<=iInitialX + curSP; ++x)
            if (x>=1 && x<=N){
                int y = iInitialY + curSP - abs(x - iInitialX);
                if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                    NextChoice.iMoveX = x;
                    NextChoice.iMoveY = y;
                    NextChoice.Evaluate(*this);
                    if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                        BestChoice = NextChoice;
                }
                if (abs(x - iInitialX)!= curSP){
                    y = iInitialY - curSP + abs(x-iInitialX);
                    if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                        NextChoice.iMoveX = x;
                        NextChoice.iMoveY = y;
                        NextChoice.Evaluate(*this);
                        if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                            BestChoice = NextChoice;
                    }
                }
            }
        }

        for (int i = 0 ;i<BestChoice.iAddSpeedBeforeAction; ++i) increaseSpeed();
        for (int i = 0; i<BestChoice.iAddHPBeforeAction; ++i) increaseHealth();
        for (int i = 0; i<BestChoice.iAddStrengthBeforeAction; ++i) increaseStrength();
        if (BestChoice.iMoveX != iInitialX || BestChoice.iMoveY != iInitialY)
            move(BestChoice.iMoveX, BestChoice.iMoveY);
        if (BestChoice.iTargetX!=-1)
            attack(BestChoice.iTargetX, BestChoice.iTargetY);

        if (getLevel() - myLevel){
            for (int i = 0; i<getLevel() - myLevel; ++i){
                if (getSp() < N+M-2) increaseSpeed();
                if (getAtt() < getMaxHP() / 2) increaseStrength(); else increaseHealth();
            }
        }
    }
}

void st20::revive(int& iPosX, int& iPosY){
    ++CurRound;
    DataValid = false;
    iPosX = iPosY = -1;
}
