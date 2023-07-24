/**
 * @file uart.c
 * @briefParser Library for SCPI-like Messages
 * This library provides functionality for parsing SCPI-like messages
 * using a tree structure to handle different commands and variables.
 * @note The library assumes a sequential buffer representation and provides
 * functions for matching values, consuming the buffer, and executing callbacks
 * associated with the parsed nodes.
 */

#include "parser.h"
#include <string.h>

/**
 * @brief Match function
 * @param data Pointer to the data to be matched
 * @param match_value Pointer to the value to be matched
 * @return EQUAL if the data matches the value, NOT_EQUAL otherwise
 */
parser_match_t match_func(void *data, const void *match_value)
{
	LOG_INFO("trying match");

	if (!memcmp((char *)data, (const char *)match_value, 5))
	{
		LOG_INFO("equal");
		return EQUAL;
	}
	LOG_INFO("not equal: %s, %s", (char *)data, (char *)match_value);
	return NOT_EQUAL;
}

/**
 * @brief Consumer function
 * @param buffer Pointer to the buffer to be consumed
 * @return OK if the buffer was consumed, ERROR otherwise, END_OF_BUFFER if the
 * end of the buffer was reached
 */
parser_consumer_data_t consumer(struct buffer_t *buffer)
{
	if (buffer->actual == NULL)
	{
		LOG_ERROR("error reading buffer: buffer is NULL");
		return ERROR;
	}

	/* Message have 5 chars size */
	buffer->actual += sizeof(char) * 5;

	if (buffer->actual >= buffer->end)
	{
		LOG_INFO("end of buffer");
		return END_OF_BUFFER;
	}

	return OK;
}

/**
 * @brief Convert a buffer to an integer
 * @param buffer Pointer to the buffer to be converted
 * @return The integer value of the buffer
 */
int buffer2int(buffer_t *buffer)
{
	return atoi((char *)buffer->actual); 
}

/**
 * @brief Callback function for var1 node
 * 
 * @param buffer pointer to the buffer
 * @param value pointer to the value
 */
void write_var1(buffer_t *buffer, const void *value)
{
	printf("writing var1, value %d\n", buffer2int(buffer));
}

/**
 * @brief Callback function for var2 node
 * 
 * @param buffer pointer to the buffer
 * @param value pointer to the value
 */
void write_var2(buffer_t *buffer, const void *value)
{
	printf("writing var2, value %d\n", buffer2int(buffer));
}

/**
 * @brief Callback function for var3 node
 * 
 * @param buffer pointer to the buffer
 * @param value pointer to the value
 */
void write_var3(buffer_t *buffer, const void *value)
{
	printf("writing var3, value %d\n", buffer2int(buffer));
}

/**
 * @brief Callback function for allm node
 * 
 * @param buffer pointer to the buffer
 * @param value pointer to the value
 */
void read_allm(buffer_t *buffer, const void *value)
{
	printf("Reading all measurements\n");
}

int main()
{
	char uart_buffer[] = "WRTE:VAR2:VAR3:1000";

/* Describe the buffer */
	buffer_t buffer = {
		.begin = uart_buffer,
		.actual = uart_buffer,
		.end = &uart_buffer[sizeof(uart_buffer) / sizeof(char)],
		.consumer = &consumer};

/**
 * Read SCPI like messages all commands have 5 chars size
 * example:
 * XXXX:XXXX:XXXX:XXXX
 * XXXX: is a command
 * 
 * WRTE: Write something
 *      VAR1:XXXX Write the var 1 with the value XXXX.
 *      VAR2:XXXX Write the var 2 with the value XXXX, if the next value is VAR3: read the var 3. 
 * READ: 
 *      ALLM: Read all measurements
 * 
 */
	MAKE_NODES(
		var1,
		&match_func,
		{
			MAKE_WILDCARD_NODE(&write_var1, NULL),
		});

	MAKE_NODES(
		var3,
		&match_func,
		{
			MAKE_WILDCARD_NODE(&write_var3, NULL),
		});

	MAKE_NODES(
		var2,
		&match_func,
		{
			MAKE_NODE("VAR3:", NULL, &var3),
			MAKE_WILDCARD_NODE(&write_var2, NULL),
		});

	MAKE_NODES(
		write,
		&match_func,
		{
			MAKE_NODE("VAR1:", NULL, &var1),
			MAKE_NODE("VAR2:", NULL, &var2),
		});

	MAKE_NODES(
		allm,
		&match_func,
		{
			MAKE_WILDCARD_NODE(&read_allm, NULL),
		});

	MAKE_NODES(
		read,
		&match_func,
		{
			MAKE_NODE("ALLM:", NULL, &allm),
		});

	MAKE_NODES(
		root,
		&match_func,
		{
			MAKE_NODE("WRTE:", NULL, &write),
			MAKE_NODE("READ:", NULL, &read),
		});

	parser(&buffer, &root);
}