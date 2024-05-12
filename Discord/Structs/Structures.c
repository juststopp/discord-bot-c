#include <stdbool.h>
#include "../../cJSON/cJSON.h"

struct User {
    int id;
    char username[64];
    bool bot;
};

struct Message {
    int id;
    int channel_id;
    int reference;
    struct User author;
    char content[2048];
    bool pinned;
    bool tts;
    bool everyone_mention;
    cJSON user_mentions;
    cJSON role_mentions;
    cJSON channel_mentions;
    cJSON components;
};