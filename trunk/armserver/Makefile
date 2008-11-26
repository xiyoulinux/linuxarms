src/Myshell:
	@echo start compiling armserver...
	(cd ./src;make)
	(cd ./src;mv armserver ../)
	@echo compiling end,now you can type \"./armserver\" to run armserver
clean:
	@echo rm .o file...
	rm armserver
	(cd ./src;make clean)
	@echo rm end.
install:
	@echo install \"armserver\"...
	mv ./armserver /usr/bin/
	@echo install end.you can uninstall to mv \"armserver\".
uninstall:
	rm  -f /usr/bin/armserver
