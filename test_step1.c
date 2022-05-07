#include <stdio.h>
#include <stdlib.h>
#include "storage.h"
#include <string.h>

int main() {
    channel_list_t *channels = get_channels();
	channels->head = channels->tail = NULL;
	// channel_t *weather = create_channel(channels, "weather");
	create_channel(channels, "news");
    // create_channel(channels, "the_empty_channel");

	// add_message(weather, "the weather is going to be great!");
	// add_message(weather, "sunny with a high of 70F");

    channel_t *news = get_channel(get_channels(), "news");
    const char *foo[256];
    strcpy(foo, "summer vacation is almost here");
    add_message(news, foo);
    dump(channels);
    strcpy(foo, "yay!");
    // add_message(news, "fuck this");
    dump(channels);
    // strcpy(foo, ":-)");
    // add_message(news, foo);
	// dump(channels);

    // free_channels(get_channels());

    // dump(channels);

    return 0;
}