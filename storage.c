#include "storage.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

channel_list_t *channel_list = NULL;

channel_list_t *get_channels() {
	if (channel_list == NULL) {
		channel_list = (channel_list_t *) malloc(sizeof(channel_list_t));
		channel_list->head = NULL;
		channel_list->tail = NULL;
	}
	
    return channel_list;
}

channel_t *create_channel(channel_list_t *channels, const char *name) {

	// if channel list is empty, make new channel and set it as head
	if (channels->head == NULL) {
		channel_t *new_c = (channel_t *) malloc(sizeof(channel_t));
		new_c->name = name;
		channels->head = new_c;
	}

	for (channel_t *c = channels->head; c != NULL; c = c->next) {
		// return pointer to channel if it already exists in channel list
		if (c->name == name) {
			return c;
		} if (c->next == NULL) {
			// create channel with name and add to list
			channel_t *new_c = (channel_t *) malloc(sizeof(channel_t));
			new_c->name = name;
			c->next = new_c;
			channels->tail = new_c;
		}
	}

    return NULL;
}

channel_t *get_channel(channel_list_t *channels, const char *name) {

	// iterate through channel list and return channel based on name if it exists
	for (channel_t *c = channels->head; c != NULL; c = c->next) {
		if (strcmp(c->name, name) == 0) {
			return c;
		}
	}

	return NULL;
}


void free_channels(channel_list_t *channels) {

	// for (channel_t *c = channels->head; c != NULL; c = c->next) {
	// 	for (message_t *m = c->head; m != NULL; m = m->next) {
	// 		free(m);
	// 	}
	// 	free(c);
	// }
	free(channels);

}

void add_message(channel_t *channel, const char *text) {

	// if message list is empty, make new message and set it as head
	if (channel->head == NULL) {
		message_t *m = (message_t *) malloc(sizeof(message_t));
		m->text = text;
		m->id = 0;
		channel->last_msg = 1;
		channel->head = m;
		channel->tail = m;
	} else {
		message_t *old_tail = channel->tail;
		message_t *m = (message_t *) malloc(sizeof(message_t));
		m->text = text;
		m->id = old_tail->id + 1;
		channel->last_msg += 1;
		old_tail->next = m;
		channel->tail = m;
	}

}

message_t *get_message(channel_t *channel, message_id_t id) {

	// iterate through channel messages and return message based on name if it exists
	for (message_t *m = channel->head; m != NULL; m = m->next) {
		if (m->id == id) {
			return m;
		}
	}

	return NULL; // otherwise return NULL

}

void dump(channel_list_t *channels) {
	for (channel_t *c = channels->head; c != NULL; c = c->next) {
		printf("Channel %s\n", c->name);
		for (message_t *m = c->head; m != NULL; m = m->next) {
			printf("\t[%lu]: %s\n", m->id, m->text);
		}
		printf("\n\n");
	}
}

