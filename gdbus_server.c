/*
 * gdbus_server - simple gdbus benchmark
 * Copyright (C) 2013 Lukasz Skalski <lukasz.skalski@op.pl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdlib.h>
#include <gio/gio.h>

#define DBUS_NAME "com.test.app"
#define DBUS_PATH "/com/test/app"
#define DBUS_IFACE "com.test.app"

/*
 * Handlers
 */
static void bus_acquired_handler (GDBusConnection *connection, const gchar *name, gpointer user_data);
static void name_acquired_handler (GDBusConnection *connection, const gchar *name, gpointer user_data);
static void name_lost_handler (GDBusConnection *connection, const gchar *name, gpointer user_data);

/*
 * Globals
 */
static GDBusNodeInfo *introspection_data = NULL;


/*
 * method_call_handler
 */
static void method_call_handler (GDBusConnection *connection,
				 const gchar *sender,
				 const gchar *object_path,
				 const gchar *interface_name,
				 const gchar *method_name,
				 GVariant *parameters,
				 GDBusMethodInvocation *invocation,
				 gpointer user_data)
{
  g_dbus_method_invocation_return_value (invocation, g_variant_new("(s)", "OK"));
}


/*
 * Vtable
 */
static const GDBusInterfaceVTable vtable =
{
  method_call_handler
};


/*
 * Main function
 */
int main (int argc, char ** argv)
{
  GMainLoop *g_main_loop;
  guint id;

  gchar xml[] =
  "<node>"
  "  <interface name= 'com.test.app'>"
  "    <method name= 'HelloTestApp'>"
  "      <arg type= 's' name= 'string_in' direction= 'in'/>"
  "      <arg type= 's' name= 'string_out' direction= 'out'/>"
  "    </method>"
  "  </interface>"
  "</node>";

  introspection_data = g_dbus_node_info_new_for_xml (xml, NULL);
  id = g_bus_own_name (G_BUS_TYPE_SESSION,DBUS_NAME,G_BUS_NAME_OWNER_FLAGS_NONE,bus_acquired_handler,name_acquired_handler,name_lost_handler,NULL,NULL);

  g_main_loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (g_main_loop);

  g_bus_unown_name (id);
  g_dbus_node_info_unref (introspection_data);
  return (0);
}


/*
 * Handler to invoke when connected to the bus of type bus_type or NULL
 */
static void bus_acquired_handler (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
  guint registration_id = g_dbus_connection_register_object (connection,DBUS_PATH,introspection_data->interfaces[0],&vtable,NULL,NULL,NULL);
  g_print ("HANDLER: bus_acquired_handler\n");
  g_assert (registration_id > 0);
}


/*
 * Handler to invoke when name is acquired or NULL
 */
static void name_acquired_handler (GDBusConnection *connection,const gchar *name,gpointer user_data)
{
  g_print ("HANDLER: name_acquired_handler\n");
}


/*
 * Handler to invoke when name is lost or NULL
 */
static void name_lost_handler (GDBusConnection *connection,const gchar *name,gpointer user_data)
{
  g_print ("HANDLER: name_lost_handler\n");
  exit(1);
}
