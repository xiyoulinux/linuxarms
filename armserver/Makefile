CONFIG_DIR=/etc/linuxarms-armserver/
CONFIG_FILE=./config
EXECUTE_DIR=/usr/bin
DRIVE_FILE=./drive
OBJECT=armserver
$(OBJECT):
	@echo start compiling armserver...
	(cd ./src;make)
	(cd ./src;mv $(OBJECT) ../)
	@echo compiling end,now you can type \"./$(OBJECT)\" to run $(OBJECT)
clean:
	@echo rm .o file...
	@if [ -e $(OBJECT) ]; \
	then \
		rm $(OBJECT) ; \
	fi 
	(cd ./src;make clean)
	@echo rm end.
install:
	@echo install \"$(OBJECT)\"...
	@if [ -e $(CONFIG_FILE) ]; \
	then \
		echo "test \"$(CONFIG_FILE)\" directory :exist..." ;\
	else \
		echo "ERROR:can't find \"$(CONFIG_FILE)\" directory..." ;\
		exit 1 ; \
	fi
	@if [ -e "$(CONFIG_FILE)/port.conf" ] ; \
	then \
		echo "test file \"$(CONFIG_FILE)/port.conf\":exist..." ;\
	else \
		echo "ERROR:can't find file  \"$(CONFIG_FILE)/port.conf\" ..." ;\
		exit 1 ; \
	fi
	@if [ -e "$(DRIVE_FILE)/beep/beepdrv.ko" ] ; \
	then \
		echo "test file \"$(DRIVE_FILE)/beep/beepdrv.ko\":exist..." ;\
	else \
		echo "ERROR:can't find file  \"$(DRIVE_FILE)/beep/beepdrv.ko\" ..." ;\
		exit 1 ; \
	fi
	@if [ -e "$(DRIVE_FILE)/led/leddrv.ko" ] ; \
	then \
		echo "test file \"$(DRIVE_FILE)/led/leddrv.ko\":exist..." ;\
	else \
		echo "ERROR:can't find file  \"$(DRIVE_FILE)/led/leddrv.ko\" ..." ;\
		exit 1 ; \
	fi
	@if [ -e $(CONFIG_DIR) ]; \
	then \
		echo "WARNING:directory $(CONFIG_DIR) exist..." ; \
	else \
		mkdir $(CONFIG_DIR) ; \
	fi
	@if [ -e $(OBJECT) ]; \
	then \
		cp $(OBJECT) $(EXECUTE_DIR) ;\
	else \
		echo "execute file \"$(OBJECT)\" not exist..." ;\
		(make uninstall); \
		exit 1; \
	fi
	cp -r $(CONFIG_FILE) $(CONFIG_DIR)
	cp -r $(DRIVE_FILE) $(CONFIG_DIR)
	@echo install end.you can type \"make uninstall\" to uninstall \"$(OBJECT)\".
uninstall:
	@echo "uninstall \"$(OBJECT)\""
	@if [ -e $(CONFIG_DIR) ]; \
	then \
		rm  -r $(CONFIG_DIR) ; \
		echo "rm -r $(CONFIG_DIR) " ; \
	else \
		echo "test directory $(CONFIG_DIR) :not exist..." ; \
	fi
	@if [ -e "$(EXECUTE_DIR)/$(OBJECT)" ] ; \
	then \
		rm  $(EXECUTE_DIR)/$(OBJECT) ;\
		echo "rm  $(EXECUTE_DIR)/$(OBJECT) ";\
	else \
		echo "test exectue file $(EXECUTE_DIR)/$(OBJECT) :not exist..." ; \
	fi
