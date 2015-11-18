/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  RecordId   rid;  // record cursor for table scanning

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }

  bool index = false;
  int keyStart = -1; // TODO: support negative keys
  IndexCursor cursor;
  rid.pid = rid.sid = 0;

  BTreeIndex btree;
  if (btree.open(table + ".index", 'r') == 0) {
    index = true;

    for (long i = 0; i < cond.size(); i++) {
      // select on key value, use first condition as starting point
      if (cond[i].attr == 1) {
        switch (cond[i].comp) {
          case SelCond::EQ:
          case SelCond::GT:
          case SelCond::GE:
            keyStart = atoi(cond[i].value);
            break;
          default:
            // try to find eq or gt/ge
            continue;
        }
        break;
      }
    }

    // default keyStart is at beginning of tree, if no eq, gt, ge condition
    btree.locate(keyStart, cursor);
    rc = btree.readForward(cursor, key, rid); // error?
  }

  count = 0;
  while (index || rid < rf.endRid()) {
    bool lastTuple = rc == RC_END_OF_TREE;
    if (rc == RC_NO_SUCH_RECORD) {
      // skip to next valid record
      goto next_tuple;
    }

    // read the tuple
    if ((rc = rf.read(rid, key, value)) < 0) {
      fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
      goto exit_select;
    }

    // check the conditions on the tuple
    for (unsigned i = 0; i < cond.size(); i++) {
      // compute the difference between the tuple value and the condition value
      switch (cond[i].attr) {
        case 1:
          diff = key - atoi(cond[i].value);
          break;
        case 2:
          diff = strcmp(value.c_str(), cond[i].value);
          break;
      }

      // skip the tuple if any condition is not met
      switch (cond[i].comp) {
        case SelCond::EQ:
          if (diff != 0) {
            goto next_tuple;
          }
          else {
            lastTuple = true;
          }
          break;
        case SelCond::NE:
          if (diff == 0) {
            goto next_tuple;
          }
          break;
        case SelCond::GT:
          if (diff <= 0) {
            goto next_tuple;
          }
          break;
        case SelCond::LT:
          if (diff >= 0) {
            lastTuple = true;
            goto next_tuple;
          }
          break;
        case SelCond::GE:
          if (diff < 0) {
            goto next_tuple;
          }
          break;
        case SelCond::LE:
          if (diff > 0) {
            lastTuple = true;
            goto next_tuple;
          }
          break;
      }
    }

    // the condition is met for the tuple.
    // increase matching tuple counter
    count++;

    // print the tuple
    switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
    }

    // move to the next tuple
    next_tuple:
      if (index) {
        if (lastTuple) {
          break;
        }
        rc = btree.readForward(cursor, key, rid);
      } else {
        ++rid;
      }
  }

  // print matching tuple count if "select count(*)"
  if (attr == 4) {
    fprintf(stdout, "%d\n", count);
  }
  rc = 0;

  // close the table file and return
  exit_select:
  rf.close();
  btree.close();
  return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  RC status;
  ifstream stream;
  RecordFile tableRF;

  // open file and open / create table record file
  stream.open(loadfile.c_str());
  status = tableRF.open(table + ".tbl", 'w');
  if (status != 0) {
    return status;
  }

  int key;
  RecordId rid;
  string line, value;
  BTreeIndex btree;

  if (index) {
    status = btree.open(table + ".index", 'w');
    if (status < 0) {
      return status;
    }
  }

  // read key, value line by line and append to table and add to index if needed
  while (getline(stream, line)) {
    status = parseLoadLine(line, key, value);
    if(status != 0) {
      return status;
    } else {
      status = tableRF.append(key, value, rid);
      if (status < 0) {
        return status;
      } else if (index) {
        status = btree.insert(key, rid);
        if (status < 0) {
          return status;
        }
      }
    }
  }

  stream.close();
  int rc = tableRF.close();
  if (rc < 0) {
    return rc;
  }

  if (index) {
    rc = btree.close();
  }

  return rc;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
