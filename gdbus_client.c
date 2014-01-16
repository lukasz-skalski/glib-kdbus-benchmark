/*
 * gdbus_client - simple gdbus benchmark
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
#include <time.h>

#define DBUS_NAME "com.test.app"
#define DBUS_PATH "/com/test/app"
#define DBUS_IFACE "com.test.app"

/*
 * Main function
 */
int main (int argc, char ** argv)
{
  GDBusConnection *connection;
  int k,i,l;
  char *buf = NULL;
  int msg_size[9] = {4096,8192,16384,32768,65536,131072,262144,524288,1048576};
  char *bus_name = "HelloTestApp";

  struct timespec start, end;
  double elapsed;

  connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, NULL);

  for (i=0; i<9; ++i)
    {
      buf = malloc(sizeof(char) * (msg_size[i] + 1));
      for(k=0; k<msg_size[i]; ++k)
        buf[k] = 'a';
      buf[k] = '\0';

      clock_gettime(CLOCK_REALTIME, &start);

      for(l=0; l<1000; l++)
        {
          g_dbus_connection_call_sync (connection,
                                       DBUS_NAME,
                                       DBUS_PATH,
                                       DBUS_IFACE,
                                       "HelloTestApp",
                                       g_variant_new ("(s)",buf),
                                       NULL,
                                       G_DBUS_CALL_FLAGS_NONE,
                                       -1,
                                       NULL,
                                       NULL);
        }

      clock_gettime(CLOCK_REALTIME, &end);
      elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
      g_print("elapsed time : %lf s\n", elapsed);
      free (buf);
    }

  return 0;
}
