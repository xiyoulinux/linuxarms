RESOURCE_DIR=/usr/share/linuxarms-hostclient/
CONFIG_DIR=/etc/linuxarms-hostclient/
EXECUTE_DIR=/usr/bin
PIXMAPS_DIR=pixmaps
DOC_DIR=doc
CONFIG_FILE=./config
OBJECT=hostclient
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
	@if [ -e $(PIXMAPS_DIR) ]; \
	then \
		echo "test directory $(PIXMAPS_DIR) :exist..." ;\
	else \
		echo "test directory $(PIXMAPS_DIR) :not exist..." ; \
		exit 1 ; \
	fi
	@if [ -e $(DOC_DIR) ]; \
	then \
		echo "test directory $(DOC_DIR) :exist..." ;\
	else \
		echo "test directory $(DOC_DIR) :not exist..." ; \
		exit 1 ; \
	fi
	@if [ -e $(OBJECT) ]; \
	then \
		echo "test exectue file $(OBJECT) :exist..." ;\
	else \
		echo "test exectue file $(OBJECT) :not exist..." ; \
		exit 1; \
	fi
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
	@if [ -e $(CONFIG_DIR) ]; \
	then \
		echo "WARNING:directory $(CONFIG_DIR) exist..." ; \
	else \
		mkdir $(CONFIG_DIR) ; \
	fi
	@if [ -e $(RESOURCE_DIR) ]; \
	then \
		echo "WARNING:directory $(RESOURCE_DIR) exist..." ; \
	else \
		mkdir $(RESOURCE_DIR) ; \
	fi
	cp -r $(CONFIG_FILE) $(CONFIG_DIR)
	cp -r $(PIXMAPS_DIR) $(RESOURCE_DIR)
	cp -r $(DOC_DIR) $(RESOURCE_DIR)
	cp $(OBJECT) $(EXECUTE_DIR)
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
	@if [ -e $(RESOURCE_DIR) ]; \
	then \
		rm  -r $(RESOURCE_DIR) ; \
		echo "rm  -r $(RESOURCE_DIR)"; \
	else \
		echo "test directory $(RESOURCE_DIR) :not exist..." ; \
	fi
	@if [ -e "$(EXECUTE_DIR)/$(OBJECT)" ] ; \
	then \
		rm  $(EXECUTE_DIR)/$(OBJECT) ;\
		echo "rm  $(EXECUTE_DIR)/$(OBJECT) ";\
	else \
		echo "test exectue file $(EXECUTE_DIR)/$(OBJECT) :not exist..." ; \
	fi
