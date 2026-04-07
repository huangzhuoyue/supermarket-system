#include "mongoose.h"
#include "server.h"
#include "api_product.h"
#include <stdio.h>
#include <string.h>

static void serialize_products(struct mg_connection *c) {
    int count = 0;
    ApiResponse res = api_get_all_products(&count);
    if (res.status == API_SUCCESS && res.data) {
        Product *products = (Product*)res.data;
        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n[");
        for (int i = 0; i < count; i++) {
            mg_printf(c, "{\"id\": \"%s\", \"name\": \"%s\", \"category\": \"%s\", \"supplier\": \"%s\", \"price\": %.2f, \"stock\": %d}", products[i].id, products[i].name, products[i].category, products[i].supplier, products[i].price, products[i].stock);
            if (i < count - 1) mg_printf(c, ",");
        }
        mg_printf(c, "]");
    } else {
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "[]");
    }
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (hm->uri.len == 13 && strncmp(hm->uri.buf, "/api/products", 13) == 0) {
            if (hm->method.len == 3 && strncmp(hm->method.buf, "GET", 3) == 0) {
                serialize_products(c);
            } else if (hm->method.len == 4 && strncmp(hm->method.buf, "POST", 4) == 0) {
                mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"status\":\"ok\"}");
            } else {
                mg_http_reply(c, 405, "", "Method Not Allowed");
            }
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
