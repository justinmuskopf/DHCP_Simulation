all: clean svr cli
	
svr:
	gcc server.c DHCP.c -o dhcp
cli:
	gcc client.c DHCP.c -o client
clean:
	rm -f dhcp client
