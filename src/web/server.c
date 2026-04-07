#include "mongoose.h"
#include "server.h"
#include "api_product.h"
#include "auth.h"
#include "common.h"
#include "sales.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int get_role(struct mg_http_message *hm) {
    struct mg_str *hdr = mg_http_get_header(hm, "X-Role");
    if (hdr && hdr->len > 0) return atoi(hdr->buf);
    return 0;
}

// ----------------- SERIALIZATION UTILS -----------------

static void serialize_users(struct mg_connection *c) {
    mg_printf(c, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n[");
    for (int i = 0; i < user_count; i++) {
        mg_printf(c, "{\"username\": \"%s\", \"role\": %d}", users[i].username, users[i].role);
        if (i < user_count - 1) mg_printf(c, ",");
    }
    mg_printf(c, "]");
    c->is_draining = 1;
}

static void serialize_products(struct mg_connection *c) {
    int count = 0;
    ApiResponse res = api_get_all_products(&count);
    if (res.status == API_SUCCESS && res.data) {
        Product *products = (Product*)res.data;
        mg_printf(c, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n[");
        for (int i = 0; i < count; i++) {
            mg_printf(c, "{\"id\": \"%s\", \"name\": \"%s\", \"category\": \"%s\", \"supplier\": \"%s\", \"price\": %.2f, \"stock\": %d}", products[i].id, products[i].name, products[i].category, products[i].supplier, products[i].price, products[i].stock);
            if (i < count - 1) mg_printf(c, ",");
        }
        mg_printf(c, "]");
        c->is_draining = 1;
    } else {
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "[]");
    }
}

static void serialize_members(struct mg_connection *c) {
    mg_printf(c, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n[");
    for (int i = 0; i < member_count; i++) {
        mg_printf(c, "{\"id\": \"%s\", \"name\": \"%s\", \"phone\": \"%s\", \"points\": %d, \"level\": %d, \"total_amount\": %.2f}", members[i].id, members[i].name, members[i].phone, members[i].points, members[i].level, members[i].total_amount);
        if (i < member_count - 1) mg_printf(c, ",");
    }
    mg_printf(c, "]");
    c->is_draining = 1;
}

static void serialize_coupons(struct mg_connection *c, struct mg_http_message *hm) {
    char mid[32] = {0};
    struct mg_str q = hm->query;
    if (q.len > 0) {
        char *qn = strstr(q.buf, "member_id=");
        if (qn) { 
            char *end = strchr(qn, '&'); 
            if (!end) end = (char *)q.buf + q.len; 
            int c_len = end - (qn + 10);
            if (c_len < 32 && c_len > 0) strncpy(mid, qn + 10, c_len);
        }
    }
    
    mg_printf(c, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n[");
    int first = 1;
    for (int i=0; i<coupon_count; i++) {
        if (coupons[i].is_used == 0 && strcmp(coupons[i].member_id, mid) == 0) {
            if (!first) mg_printf(c, ",");
            mg_printf(c, "{\"id\": \"%s\", \"amount\": %.2f, \"expire_date\": \"%s\"}", coupons[i].id, coupons[i].amount, coupons[i].expire_date);
            first = 0;
        }
    }
    mg_printf(c, "]");
    c->is_draining = 1;
}

// ----------------- HANDLERS -----------------

static void handle_login(struct mg_connection *c, struct mg_http_message *hm) {
    char user[64] = {0}, pass[64] = {0};
    char *b = (char *)hm->body.buf;
    if (!b) { mg_http_reply(c, 400, "", "{\"error\":\"Empty body\"}"); return; }
    
    char *u = strstr(b, "\"username\""); if (u) { u = strchr(u, ':'); if (u) { u = strchr(u, '"'); if (u) { u++; char *e = strchr(u, '"'); if (e && e - u < 64) strncpy(user, u, e - u); } } }
    char *p = strstr(b, "\"password\""); if (p) { p = strchr(p, ':'); if (p) { p = strchr(p, '"'); if (p) { p++; char *e = strchr(p, '"'); if (e && e - p < 64) strncpy(pass, p, e - p); } } }
    
    int role = userLogin(user, pass);
    if (role > 0) {
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"status\":\"ok\", \"role\":%d}", role);
    } else {
        mg_http_reply(c, 401, "Content-Type: application/json\r\n", "{\"error\":\"Invalid credentials\"}");
    }
}

// ... Users & Products handlers (same as before) ...
static void handle_users_post(struct mg_connection *c, struct mg_http_message *hm) {
    if (get_role(hm) != 1) { mg_http_reply(c, 403, "", "{\"error\":\"Forbidden\"}"); return; }
    char username[64] = {0}, password[64] = {0}; int role = 2; char *b = (char *)hm->body.buf;
    if (!b) { mg_http_reply(c, 400, "", "{\"error\":\"Empty body\"}"); return; }
    char *u = strstr(b, "\"username\""); if (u) { u=strchr(u,':'); if(u){ u=strchr(u,'"'); if(u){ u++; char *e=strchr(u,'"'); if(e&&e-u<64)strncpy(username,u,e-u); } } }
    char *p = strstr(b, "\"password\""); if (p) { p=strchr(p,':'); if(p){ p=strchr(p,'"'); if(p){ p++; char *e=strchr(p,'"'); if(e&&e-p<64)strncpy(password,p,e-p); } } }
    char *r = strstr(b, "\"role\""); if (r) { r=strchr(r,':'); if(r){ role=atoi(r+1); } }
    if (strlen(username) > 0 && strlen(password) > 0) {
        for (int i=0; i<user_count; i++) { if (strcmp(users[i].username, username) == 0) { mg_http_reply(c, 400, "", "{\"error\":\"User already exists\"}"); return; } }
        User user; strcpy(user.username, username); strcpy(user.password, password); user.role = role;
        extern int saveUsers(); users[user_count++] = user; saveUsers();
        mg_http_reply(c, 200, "", "{\"status\":\"ok\"}");
    } else { mg_http_reply(c, 400, "", "{\"error\":\"Missing fields\"}"); }
}

static void handle_users_delete(struct mg_connection *c, struct mg_http_message *hm) {
    if (get_role(hm) != 1) { mg_http_reply(c, 403, "", "{\"error\":\"Forbidden\"}"); return; }
    char username[64] = {0}; struct mg_str q = hm->query;
    if (q.len > 0) { char *qn = strstr(q.buf, "username="); if (qn) {  char *end = strchr(qn, '&'); if (!end) end = (char *)q.buf + q.len; int c_len = end - (qn + 9); if (c_len < 64 && c_len > 0) strncpy(username, qn + 9, c_len); } }
    if (strlen(username) > 0) {
        int found = -1;
        for (int i=0; i<user_count; i++) { if (strcmp(users[i].username, username) == 0) { found = i; break; } }
        if (found >= 0) {
            for (int i=found; i<user_count-1; i++) users[i] = users[i+1];
            user_count--; extern int saveUsers(); saveUsers();
            mg_http_reply(c, 200, "", "{\"status\":\"ok\"}");
        } else { mg_http_reply(c, 404, "", "{\"error\":\"Not found\"}"); }
    } else { mg_http_reply(c, 400, "", "{\"error\":\"Missing username param\"}"); }
}

static void handle_products_post(struct mg_connection *c, struct mg_http_message *hm) {
    int r = get_role(hm); if (r != 1 && r != 2) { mg_http_reply(c, 403, "", "{\"error\":\"Forbidden\"}"); return; }
    Product p; memset(&p, 0, sizeof(Product)); p.price = 0.0; p.stock = 0; char *b = (char *)hm->body.buf;
    if (!b) { mg_http_reply(c, 400, "", "{\"error\":\"Empty body\"}"); return; }
    char *t;
    t = strstr(b, "\"id\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<20)strncpy(p.id,t,e-t);}}}
    t = strstr(b, "\"name\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<50)strncpy(p.name,t,e-t);}}}
    t = strstr(b, "\"category\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<20)strncpy(p.category,t,e-t);}}}
    t = strstr(b, "\"supplier\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<50)strncpy(p.supplier,t,e-t);}}}
    t = strstr(b, "\"price\""); if(t){t=strchr(t,':');if(t){p.price = atof(t+1);}}
    t = strstr(b, "\"stock\""); if(t){t=strchr(t,':');if(t){p.stock = atoi(t+1);}}
    if (strlen(p.id) > 0) {
        int found = -1; for (int i=0; i<product_count; i++) { if (strcmp(products[i].id, p.id) == 0) { found = i; break; } }
        if (found >= 0) products[found] = p; else products[product_count++] = p;
        extern int saveProducts(); saveProducts(); mg_http_reply(c, 200, "", "{\"status\":\"ok\"}");
    } else { mg_http_reply(c, 400, "", "{\"error\":\"Missing ID\"}"); }
}

static void handle_products_delete(struct mg_connection *c, struct mg_http_message *hm) {
    int r = get_role(hm); if (r != 1 && r != 2) { mg_http_reply(c, 403, "", "{\"error\":\"Forbidden\"}"); return; }
    char pid[32] = {0}; struct mg_str q = hm->query;
    if (q.len > 0) { char *qn = strstr(q.buf, "id="); if (qn) { char *end = strchr(qn, '&'); if (!end) end = (char *)q.buf + q.len; int c_len = end - (qn + 3); if (c_len < 32 && c_len > 0) strncpy(pid, qn + 3, c_len); } }
    if (strlen(pid) > 0) {
        int found = -1; for (int i=0; i<product_count; i++) { if (strcmp(products[i].id, pid) == 0) { found = i; break; } }
        if (found >= 0) { for (int i=found; i<product_count-1; i++) products[i] = products[i+1]; product_count--; extern int saveProducts(); saveProducts(); mg_http_reply(c, 200, "", "{\"status\":\"ok\"}"); }
        else mg_http_reply(c, 404, "", "{\"error\":\"Not found\"}");
    } else { mg_http_reply(c, 400, "", "{\"error\":\"Missing id\"}"); }
}

static void handle_members_post(struct mg_connection *c, struct mg_http_message *hm) {
    int r = get_role(hm); if (r != 1 && r != 2) { mg_http_reply(c, 403, "", "{\"error\":\"Forbidden\"}"); return; }
    Member m; memset(&m, 0, sizeof(Member)); char *b = (char *)hm->body.buf;
    if (!b) { mg_http_reply(c, 400, "", "{\"error\":\"Empty body\"}"); return; }
    char *t;
    t = strstr(b, "\"id\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<10)strncpy(m.id,t,e-t);}}}
    t = strstr(b, "\"name\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<50)strncpy(m.name,t,e-t);}}}
    t = strstr(b, "\"phone\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<15)strncpy(m.phone,t,e-t);}}}
    m.points = 0; m.level = 1; m.total_amount = 0.0;
    // Set date
    extern char* getCurrentDate(char *dateStr); getCurrentDate(m.reg_date);
    if (strlen(m.id) > 0) {
        int found = -1; for (int i=0; i<member_count; i++) { if (strcmp(members[i].id, m.id) == 0) { found = i; break; } }
        if (found >= 0) members[found] = m; else members[member_count++] = m;
        extern int saveMembers(); saveMembers(); mg_http_reply(c, 200, "", "{\"status\":\"ok\"}");
    } else { mg_http_reply(c, 400, "", "{\"error\":\"Missing ID\"}"); }
}

static void handle_members_delete(struct mg_connection *c, struct mg_http_message *hm) {
    int r = get_role(hm); if (r != 1 && r != 2) { mg_http_reply(c, 403, "", "{\"error\":\"Forbidden\"}"); return; }
    char mid[32] = {0}; struct mg_str q = hm->query;
    if (q.len > 0) { char *qn = strstr(q.buf, "id="); if (qn) { char *end = strchr(qn, '&'); if (!end) end = (char *)q.buf + q.len; int c_len = end - (qn + 3); if (c_len < 32 && c_len > 0) strncpy(mid, qn + 3, c_len); } }
    if (strlen(mid) > 0) {
        int found = -1; for (int i=0; i<member_count; i++) { if (strcmp(members[i].id, mid) == 0) { found = i; break; } }
        if (found >= 0) { for (int i=found; i<member_count-1; i++) members[i] = members[i+1]; member_count--; extern int saveMembers(); saveMembers(); mg_http_reply(c, 200, "", "{\"status\":\"ok\"}"); }
        else mg_http_reply(c, 404, "", "{\"error\":\"Not found\"}");
    } else { mg_http_reply(c, 400, "", "{\"error\":\"Missing id\"}"); }
}

// FORMAT: {"member_id":"18800000001", "points_used": 10, "coupon_id":"C001", "items":"P001:2,P002:1"}
static void handle_checkout_post(struct mg_connection *c, struct mg_http_message *hm) {
    int r = get_role(hm); if (r != 1 && r != 2) { mg_http_reply(c, 403, "", "{\"error\":\"Forbidden\"}"); return; }
    
    char *b = (char *)hm->body.buf;
    if (!b) { mg_http_reply(c, 400, "", "{\"error\":\"Empty body\"}"); return; }
    
    char member_id[20] = "NONE";
    char coupon_id[20] = "NONE";
    char items_str[512] = {0};
    int points_used = 0;
    
    char *t;
    t = strstr(b, "\"member_id\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<20){strncpy(member_id,t,e-t); if(strlen(member_id)==0)strcpy(member_id,"NONE");}}}}
    t = strstr(b, "\"coupon_id\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<20){strncpy(coupon_id,t,e-t); if(strlen(coupon_id)==0)strcpy(coupon_id,"NONE");}}}}
    t = strstr(b, "\"items\""); if(t){t=strchr(t,':');if(t){t=strchr(t,'"');if(t){t++;char *e=strchr(t,'"');if(e&&e-t<512){strncpy(items_str,t,e-t);}}}}
    t = strstr(b, "\"points_used\""); if(t){t=strchr(t,':');if(t){points_used = atoi(t+1);}}
    
    ShoppingCart cart;
    initShoppingCart(&cart);
    
    char *item_pair = strtok(items_str, ",");
    while (item_pair) {
        char p_id[20] = {0};
        int qty = 0;
        sscanf(item_pair, "%[^:]:%d", p_id, &qty);
        if(strlen(p_id) > 0 && qty > 0) {
            addToCart(&cart, p_id, qty);
        }
        item_pair = strtok(NULL, ",");
    }
    
    char *mid_ptr = strcmp(member_id, "NONE") == 0 ? NULL : member_id;
    char *cid_ptr = strcmp(coupon_id, "NONE") == 0 ? NULL : coupon_id;
    
    Transaction *trans = checkout(&cart, mid_ptr, points_used, cid_ptr);
    if (trans) {
        extern int saveProducts(); saveProducts(); // Emulate updating stock files natively
        mg_http_reply(c, 200, "", "{\"status\":\"ok\",\"transaction_id\":\"%s\"}", trans->id);
    } else {
        mg_http_reply(c, 400, "", "{\"error\":\"Checkout failed (No stock or empty)\"}");
    }
}


static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (hm->uri.len == 13 && strncmp(hm->uri.buf, "/api/products", 13) == 0) {
            if (hm->method.len == 3 && strncmp(hm->method.buf, "GET", 3) == 0) serialize_products(c);
            else if (hm->method.len == 4 && strncmp(hm->method.buf, "POST", 4) == 0) handle_products_post(c, hm);
            else if (hm->method.len == 6 && strncmp(hm->method.buf, "DELETE", 6) == 0) handle_products_delete(c, hm);
            else mg_http_reply(c, 405, "", "Method Not Allowed");
        } else if (hm->uri.len == 10 && strncmp(hm->uri.buf, "/api/users", 10) == 0) {
            if (hm->method.len == 3 && strncmp(hm->method.buf, "GET", 3) == 0) serialize_users(c);
            else if (hm->method.len == 4 && strncmp(hm->method.buf, "POST", 4) == 0) handle_users_post(c, hm);
            else if (hm->method.len == 6 && strncmp(hm->method.buf, "DELETE", 6) == 0) handle_users_delete(c, hm);
            else mg_http_reply(c, 405, "", "Method Not Allowed");
        } else if (hm->uri.len == 12 && strncmp(hm->uri.buf, "/api/members", 12) == 0) {
            if (hm->method.len == 3 && strncmp(hm->method.buf, "GET", 3) == 0) serialize_members(c);
            else if (hm->method.len == 4 && strncmp(hm->method.buf, "POST", 4) == 0) handle_members_post(c, hm);
            else if (hm->method.len == 6 && strncmp(hm->method.buf, "DELETE", 6) == 0) handle_members_delete(c, hm);
            else mg_http_reply(c, 405, "", "Method Not Allowed");
        } else if (hm->uri.len == 12 && strncmp(hm->uri.buf, "/api/coupons", 12) == 0) {
            if (hm->method.len == 3 && strncmp(hm->method.buf, "GET", 3) == 0) serialize_coupons(c, hm);
            else mg_http_reply(c, 405, "", "Method Not Allowed");
        } else if (hm->uri.len == 13 && strncmp(hm->uri.buf, "/api/checkout", 13) == 0) {
            if (hm->method.len == 4 && strncmp(hm->method.buf, "POST", 4) == 0) handle_checkout_post(c, hm);
            else mg_http_reply(c, 405, "", "Method Not Allowed");
        } else if (hm->uri.len == 10 && strncmp(hm->uri.buf, "/api/login", 10) == 0) {
            if (hm->method.len == 4 && strncmp(hm->method.buf, "POST", 4) == 0) handle_login(c, hm);
            else mg_http_reply(c, 405, "", "Method Not Allowed");
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "public"};
            mg_http_serve_dir(c, ev_data, &opts);
        }
    }
}

void start_web_server(const char *port) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    char listen_url[32];
    snprintf(listen_url, sizeof(listen_url), "http://0.0.0.0:%s", port);
    printf("Starting web server on %s\n", listen_url);
    mg_http_listen(&mgr, listen_url, fn, NULL);
    while (1) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
}
