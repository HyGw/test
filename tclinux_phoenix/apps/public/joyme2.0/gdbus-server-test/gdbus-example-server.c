#include <gio/gio.h>
#include <stdlib.h>
#include <string.h>

static GDBusNodeInfo *device_introspection_data = NULL;

/* Introspection data for the service we are exporting */
static const gchar device_introspection_xml[] =
  "<node>"
  "  <interface name='org.gtk.GDBus.TestInterface'>"
  "    <annotation name='org.gtk.GDBus.Annotation' value='OnInterface'/>"
  "    <annotation name='org.gtk.GDBus.Annotation' value='AlsoOnInterface'/>"
  "    <method name='HelloWorld'>"
  "      <annotation name='org.gtk.GDBus.Annotation' value='OnMethod'/>"
  "      <arg type='s' name='greeting' direction='in'/>"
  "      <arg type='s' name='response' direction='out'/>"
  "    </method>"
  "    <method name='EmitSignal'>"
  "      <arg type='d' name='speed_in_mph' direction='in'>"
  "        <annotation name='org.gtk.GDBus.Annotation' value='OnArg'/>"
  "      </arg>"
  "    </method>"
  "    <method name='GimmeStdout'/>"
  "    <signal name='VelocityChanged'>"
  "      <annotation name='org.gtk.GDBus.Annotation' value='Onsignal'/>"
  "      <arg type='d' name='speed_in_mph'/>"
  "      <arg type='s' name='speed_as_string'>"
  "        <annotation name='org.gtk.GDBus.Annotation' value='OnArg_NonFirst'/>"
  "      </arg>"
  "    </signal>"
  "    <property type='s' name='FluxCapicitorName' access='read'>"
  "      <annotation name='org.gtk.GDBus.Annotation' value='OnProperty'>"
  "        <annotation name='org.gtk.GDBus.Annotation' value='OnAnnotation_YesThisIsCrazy'/>"
  "      </annotation>"
  "    </property>"
  "    <property type='s' name='Title' access='readwrite'/>"
  "    <property type='s' name='ReadingAlwaysThrowsError' access='read'/>"
  "    <property type='s' name='WritingAlwaysThrowsError' access='readwrite'/>"
  "    <property type='s' name='OnlyWritable' access='write'/>"
  "    <property type='s' name='Foo' access='read'/>"
  "    <property type='s' name='Bar' access='read'/>"
  "  </interface>"
  "</node>";

/* ---------------------------------------------------------------------------------------------------- */

static void
device_handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
  if (g_strcmp0 (method_name, "HelloWorld") == 0)
    {
      const gchar *greeting;

      g_variant_get (parameters, "(&s)", &greeting);

      if (g_strcmp0 (greeting, "Return Unregistered") == 0)
        {
          g_dbus_method_invocation_return_error (invocation,
                                                 G_IO_ERROR,
                                                 G_IO_ERROR_FAILED_HANDLED,
                                                 "As requested, here's a GError not registered (G_IO_ERROR_FAILED_HANDLED)");
        }
      else if (g_strcmp0 (greeting, "Return Registered") == 0)
        {
          g_dbus_method_invocation_return_error (invocation,
                                                 G_DBUS_ERROR,
                                                 G_DBUS_ERROR_MATCH_RULE_NOT_FOUND,
                                                 "As requested, here's a GError that is registered (G_DBUS_ERROR_MATCH_RULE_NOT_FOUND)");
        }
      else if (g_strcmp0 (greeting, "Return Raw") == 0)
        {
          g_dbus_method_invocation_return_dbus_error (invocation,
                                                      "org.gtk.GDBus.SomeErrorName",
                                                      "As requested, here's a raw D-Bus error");
        }
      else
        {
          gchar *response;
          response = g_strdup_printf ("You greeted me with '%s'. Thanks!", greeting);
          g_dbus_method_invocation_return_value (invocation,
                                                 g_variant_new ("(s)", response));
          g_free (response);
        }
    }
  else if (g_strcmp0 (method_name, "EmitSignal") == 0)
    {
      GError *local_error;
      gdouble speed_in_mph;
      gchar *speed_as_string;

      g_variant_get (parameters, "(d)", &speed_in_mph);
      speed_as_string = g_strdup_printf ("%g mph!", speed_in_mph);

      local_error = NULL;
      g_dbus_connection_emit_signal (connection,
                                     NULL,
                                     object_path,
                                     interface_name,
                                     "VelocityChanged",
                                     g_variant_new ("(ds)",
                                                    speed_in_mph,
                                                    speed_as_string),
                                     &local_error);
      g_assert_no_error (local_error);
      g_free (speed_as_string);

      g_dbus_method_invocation_return_value (invocation, NULL);
    }
  else if (g_strcmp0 (method_name, "GimmeStdout") == 0)
    {
#if 0 //def G_OS_UNIX
      if (g_dbus_connection_get_capabilities (connection) & G_DBUS_CAPABILITY_FLAGS_UNIX_FD_PASSING)
        {
          GDBusMessage *reply;
          GUnixFDList *fd_list;
          GError *error;

          fd_list = g_unix_fd_list_new ();
          error = NULL;
          g_unix_fd_list_append (fd_list, STDOUT_FILENO, &error);
          g_assert_no_error (error);

          reply = g_dbus_message_new_method_reply (g_dbus_method_invocation_get_message (invocation));
          g_dbus_message_set_unix_fd_list (reply, fd_list);

          error = NULL;
          g_dbus_connection_send_message (connection,
                                          reply,
                                          G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                          NULL, /* out_serial */
                                          &error);
          g_assert_no_error (error);

          g_object_unref (invocation);
          g_object_unref (fd_list);
          g_object_unref (reply);
        }
      else
        {
          g_dbus_method_invocation_return_dbus_error (invocation,
                                                      "org.gtk.GDBus.Failed",
                                                      "Your message bus daemon does not support file descriptor passing (need D-Bus >= 1.3.0)");
        }
#else
      g_dbus_method_invocation_return_dbus_error (invocation,
                                                  "org.gtk.GDBus.NotOnUnix",
                                                  "Your OS does not support file descriptor passing");
#endif
    }
}

