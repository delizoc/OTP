
# To run testscript in makefile use this setup and clean. Update PORT1 and PORT2 numbers each run.

setup:
	gcc -g -o enc_server enc_server.c code.c server.c
	gcc -g -o enc_client enc_client.c client.c
	gcc -g -o dec_server dec_server.c code.c server.c
	gcc -g -o dec_client dec_client.c client.c
	gcc -g -o keygen keygen.c
	chmod +x ./p5testscript
	./p5testscript PORT1 PORT2 > mytestresults 2>&1 

	
clean:
	rm enc_client
	rm enc_server
	rm dec_client
	rm dec_server
	rm keygen
	rm mytestresults

