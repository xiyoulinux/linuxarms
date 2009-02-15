CONFIG_DIR = /etc/linuxarms-armserver/
CONFIG_FILE = ./config
EXECUTE_DIR = /usr/bin
RESOURCE_DIR = /usr/share/linuxarms-armserver/
DRIVE_FILE = ./drive
srcdir = ./src
object = armserver
DEPEND = $(srcdir)/*.c $(srcdir)/*.h
$(object):$(DEPEND)
	@echo start compiling armserver...
	(cd ./src;make)
	(cd ./src;mv $(object) ../)
	@echo compiling end,now you can type \"./$(object)\" to run $(object)
clean:
	@echo "clean all binary file..."
	-rm -f $(object)
	(cd ./src;make clean)
install:
	@echo install \"$(object)\"...
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
	@if [ -e $(RESOURCE_DIR) ]; \
	then \
		echo "WARNING:directory $(RESOURCE_DIR) exit..." ; \
	else \
		mkdir $(RESOURCE_DIR); \
	fi
	@if [ -e $(object) ]; \
	then \
		cp $(object) $(EXECUTE_DIR) ;\
	else \
		echo "execute file \"$(object)\" not exist..." ;\
		(make uninstall); \
		exit 1; \
	fi
	cp -r $(CONFIG_FILE) $(CONFIG_DIR)
	cp -r $(DRIVE_FILE) $(RESOURCE_DIR)
	@echo install end.you can type \"make uninstall\" to uninstall \"$(object)\".
uninstall:
	@echo "uninstall \"$(object)\""
	@if [ -e $(CONFIG_DIR) ]; \
	then \
		rm  -r $(CONFIG_DIR) ; \
		echo "rm -r $(CONFIG_DIR) " ; \
	else \
		echo "test directory $(CONFIG_DIR) :not exist..." ; \
	fi
	@if [ -e $(RESOURCE_DIR) ]; \
	then \
		rm -r $(RESOURCE_DIR); \
		echo "rm -r $(RESOURCE_DIR)"; \
	else \
		echo "test directory $(RESOURCE_DIR) :not exist..."; \
	fi
	@if [ -e "$(EXECUTE_DIR)/$(object)" ] ; \
	then \
		rm  $(EXECUTE_DIR)/$(object) ;\
		echo "rm  $(EXECUTE_DIR)/$(object) ";\
	else \
		echo "test exectue file $(EXECUTE_DIR)/$(object) :not exist..." ; \
	fi
