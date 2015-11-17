/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"
#include <cstdio>
#include <iostream>

int main()
{
  // run the SQL engine taking user commands from standard input (console).
//  SqlEngine::run(stdin);

  RecordId rid = { 1, 2 };
  RecordId rid3 = {3, 4};
  BTreeIndex* index = new BTreeIndex();
  index->open("myindex7", 'w');
  index->insert(10, rid);
  index->insert(20, rid);
  index->insert(30, rid);
  index->insert(40, rid);
  index->insert(50, rid);
  index->insert(60, rid);

  index->insert(70, rid);
  index->insert(80, rid);
  index->insert(90, rid);

//  // test this case overflow non-leaf root node
  index->insert(100, rid);
  index->insert(65, rid3);
  index->insert(66, rid);
  index->printIndex(65);

  IndexCursor cursor;
  index->locate(65, cursor);
  int key;
  RecordId rid2;
  index->readForward(cursor, key, rid2);
  std::cout << rid2.pid;

  index->close();
//  BTNonLeafNode node;
//  node.insert(10, 1);
//  node.insert(20, 2);
//  node.insert(30, 3);
//  BTNonLeafNode sibling;
//  int midKey;
//  node.insertAndSplit(35, 5, sibling, midKey);
//  node.printNode(100);
//  sibling.printNode(200);
//  std::cout << "Mid key " << midKey;

  return 0;
}
