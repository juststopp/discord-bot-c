#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>
#include <stdbool.h>

#include "../JSON.c"
#include "../Utils/Strings.c"

#define TOKEN "OTAwMzczNjc5OTM3MDU2ODU5.GcKKI9.pxPjhy1cVxwo0rMbViqFNrr0dTzW3YVPdrgt2A"

bool CONNECTED = false;

static int callback_received(void *user, void *in, size_t len) {
    cJSON *data = parse(in);

    cJSON *res = cJSON_GetObjectItem(data, "op");
    int code = cJSON_GetNumberValue(res);

    if(code == 0) {
        char *event = cJSON_GetStringValue(cJSON_GetObjectItem(data, "t"));
        if(event != NULL && strstr(event, "MESSAGE_CREATE") != NULL) {
            cJSON *message_data = cJSON_GetObjectItem(data, "d");
            char *message_id = cJSON_GetStringValue(cJSON_GetObjectItem(message_data, "id"));
            char *content = cJSON_GetStringValue(cJSON_GetObjectItem(message_data, "content"));

            printf("New message:\n");
            printf("- ID: %s\n", message_id);
            printf("- Content: %s\n", content);
        }
    }

    return 0;
}

static int callback_websocket(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch(reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Connected to Discord gateway.\n");
            CONNECTED = true;

            char payload[1024];
            snprintf(payload,
                     sizeof(payload),
                     "{\"op\": 2, \"d\": {\"token\": \"%s\", \"intents\": 3276799, \"properties\": {\"$os\": \"linux\", \"$browser\": \"my_library\", \"$device\": \"my_library\"}}}",
                     TOKEN);
            lws_write(wsi, (unsigned char *)payload, strlen(payload), LWS_WRITE_TEXT);
            break;
        case LWS_CALLBACK_CLIENT_RECEIVE:
            callback_received(user, in, len);
            break;
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            printf("Connection Error: %s\n", in ? (char * ) in : "(null)");
            break;
        case LWS_CALLBACK_CLOSED:
            printf("Connection Closed\n");
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            if (CONNECTED) {
                char presence_payload[1024];
                snprintf(presence_payload, sizeof(presence_payload),
                         "{\"op\": 3, \"d\": {\"since\": 0, \"activities\": [{\"name\": \"with C\", \"type\": 0}], \"status\": \"online\", \"afk\": false}}");
                lws_write(wsi, (unsigned char *)presence_payload, strlen(presence_payload), LWS_WRITE_TEXT);
            }
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
            printf("Appending handshake header\n");
            break;
        case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED:
            printf("Confirming extension support\n");
            break;
        default:
            break;
    }

    return 0;
}

int run() {
    struct lws_context *context;
    struct lws_context_creation_info info;
    struct lws_protocols protocols[] = {
            { "discord-gateway", callback_websocket, 0, 65536, 0, NULL, 0 }
    };

    memset(&info, 0, sizeof info);
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    context = lws_create_context(&info);
    if(!context) {
        fprintf(stderr, "lws init failed.\n");
        return -1;
    }

    struct lws_client_connect_info ccinfo = { 0 };
    ccinfo.context = context;
    ccinfo.address = "gateway.discord.gg";
    ccinfo.port = 443;
    ccinfo.path = "/?v=10&encoding=json";
    ccinfo.host = "gateway.discord.gg";
    ccinfo.origin = "origin";
    ccinfo.protocol = "discord-gateway";
    ccinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    if(!wsi) {
        fprintf(stderr, "Connection failed.\n");
        return -1;
    }

    int n = 0;
    while(n >= 0) {
        n = lws_service(context, 1000);
    }

    lws_context_destroy(context);

    return 0;
}