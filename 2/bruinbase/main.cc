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
//  index->insert(10, rid);
//  index->insert(103, rid);
//  index->insert(80, rid);
//  index->insert(102, rid);
//  index->insert(20, rid);
//  index->insert(115, rid);
//  index->insert(70, rid);
//  index->insert(30, rid);
//  index->insert(50, rid);
//  index->insert(112, rid);
//  index->insert(60, rid);
//  index->insert(90, rid);
//  index->insert(101, rid);
//  index->insert(104, rid);
//
//  index->printIndex(-1);
//
//  index->close();
  return 0;
}
