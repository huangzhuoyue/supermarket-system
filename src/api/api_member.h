#ifndef API_MEMBER_H
#define API_MEMBER_H

#include "api_types.h"
#include "member.h"

ApiResponse api_add_member(Member member);
ApiResponse api_update_member(Member member);
ApiResponse api_find_member_by_id(char *id);
ApiResponse api_find_member_by_phone(char *phone);

#endif