static gchar *_global_title = NULL;

static gboolean swap_a_and_b = FALSE;

static GVariant *
device_handle_get_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GError          **error,
                     gpointer          user_data)
{
  GVariant *ret;

  ret = NULL;
  if (g_strcmp0 (property_name, "FluxCapicitorName") == 0)
    {
      ret = g_variant_new_string ("DeLorean");
    }
  else if (g_strcmp0 (property_name, "Title") == 0)
    {
      if (_global_title == NULL)
        _global_title = g_strdup ("Back To C!");
      ret = g_variant_new_string (_global_title);
    }
  else if (g_strcmp0 (property_name, "ReadingAlwaysThrowsError") == 0)
    {
      g_set_error (error,
                   G_IO_ERROR,
                   G_IO_ERROR_FAILED,
                   "Hello %s. I thought I said reading this property "
                   "always results in an error. kthxbye",
                   sender);
    }
  else if (g_strcmp0 (property_name, "WritingAlwaysThrowsError") == 0)
    {
      ret = g_variant_new_string ("There's no home like home");
    }
  else if (g_strcmp0 (property_name, "Foo") == 0)
    {
      ret = g_variant_new_string (swap_a_and_b ? "Tock" : "Tick");
    }
  else if (g_strcmp0 (property_name, "Bar") == 0)
    {
      ret = g_variant_new_string (swap_a_and_b ? "Tick" : "Tock");
    }

  return ret;
}

static gboolean
device_handle_set_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GVariant         *value,
                     GError          **error,
                     gpointer          user_data)
{
  if (g_strcmp0 (property_name, "Title") == 0)
    {
      if (g_strcmp0 (_global_title, g_variant_get_string (value, NULL)) != 0)
        {
          GVariantBuilder *builder;
          GError *local_error;

          g_free (_global_title);
          _global_title = g_variant_dup_string (value, NULL);

          local_error = NULL;
          builder = g_variant_builder_new (G_VARIANT_TYPE_ARRAY);
          g_variant_builder_add (builder,
                                 "{sv}",
                                 "Title",
                                 g_variant_new_string (_global_title));
          g_dbus_connection_emit_signal (connection,
                                         NULL,
                                         object_path,
                                         "org.freedesktop.DBus.Properties",
                                         "PropertiesChanged",
                                         g_variant_new ("(sa{sv}as)",
                                                        interface_name,
                                                        builder,
                                                        NULL),
                                         &local_error);
          g_assert_no_error (local_error);
        }
    }
  else if (g_strcmp0 (property_name, "ReadingAlwaysThrowsError") == 0)
    {
      /* do nothing - they can't read it after all! */
    }
  else if (g_strcmp0 (property_name, "WritingAlwaysThrowsError") == 0)
    {
      g_set_error (error,
                   G_IO_ERROR,
                   G_IO_ERROR_FAILED,
                   "Hello AGAIN %s. I thought I said writing this property "
                   "always results in an error. kthxbye",
                   sender);
    }

  return *error == NULL;
}


