/* Simple example demonstrating how to use libtr50 to connect to the
 * telit cloud.  This is intended to be used as a learning tool, this
 * is not a product.  There will be errors, e.g. I don't check return
 * codes for simplicity.
 */
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <tr50/tr50.h>
#include <tr50/worker.h>

#define CALLBACK "method_callback"
#define PASS 0
#define FAIL 1
#define PASS_MSG "Successful"
#define FAIL_MSG "Failure"

/* callback handler for all methods.  The method name is returned
 * here.  Do a strcmp and handle that method. One method returns a
 * failure, the other a success.  Success does not return a result
 * string. */
int _method_callback(void* tr50, const char *id, const char *thing_key,
		const char *method, const char *from, JSON *params, void* custom)
{
	int ret = FAIL;
	char result[256];
	memset(result, 0, sizeof(result) );
	printf("calling method=%s\n", method);
	tr50_method_update(tr50, id, "in progress");
	if ( strcmp(method, "method_1") == 0 )
	{
		printf( "Executing method_1 handler\n" );
		strncpy(result, FAIL_MSG, strlen( FAIL_MSG) );
	}
	else if ( strcmp(method, "method_2") == 0 )
	{
		ret = PASS;
		strncpy(result, PASS_MSG, strlen( PASS_MSG) );
		printf( "Executing method_2 handler\n" );
	}

	tr50_method_ack(tr50, id, ret, result, NULL);
	return 0;
}

int main(int argc, char *argv[])
{
	double value;
	int rand_value;
	int state;
	void * tr50;

	/* credentials arg 1 thing_key, arg 2 app_token.  I used
	 * defaults for simplicity */
	char *thing_key;
	char *app_token;
	if ( argc > 2 )
	{
		thing_key = argv[1];
		app_token = argv[2];
	}
	else
	{
		printf ("Usage: %s <thing key> <app token>\n", argv[0]);
		return 1;
	}
	printf( "Command line args: %s %s\n", thing_key, app_token );

	/* cloud broker connection */
	const char *end_point = "api.devicewise.com";
	int mqtt_port = 1883;

	/* example attributes, could be anything */
	const char *test_attr = "Demo_Version";
	char *test_attr_value = "0.1";

	/* create connection context */
	if ( tr50_create(&tr50, thing_key, end_point, mqtt_port) != 0 )
	{
		printf( "tr50_create(): ERROR\n" );
		return 1;
	}
	else
		printf("tr50_create(): OK\n");

	/* use ssl */
	/*tr50_config_set_ssl(tr50, 1);*/

	/* username, password */
	tr50_config_set_username( tr50, thing_key );
	tr50_config_set_password( tr50, app_token );

	if ( tr50_start(tr50) != 0 )
	{
		printf("tr50_start(): ERROR \n");
		return 1;
	}
	else
		printf("tr50_start(): OK\n");

	/* Register callbacks, use the same callback for several
	 * methods */
	tr50_method_register(tr50, "method_1", _method_callback);
	tr50_method_register(tr50, "method_2", _method_callback);

	/* example set attribute */
	tr50_thing_attr_set( tr50, test_attr, test_attr_value );

	while (1)
	{
		/* random telemetry, must be float.  Properties are
		 * dynmically assigned */
		rand_value = rand();
		value = (double)(rand_value % 1000) / 10.0;
		tr50_property_publish( tr50, "my-property", value );

		/* based on the rand value, trigger an alarm */
		if ( rand_value > 50 )
			state = 1;
		else if ( rand_value > 100 )
			state = 2;
		else
			state = 0;
		tr50_alarm_publish(tr50, "alarm1", state);

		sleep(1);
	}

	return 0;
}
