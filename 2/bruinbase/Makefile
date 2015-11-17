SRC = main.cc SqlParser.tab.c lex.sql.c SqlEngine.cc BTreeIndex.cc BTreeNode.cc RecordFile.cc PageFile.cc 
HDR = Bruinbase.h PageFile.h SqlEngine.h BTreeIndex.h BTreeNode.h RecordFile.h SqlParser.tab.h

bruinbase: $(SRC) $(HDR)
	g++ -ggdb -o $(CLION_EXE_DIR)/$@ $(SRC)

lex.sql.c: SqlParser.l
	flex -Psql $(CLION_EXE_DIR)/$<

SqlParser.tab.c: SqlParser.y
	bison -d -psql $(CLION_EXE_DIR)/$<

clean:
	rm -f bruinbase bruinbase.exe *.o *~ lex.sql.c SqlParser.tab.c SqlParser.tab.h 
