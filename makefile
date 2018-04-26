all: clean svr cli
	
svr:
	gcc UDP_Server.c DHCP.c -o svr
cli:
	gcc UDP_Client.c DHCP.c -o cli
clean:
	rm -f svr cli
