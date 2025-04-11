include config.mk

generate_lib: libstructs_data_c.a $(TARGET).a
	ar -t $(TARGET).a

generate_lib_debug: libstructs_data_c_debug.a $(TARGET)_debug.a
	ar -t $(TARGET).a

all: generate_lib
	$(MAKE) -C . -f $(MAKE_NAME) examples

examples: libCDirExplorer_debug.a
	$(CC) examples/code.c $(CFLAGS_EXAMPLES) -o code.$(EXTENSION)
examples_debug: generate_lib_debug
	$(CC) examples/code.c $(CFLAGS_EXAMPLES_DEBUG) -o code.$(EXTENSION)

libstructs_data_c.a:
	echo "generando librerias estatica... $@"
	$(MAKE) -C ./$(PATH_STRUCTS_DATA_C) -f $(MAKE_NAME)

libstructs_data_c_debug.a:
	echo "generando librerias estatica... $@"
	$(MAKE) -C ./$(PATH_STRUCTS_DATA_C) -f $(MAKE_NAME) generate_lib_debug

$(TARGET).a: $(OBJECTS)
	echo "generando librerias estatica... $@"
	$(ARR) $(ARR_FLAGS) $@ $^
	ranlib $@

$(TARGET)_debug.a: $(OBJECTS_DEBUG)
	$(ARR) $(ARR_FLAGS) $(TARGET).a $^
	ranlib $(TARGET).a

CDirExplorer.o: $(PATH_SRC)/CDirExplorer.c
	$(CC) $(CFLAGS) -c $^ -o $@

CDirExplorer_debug.o: $(PATH_SRC)/CDirExplorer.c
	$(CC) $(CFLAGS_DEBUG) -c $^ -o $@

cleanobj:
	$(RM) $(RMFLAGS) $(OBJECTS)
	$(MAKE) -C ./$(PATH_STRUCTS_DATA_C) -f $(MAKE_NAME) cleanobj

cleanall: cleanobj
	$(RM) $(RMFLAGS) *.o $(TARGET).a \
	$(TARGET_DEBUG).a *.$(EXTENSION) 
	$(MAKE) -C ./$(PATH_STRUCTS_DATA_C) -f $(MAKE_NAME) cleanall

.SILENT: clean cleanobj cleanall
.IGNORE: cleanobj cleanall
.PHONY:  cleanobj cleanall