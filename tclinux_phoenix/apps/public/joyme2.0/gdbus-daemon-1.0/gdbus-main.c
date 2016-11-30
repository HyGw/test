#include <gio/gio.h>
#include <stdlib.h>

#ifdef G_OS_UNIX
//#include <gio/gunixfdlist.h>
/* For STDOUT_FILENO */
#include <unistd.h>
#endif

static GDBusNodeInfo *introspection_data = NULL;

/* Introspection data for the service we are exporting */
static const gchar introspection_xml[] =
	"<node>"
	"<interface name='org.myapp.JelariInterface'>"
	"<method name='HelloWorld'>"
	"<arg type='s' name='greeting' direction='in'/>" 
        "<arg type='s' name='response' direction='out'/>"
        "</method>"
        "</interface>" 
         "</node>";

static void
handle_method_call (GDBusConnection * connection,
	const gchar * sender, const gchar * object_path, const gchar * interface_name, const gchar * method_name, GVariant * parameters, GDBusMethodInvocation * invocation, gpointer user_data)
{
	g_printf ("Inside Handle Method Call \n");

	if (g_strcmp0 (method_name, "HelloWorld") == 0)
	{
		const gchar *greeting;

		gchar *response;
		g_variant_get (parameters, "(&s)", &greeting);
		response = g_strdup_printf ("You typed '%s', !!! ", greeting);

		g_dbus_method_invocation_return_value (invocation, g_variant_new ("(s)", response));
//g_print ("Sending %s  -> to the client ", response);

	}
}

static const GDBusInterfaceVTable interface_vtable = {
	handle_method_call,
	NULL,
	NULL
};
int init_pingtest(GDBusConnection *connection);

static void on_bus_acquired (GDBusConnection * connection, const gchar * name, gpointer user_data)
{
	g_print ("\n Inside on_bus_acquired \n");

	guint registration_id;

	registration_id = g_dbus_connection_register_object (connection, "/org/myapp/JelariObject", introspection_data->interfaces[0], &interface_vtable, NULL,	/* user_data */
		NULL,					/* user_data_free_func */
		NULL);					/* GError** */
	g_assert (registration_id > 0);
	
	init_pingtest(connection);
}

static void on_name_acquired (GDBusConnection * connection, const gchar * name, gpointer user_data)
{
	g_print ("\n Inside on_name_acquired \n");
}

static void on_name_lost (GDBusConnection * connection, const gchar * name, gpointer user_data)
{
	g_print ("\n err on_name_lost \n");
	exit (1);
}

static guint test(){
	g_return_val_if_fail (0, 0);

	return 1;
}

int main (int argc, char **argv)
{
	guint owner_id, test_id;
	GMainLoop *loop;

	g_log_set_handler ("all", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL  | G_LOG_FLAG_RECURSION, g_log_default_handler, NULL);
	
	g_type_init ();
	introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
	g_assert (introspection_data != NULL);

	test_id = test();

	g_print ("\n test id is %d\n ", test_id);
	owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM, "com.ctc.igd1", G_BUS_NAME_OWNER_FLAGS_REPLACE, on_bus_acquired, on_name_acquired, on_name_lost, NULL, NULL);

	g_print ("\n Owner id is %d ", owner_id);
	loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (loop);

	g_bus_unown_name (owner_id);

	g_dbus_node_info_unref (introspection_data);

}
