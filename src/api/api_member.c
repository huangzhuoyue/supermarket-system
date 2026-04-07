#include "api_member.h"

ApiResponse api_add_member(Member member) {
    ApiResponse res;
    if (addMember(member)) res.status = API_SUCCESS;
    else res.status = API_ERROR_INTERNAL;
    return res;
}



ApiResponse api_update_member(Member member) {
    ApiResponse res;
    if (updateMember(member)) res.status = API_SUCCESS;
    else res.status = API_ERROR_NOT_FOUND;
    return res;
}

ApiResponse api_find_member_by_id(char *id) {
    ApiResponse res;
    Member *m = findMemberById(id);
    if (m) { res.status = API_SUCCESS; res.data = m; }
    else { res.status = API_ERROR_NOT_FOUND; res.data = NULL; }
    return res;
}

ApiResponse api_find_member_by_phone(char *phone) {
    ApiResponse res;
    Member *m = findMemberByPhone(phone);
    if (m) { res.status = API_SUCCESS; res.data = m; }
    else { res.status = API_ERROR_NOT_FOUND; res.data = NULL; }
    return res;
}
