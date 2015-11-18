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
#include <stdio.h>

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  SqlEngine::run(stdin);
//
//  RecordId rid = { 1, 2 };
//  BTreeIndex* index = new BTreeIndex();
//  remove("myindex");
//  index->open("myindex", 'w');
//  index->insert(109, rid);
//  index->insert(40, rid);
//  index->insert(111, rid);
//
//  index->insert(10, rid);
//  index->printIndex(109);
//  index->insert(80, rid);
//  index->insert(20, rid);
//   // TODO: fix leaf root split next pid
////  index->printIndex(80);
//
//  index->close();
  return 0;
}