/* for now */
static const GDBusInterfaceVTable device_interface_vtable =
{
  device_handle_method_call,
  device_handle_get_property,
  device_handle_set_property
};


/* ---------------------------------------------------------------------------------------------------- */

static GDBusNodeInfo *introspection_data = NULL;
static GDBusInterfaceInfo *manager_interface_info = NULL;
static GDBusInterfaceInfo *block_interface_info = NULL;
static GDBusInterfaceInfo *partition_interface_info = NULL;

/* Introspection data for the service we are exporting */
static const gchar introspection_xml[] =
  "<node>"
  "  <interface name='org.gtk.GDBus.Example.Manager'>"
  "    <method name='Hello'>"
  "      <arg type='s' name='greeting' direction='in'/>"
  "      <arg type='s' name='response' direction='out'/>"
  "    </method>"
  "  </interface>"
  "  <interface name='org.gtk.GDBus.Example.Block'>"
  "    <method name='Hello'>"
  "      <arg type='s' name='greeting' direction='in'/>"
  "      <arg type='s' name='response' direction='out'/>"
  "    </method>"
  "    <property type='i' name='Major' access='read'/>"
  "    <property type='i' name='Minor' access='read'/>"
  "    <property type='s' name='Notes' access='readwrite'/>"
  "  </interface>"
  "  <interface name='org.gtk.GDBus.Example.Partition'>"
  "    <method name='Hello'>"
  "      <arg type='s' name='greeting' direction='in'/>"
  "      <arg type='s' name='response' direction='out'/>"
  "    </method>"
  "    <property type='i' name='PartitionNumber' access='read'/>"
  "    <property type='s' name='Notes' access='readwrite'/>"
  "  </interface>"
  "</node>";

/* ---------------------------------------------------------------------------------------------------- */

static void
manager_method_call (GDBusConnection       *connection,
                     const gchar           *sender,
                     const gchar           *object_path,
                     const gchar           *interface_name,
                     const gchar           *method_name,
                     GVariant              *parameters,
                     GDBusMethodInvocation *invocation,
                     gpointer               user_data)
{
  const gchar *greeting;
  gchar *response;

  g_assert_cmpstr (interface_name, ==, "org.gtk.GDBus.Example.Manager");
  g_assert_cmpstr (method_name, ==, "Hello");

  g_variant_get (parameters, "(&s)", &greeting);

  response = g_strdup_printf ("Method %s.%s with user_data '%s' on object path %s called with arg '%s'",
                              interface_name,
                              method_name,
                              (const gchar *) user_data,
                              object_path,
                              greeting);
  g_dbus_method_invocation_return_value (invocation,
                                         g_variant_new ("(s)", response));
  g_free (response);
}

const GDBusInterfaceVTable manager_vtable =
{
  manager_method_call,
  NULL,                 /* get_property */
  NULL                  /* set_property */
};

/* ---------------------------------------------------------------------------------------------------- */

static void
block_method_call (GDBusConnection       *connection,
                   const gchar           *sender,
                   const gchar           *object_path,
                   const gchar           *interface_name,
                   const gchar           *method_name,
                   GVariant              *parameters,
                   GDBusMethodInvocation *invocation,
                   gpointer               user_data)
{
  g_assert_cmpstr (interface_name, ==, "org.gtk.GDBus.Example.Block");

  if (g_strcmp0 (method_name, "Hello") == 0)
    {
      const gchar *greeting;
      gchar *response;

      g_variant_get (parameters, "(&s)", &greeting);

      response = g_strdup_printf ("Method %s.%s with user_data '%s' on object path %s called with arg '%s'",
                                  interface_name,
                                  method_name,
                                  (const gchar *) user_data,
                                  object_path,
                                  greeting);
      g_dbus_method_invocation_return_value (invocation,
                                             g_variant_new ("(s)", response));
      g_free (response);
    }
  else if (g_strcmp0 (method_name, "DoStuff") == 0)
    {
      g_dbus_method_invocation_return_dbus_error (invocation,
                                                  "org.gtk.GDBus.TestSubtree.Error.Failed",
                                                  "This method intentionally always fails");
    }
  else
    {
      g_assert_not_reached ();
    }
}

