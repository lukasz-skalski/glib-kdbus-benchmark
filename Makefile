default: gdbus_client gdbus_server 

gdbus_client:
	$(CC) -o gdbus_client gdbus_client.c `pkg-config --cflags --libs glib-2.0 gio-2.0` 

gdbus_server:
	$(CC) -o gdbus_server gdbus_server.c `pkg-config --cflags --libs glib-2.0 gio-2.0`

clean:
	rm -f gdbus_client gdbus_server *~
