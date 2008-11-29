RESOURCE_DIR=/usr/share/linuxarms-hostclient/
EXECUTE_DIR=/usr/bin
PIXMAPS_DIR=pixmaps
DOC_DIR=doc
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
		echo "test directory $(PIXMAP_DIR) :exist..." ;\
	else \
		echo "test directory $(PIXMAPS_DIR) :not exist..." ; \
		exit ; \
	fi
	@if [ -e $(DOC_DIR) ]; \
	then \
		echo "test directory $(DOC_DIR) :exist..." ;\
	else \
		echo "test directory $(DOC_DIR) :not exist..." ; \
		exit ; \
	fi
	@if [ -e $(OBJECT) ]; \
	then \
		echo "test exectue file $(OBJECT) :exist..." ;\
	else \
		echo "test exectue file $(OBJECT) :not exist..." ; \
		exit ; \
	fi
	@if [ -e $(RESOURCE_DIR) ]; \
	then \
		echo "WARNING:directory $(RESOURCE_DIR) exist..." ; \
		cp -r $(PIXMAPS_DIR) $(RESOURCE_DIR) ; \
		cp -r $(DOC_DIR) $(RESOURCE_DIR) ; \
	else \
		mkdir $(RESOURCE_DIR) ; \
		cp -r $(PIXMAPS_DIR) $(RESOURCE_DIR) ; \
		cp -r $(DOC_DIR) $(RESOURCE_DIR) ; \
	fi
	cp $(OBJECT) $(EXECUTE_DIR)
	@echo install end.you can type \"make uninstall\" to uninstall \"$(OBJECT)\".
uninstall:
	@if [ -e $(RESOURCE_DIR) ]; \
	then \
		rm  -r $(RESOURCE_DIR) ; \
	else \
		echo "test directory $(RESOURCE_DIR) :not exist..." ; \
	fi
	@if [ -e "$(EXECUTE_DIR)/$(OBJECT)" ] ; \
	then \
		rm  $(EXECUTE_DIR)/$(OBJECT) ;\
	else \
		echo "test exectue file $(EXECUTE_DIR)/$(OBJECT) :not exist..." ; \
	fi