static GVariant *
block_get_property (GDBusConnection  *connection,
                    const gchar      *sender,
                    const gchar      *object_path,
                    const gchar      *interface_name,
                    const gchar      *property_name,
                    GError          **error,
                    gpointer          user_data)
{
  GVariant *ret;
  const gchar *node;
  gint major;
  gint minor;

  node = strrchr (object_path, '/') + 1;
  if (g_str_has_prefix (node, "sda"))
    major = 8;
  else
    major = 9;
  if (strlen (node) == 4)
    minor = node[3] - '0';
  else
    minor = 0;

  ret = NULL;
  if (g_strcmp0 (property_name, "Major") == 0)
    {
      ret = g_variant_new_int32 (major);
    }
  else if (g_strcmp0 (property_name, "Minor") == 0)
    {
      ret = g_variant_new_int32 (minor);
    }
  else if (g_strcmp0 (property_name, "Notes") == 0)
    {
      g_set_error (error,
                   G_IO_ERROR,
                   G_IO_ERROR_FAILED,
                   "Hello %s. I thought I said reading this property "
                   "always results in an error. kthxbye",
                   sender);
    }
  else
    {
      g_assert_not_reached ();
    }

  return ret;
}

static gboolean
block_set_property (GDBusConnection  *connection,
                    const gchar      *sender,
                    const gchar      *object_path,
                    const gchar      *interface_name,
                    const gchar      *property_name,
                    GVariant         *value,
                    GError          **error,
                    gpointer          user_data)
{
  /* TODO */
  g_assert_not_reached ();
}

const GDBusInterfaceVTable block_vtable =
{
  block_method_call,
  block_get_property,
  block_set_property,
};

/* ---------------------------------------------------------------------------------------------------- */

static void
partition_method_call (GDBusConnection       *connection,
                       const gchar           *sender,
                       const gchar           *object_path,
                       const gchar           *interface_name,
                       const gchar           *method_name,
                       GVariant              *parameters,
                       GDBusMethodInvocation *invocation,
                       gpointer               user_data)
{
  const gchar *greeting;
  gchar *response;

  g_assert_cmpstr (interface_name, ==, "org.gtk.GDBus.Example.Partition");
  g_assert_cmpstr (method_name, ==, "Hello");

  g_variant_get (parameters, "(&s)", &greeting);

  response = g_strdup_printf ("Method %s.%s with user_data '%s' on object path %s called with arg '%s'",
                              interface_name,
                              method_name,
                              (const gchar *) user_data,
                              object_path,
                              greeting);
  g_dbus_method_invocation_return_value (invocation,
                                         g_variant_new ("(s)", response));
  g_free (response);
}

const GDBusInterfaceVTable partition_vtable =
{
  partition_method_call,
  //partition_get_property,
  //partition_set_property
};

/* ---------------------------------------------------------------------------------------------------- */

static gchar **
subtree_enumerate (GDBusConnection       *connection,
                   const gchar           *sender,
                   const gchar           *object_path,
                   gpointer               user_data)
{
  gchar **nodes;
  GPtrArray *p;

  g_printf("%s: sender [%s] object_path [%s]\r\n", __FUNCTION__, sender, object_path);

  p = g_ptr_array_new ();
  g_ptr_array_add (p, g_strdup ("sda"));
  g_ptr_array_add (p, g_strdup ("sda1"));
  g_ptr_array_add (p, g_strdup ("sda2"));
  g_ptr_array_add (p, g_strdup ("sda3"));
  g_ptr_array_add (p, g_strdup ("sdb"));
  g_ptr_array_add (p, g_strdup ("sdb1"));
  g_ptr_array_add (p, g_strdup ("sdc"));
  g_ptr_array_add (p, g_strdup ("sdc1"));
  g_ptr_array_add (p, NULL);
  nodes = (gchar **) g_ptr_array_free (p, FALSE);

  return nodes;
}

static GDBusInterfaceInfo **
subtree_introspect (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *node,
                    gpointer               user_data)
{
  GPtrArray *p;
  g_printf("%s: sender [%s] object_path [%s] node[%s]\r\n", __FUNCTION__, sender, object_path, node);

  p = g_ptr_array_new ();
  if (node == NULL)
    {
      g_ptr_array_add (p, g_dbus_interface_info_ref (manager_interface_info));
    }
  else
    {
      g_ptr_array_add (p, g_dbus_interface_info_ref (block_interface_info));
      if (strlen (node) == 4)
        g_ptr_array_add (p,
                         g_dbus_interface_info_ref (partition_interface_info));
    }

  g_ptr_array_add (p, NULL);

  return (GDBusInterfaceInfo **) g_ptr_array_free (p, FALSE);
}

