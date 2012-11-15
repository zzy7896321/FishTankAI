#ifndef _ST20_H_
#define _ST20_H_

#include "fish.h"
#include <map>
#include <vector>

class st20:virtual public fish{

public:
    void init();

    void play();

    void revive(int&,int&);

    st20():idTable(), ETMaxHP(), ETSpeed(), ifDead(), PosX(), PosY(),
           CurRound(-1), DataValid(false), playerCount(0),
           EmergencyPercentage(10), EmergencyAbsoluteValue(50),
           NormalPercentage(60), NormalAbsoluteValue(200),
           RadicalPercentage(100), RadicalAbsoluteValue(0x7F7F7F),
           iEmergencyPropertyCostForHPCoefficient(0),
           iEmergencyPropertyCostForSPCoefficient(-10000),
           iEmergencyPropertyCostForSTCoefficient(-10000),
           iEmergencyETInRangeEnemyCoefficient(-100),
           iEmergencyETNotInRangeEnemyCoefficient(-50),
           iEmergencyFoodBenefitCoefficient(50000),
           iEmergencyOneHitKillBenefitCoefficient(2000),
           iEmergencyAttackBenefitCoefficient(-20000),
           iEmergencyMaxHPAdded(2),
           iEmergencyMaxSTAdded(0),
           iEmergencyMaxSPAdded(0),
           iNormalPropertyCostForHPCoefficient(-10000),
           iNormalPropertyCostForSPCoefficient(-10000),
           iNormalPropertyCostForSTCoefficient(-10000),
           iNormalETInRangeEnemyCoefficient(-100),
           iNormalETNotInRangeEnemyCoefficient(-50),
           iNormalFoodBenefitCoefficient(10000),
           iNormalOneHitKillBenefitCoefficient(50000),
           iNormalAttackBenefitCoefficient(10000),
           iNormalMaxHPAdded(0),
           iNormalMaxSTAdded(0),
           iNormalMaxSPAdded(0),
           iRadicalPropertyCostForHPCoefficient(-10000),
           iRadicalPropertyCostForSPCoefficient(0),
           iRadicalPropertyCostForSTCoefficient(0),
           iRadicalETInRangeEnemyCoefficient(-50),
           iRadicalETNotInRangeEnemyCoefficient(-20),
           iRadicalFoodBenefitCoefficient(5000),
           iRadicalOneHitKillBenefitCoefficient(100000),
           iRadicalAttackBenefitCoefficient(50000),
           iRadicalMaxHPAdded(0),
           iRadicalMaxSTAdded(2),
           iRadicalMaxSPAdded(2)
           {}
private:
    int mapc[N+1][M+1];
    typedef std::map<int, int>::iterator mapIterator;
    typedef std::map<int, int>::const_iterator mapConstIterator;
    typedef std::vector<int>::iterator vectorIterator;
    typedef std::vector<int>::const_iterator vectorConstIterator;

    std::map<int, int> idTable;
    std::vector<int> ETMaxHP;
    std::vector<int> ETSpeed;
    std::vector<bool> ifDead;
    std::vector<int> PosX;
    std::vector<int> PosY;
    int CurRound;
    bool DataValid;
    int myId;
    int myHP;
    int mySP;
    int myST;
    int myMaxHP;
    int myRemainingPoint;
    int playerCount;

    int EmergencyPercentage;
    int EmergencyAbsoluteValue;
    int NormalPercentage;
    int NormalAbsoluteValue;
    int RadicalPercentage;
    int RadicalAbsoluteValue;

    static const int dx[4];
    static const int dy[4];

    struct GridEvaluator{
        int iMoveX, iMoveY;
        int HPGoal;
        int iAddHPBeforeAction;
        int iAddStrengthBeforeAction;
        int iAddSpeedBeforeAction;
        int iTargetX, iTargetY;
        int iEnvironmentDanger;
        int iEvaluationResult;
        void Evaluate(st20& mySelf);
        static int iPropertyCostForHPCoefficient;
        static int iPropertyCostForSPCoefficient;
        static int iPropertyCostForSTCoefficient;
        static int iETInRangeEnemyCoefficient;
        static int iETNotInRangeEnemyCoefficient;
        static int iFoodBenefitCoefficient;
        static int iOneHitKillBenefitCoefficient;
        static int iAttackBenefitCoefficient;
        static int iMaxHPAdded;
        static int iMaxSTAdded;
        static void SetCoefficients(int pcfhpc, int pcfspc, int pcfstc, int eirec, int enirec,
                                    int fbc, int ohkbc, int abc, int mhpa, int msta){
            iPropertyCostForHPCoefficient = pcfhpc;
            iPropertyCostForSPCoefficient = pcfspc;
            iPropertyCostForSTCoefficient = pcfstc;
            iETInRangeEnemyCoefficient = eirec;
            iETNotInRangeEnemyCoefficient = enirec;
            iFoodBenefitCoefficient = fbc;
            iOneHitKillBenefitCoefficient = ohkbc;
            iAttackBenefitCoefficient = abc;
            iMaxHPAdded = mhpa;
            iMaxSTAdded = msta;
        }
    };

    friend class GridEvaluator;

    int iEmergencyPropertyCostForHPCoefficient;
    int iEmergencyPropertyCostForSPCoefficient;
    int iEmergencyPropertyCostForSTCoefficient;
    int iEmergencyETInRangeEnemyCoefficient;
    int iEmergencyETNotInRangeEnemyCoefficient;
    int iEmergencyFoodBenefitCoefficient;
    int iEmergencyOneHitKillBenefitCoefficient;
    int iEmergencyAttackBenefitCoefficient;
    int iEmergencyMaxHPAdded;
    int iEmergencyMaxSTAdded;
    int iEmergencyMaxSPAdded;

    int iNormalPropertyCostForHPCoefficient;
    int iNormalPropertyCostForSPCoefficient;
    int iNormalPropertyCostForSTCoefficient;
    int iNormalETInRangeEnemyCoefficient;
    int iNormalETNotInRangeEnemyCoefficient;
    int iNormalFoodBenefitCoefficient;
    int iNormalOneHitKillBenefitCoefficient;
    int iNormalAttackBenefitCoefficient;
    int iNormalMaxHPAdded;
    int iNormalMaxSTAdded;
    int iNormalMaxSPAdded;

    int iRadicalPropertyCostForHPCoefficient;
    int iRadicalPropertyCostForSPCoefficient;
    int iRadicalPropertyCostForSTCoefficient;
    int iRadicalETInRangeEnemyCoefficient;
    int iRadicalETNotInRangeEnemyCoefficient;
    int iRadicalFoodBenefitCoefficient;
    int iRadicalOneHitKillBenefitCoefficient;
    int iRadicalAttackBenefitCoefficient;
    int iRadicalMaxHPAdded;
    int iRadicalMaxSTAdded;
    int iRadicalMaxSPAdded;
};

#endif  //_ST20_H_

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！
