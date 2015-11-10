/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <iostream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
//#include <cstdio>
//#include <iostream>

int main()
{
  // run the SQL engine taking user commands from standard input (console).
//  SqlEngine::run(stdin);

  BTNonLeafNode root;
  root.initializeRoot(0, 20, 2);
  root.insert(30, 3);
  root.insert(50, 5);
  root.insert(40, 4);
  root.insert(10, 1);
//  root.printNode();


  RecordId id;
  id.pid = 1;
  id.sid = 10;
  BTLeafNode leaf;
  leaf.insert(30, id);
  id.pid = 2;

  leaf.insert(20, id);
  leaf.insert(40, id);
  leaf.insert(10, id);
  leaf.printNode();

  return 0;
}
