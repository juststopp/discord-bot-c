#import "../../APIs/ApiCall.c"
#import "../../APIs/ApiRoutes.c"

enum BUTTON_STYLE {
    PRIMARY = 1,
    SECONDARY = 2,
    SUCCESS = 3,
    DANGER = 4,
    LINK = 5
};

struct MessageComponent {
    cJSON *message;
    cJSON *components;
};

struct MessageComponent create_message_component(char content[]) {
    cJSON *message_component = cJSON_CreateObject();
    cJSON_AddStringToObject(message_component, "content", content);

    cJSON *row = cJSON_AddArrayToObject(message_component, "components");

    struct MessageComponent message = { message_component, row };
    return message;
}

cJSON *create_action_row(cJSON *row) {
    cJSON *object = cJSON_CreateObject();

    cJSON_AddNumberToObject(object, "type", 1);
    cJSON_AddItemToArray(row, object);

    cJSON *components = cJSON_AddArrayToObject(object, "components");

    return components;
}

void create_button(cJSON *components, char label[], enum BUTTON_STYLE style, char custom_id[], char url[]) {
    cJSON *object = cJSON_CreateObject();

    cJSON_AddNumberToObject(object, "type", 2);
    cJSON_AddStringToObject(object, "label", label);
    cJSON_AddNumberToObject(object, "style", style);
    if(url == NULL) cJSON_AddStringToObject(object, "custom_id", custom_id);
    if(url != NULL) cJSON_AddStringToObject(object, "url", url);

    cJSON_AddItemToArray(components, object);
}

cJSON *get_messages(char channel_id[]) {
    char *data = get(get_messages_link(channel_id));
    cJSON *parsed_data = parse(data);

    return parsed_data;
}

cJSON *send_message(char channel_id[], cJSON *params) {
    char * parsed_params = cJSON_Print(params);
    char *data = post(send_message_link(channel_id), parsed_params);
    cJSON *parsed_Data = parse(data);

    return parsed_Data;
}