static const GDBusInterfaceVTable *
subtree_dispatch (GDBusConnection             *connection,
                  const gchar                 *sender,
                  const gchar                 *object_path,
                  const gchar                 *interface_name,
                  const gchar                 *node,
                  gpointer                    *out_user_data,
                  gpointer                     user_data)
{
  const GDBusInterfaceVTable *vtable_to_return;
  gpointer user_data_to_return;
  g_printf("%s: sender [%s] object_path [%s]  node[%s] interface_name[%s]\r\n", __FUNCTION__, sender, object_path, node, interface_name);

  if (g_strcmp0 (interface_name, "org.gtk.GDBus.Example.Manager") == 0)
    {
      user_data_to_return = "The Root";
      vtable_to_return = &manager_vtable;
    }
  else
    {
      if (strlen (node) == 4)
        user_data_to_return = "A partition";
      else
        user_data_to_return = "A block device";

      if (g_strcmp0 (interface_name, "org.gtk.GDBus.Example.Block") == 0)
        vtable_to_return = &block_vtable;
      else if (g_strcmp0 (interface_name, "org.gtk.GDBus.Example.Partition") == 0)
        vtable_to_return = &partition_vtable;
      else
        g_assert_not_reached ();
    }

  *out_user_data = user_data_to_return;

  return vtable_to_return;
}

const GDBusSubtreeVTable subtree_vtable =
{
  subtree_enumerate,
  subtree_introspect,
  subtree_dispatch
};

int init_pintest(GDBusConnection *connection);
int init_test(GDBusConnection *connection);

/* ---------------------------------------------------------------------------------------------------- */

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  guint registration_id;
  g_printf("test010:%d!!!", registration_id);

  registration_id = g_dbus_connection_register_subtree (connection,
                                                        "/com/ctc/igd1/Info/Network/WANConnectionDb",
                                                        &subtree_vtable,
                                                        G_DBUS_SUBTREE_FLAGS_NONE,
                                                        NULL,  /* user_data */
                                                        NULL,  /* user_data_free_func */
                                                        NULL); /* GError** */
g_printf("test000:%d!!!", registration_id);
  registration_id = g_dbus_connection_register_object (connection,
                                                       "/com/ctc/igd1/Info/Device",
                                                       device_introspection_data->interfaces[0],
                                                       &device_interface_vtable,
                                                       NULL,  /* user_data */
                                                       NULL,  /* user_data_free_func */
                                                       NULL); /* GError** */
  g_printf("test 1:%d!!!", registration_id);

  g_assert (registration_id > 0);

  init_pintest(connection);
  init_test(connection);
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  g_printf("on_name_lost %s\r\n", name);
  exit (1);
}

int
main (int argc, char *argv[])
{
  guint owner_id;
  GMainLoop *loop;

  /* We are lazy here - we don't want to manually provide
   * the introspection data structures - so we just build
   * them from XML.
   */

  device_introspection_data = g_dbus_node_info_new_for_xml (device_introspection_xml, NULL);
  g_assert (device_introspection_data != NULL);

  introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
  g_assert (introspection_data != NULL);

  manager_interface_info = g_dbus_node_info_lookup_interface (introspection_data, "org.gtk.GDBus.Example.Manager");
  block_interface_info = g_dbus_node_info_lookup_interface (introspection_data, "org.gtk.GDBus.Example.Block");
  partition_interface_info = g_dbus_node_info_lookup_interface (introspection_data, "org.gtk.GDBus.Example.Partition");
  g_assert (manager_interface_info != NULL);
  g_assert (block_interface_info != NULL);
  g_assert (partition_interface_info != NULL);

  owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                             "org.win847.testdbus",
                             G_BUS_NAME_OWNER_FLAGS_REPLACE,
                             on_bus_acquired,
                             on_name_acquired,
                             on_name_lost,
                             NULL,
                             NULL);
	g_printf("test ower:%d!\n", owner_id);
  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  g_bus_unown_name (owner_id);

  g_dbus_node_info_unref (introspection_data);

  return 0;
}

