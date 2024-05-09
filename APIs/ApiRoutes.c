#include "../Utils/Strings.c"

char * get_messages_link(char channel_id[]) {
    char buffer[80] = "https://discord.com/api/v10/channels/{CHANNEL_ID}/messages";
    return str_replace(buffer, "{CHANNEL_ID}", channel_id);
}

char * send_message_link(char channel_id[]) {
    char buffer[80] = "https://discord.com/api/v10/channels/{CHANNEL_ID}/messages";
    return str_replace(buffer, "{CHANNEL_ID}", channel_id);
}