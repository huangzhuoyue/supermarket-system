#ifndef MEMBER_H
#define MEMBER_H

#include "common.h"

// 会员管理函数声明
int addMember(Member member);
int updateMember(Member member);
Member* findMemberById(char *id);
Member* findMemberByPhone(char *phone);
int addPoints(char *member_id, int points);
int usePoints(char *member_id, int points);
int updateMemberLevel(char *member_id);
int createCoupon(char *member_id, float amount);
int useCoupon(char *coupon_id);
void displayMember(Member member);
void displayAllMembers();
int isMemberIdExists(char *id);
int isMemberPhoneExists(char *phone);

#endif // MEMBER_H
