#ifndef DATA_H
#define DATA_H

#define WIDE 1000
#define HEIGHT 700

#define X_START 100
#define Y_START 100

#define SPACING 35
#define SCANSCOPE 16

#define DOUBLE_RULE_3 100
#define DOUBLE_RULE_4 5000
#define DOUBLE_RULE_5 100000
#define RULE_3_4 400
#define RULE_3_5 800
#define RULE_4_5 30000


//取定AI和人的分数比例系数，根据系数比值分为防守型和进攻型
#define ATTACK_AI_COE 2//进攻
#define ATTACK_HUM_COE 1

#define DEFEND_AI_COE 1//防守
#define DEFEND_HUM_COE 2

#define DEPTH 2//min-max的深度

#endif // DATA_H
