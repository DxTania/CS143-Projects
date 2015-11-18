/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <string.h>
#include <iostream>

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
  rootPid = -1;
  treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
  char buffer[PageFile::PAGE_SIZE];
  if (pf.open(indexname, mode) == 0) {
    if (pf.endPid() == 0) {
      // create the index
      BTLeafNode leaf;
      treeHeight = 1;
      rootPid = 1;
      memcpy(buffer, &treeHeight, sizeof(int));
      memcpy(buffer + sizeof(int), &rootPid, sizeof(PageId));
      pf.write(0, buffer);
      return leaf.write(rootPid, pf);
    } else {
      // read tree height and root pid
      int rc = pf.read(0, buffer);
      if (rc < 0) {
        return RC_FILE_READ_FAILED;
      }
      memcpy(&treeHeight, buffer, sizeof(PageId));
      memcpy(&rootPid, buffer + sizeof(int), sizeof(PageId));
      return rc;
    }
  }

  return RC_FILE_OPEN_FAILED;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
  char buffer[PageFile::PAGE_SIZE];
  if (pf.read(0, buffer)) {
    return RC_FILE_READ_FAILED;
  }

  // save tree height and root pid
  memcpy(buffer, &treeHeight, sizeof(int));
  memcpy(buffer + sizeof(int), &rootPid, sizeof(PageId));

  if (pf.write(0, buffer) < 0) {
    return RC_FILE_WRITE_FAILED;
  }

  return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
  PageId siblingPid;
  int siblingKey;
  return insertTraverse(key, rid, rootPid, siblingPid, siblingKey, 1);
}

/**
 * Recursively insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @param pid [IN] the page id of the node we're looking at
 * @param siblingPid[OUT] the page id of overflowed sibling
 * @param siblingKey[OUT] the first key the sibling contains
 * @param curHeight[IN] the height we are in the tree
 * @return error code. 0 if no error
 */
RC BTreeIndex::insertTraverse(int key, RecordId rid, PageId pid, PageId &siblingPid, int &siblingKey, int curHeight)
{
  if (curHeight < treeHeight) {
    // non leaf nodes
    BTNonLeafNode node;
    node.read(pid, pf);

    PageId childPid;
    node.locateChildPtr(key, childPid);

    int rc = insertTraverse(key, rid, childPid, siblingPid, siblingKey, curHeight + 1);
    if (rc == RC_NODE_FULL) {
      // we split into two

      if (node.isFull()) {
        // overflow, split here too. check if we are root
        BTNonLeafNode sibling;
        int midKey;
        node.insertAndSplit(siblingKey, siblingPid, sibling, midKey);
        siblingPid = pf.endPid();
        sibling.write(siblingPid, pf);
        node.write(pid, pf);

        if (pid == rootPid) {
          // create new root node
          BTNonLeafNode root;
          rootPid = pf.endPid();
          root.initializeRoot(pid, midKey, siblingPid);
          root.write(rootPid, pf);
          treeHeight++;

        } else {
          // hand back new sibling pid and key
          siblingKey = midKey;
          return RC_NODE_FULL;
        }
      } else {
        // no overflow
        node.insert(siblingKey, siblingPid);
        node.write(pid, pf);
      }
    } else {
      // it was successfully inserted without overflow
    }

  } else {
    // we reached a leaf
    BTLeafNode leaf;
    leaf.read(pid, pf);

    if (leaf.isFull()) {
      BTLeafNode sibling;
      leaf.insertAndSplit(key, rid, sibling, siblingKey);

      siblingPid = pf.endPid();
      // set sibling next ptr to what leaf was pointing to
      sibling.setNextNodePtr(leaf.getNextNodePtr());
      // set leaf next ptr to sibling pid
      leaf.setNextNodePtr(siblingPid);
      sibling.write(siblingPid, pf);
      leaf.write(pid, pf);

      if (pid == rootPid) {
        // create new root node
        BTNonLeafNode root;
        rootPid = pf.endPid();
        root.initializeRoot(pid, siblingKey, siblingPid);
        root.write(rootPid, pf);
        treeHeight++;

      } else {
        // recursively hands back siblingPid and siblingKey
        return RC_NODE_FULL;
      }

    } else {
      leaf.insert(key, rid);
      leaf.write(pid, pf);
    }
  }

  return 0;
}

void BTreeIndex::printIndex(int key) {
  int curHeight = 1;

  if (treeHeight == 1) {
    BTLeafNode root;
    root.read(rootPid, pf);
    root.printNode(rootPid);
    return;
  }

  PageId pid = rootPid;
  while (curHeight < treeHeight){
    // non leaf
    BTNonLeafNode node;
    node.read(pid, pf);
    if (key >= 0) {
      node.printNode(pid);
    }
    node.locateChildPtr(key, pid);
    curHeight++;
  }

  // leaf
  BTLeafNode leaf;
  leaf.read(pid, pf);
  if (key == -1) {
    // print count
    int count = 0;
    while (pid != RC_END_OF_TREE) {
      leaf.read(pid, pf);
      count += leaf.getKeyCount();
      pid = leaf.getNextNodePtr();
    }
    std::cout << count;
  } else {
    leaf.printNode(pid);
  }
}

/**
 * Run the standard B+Tree key search algorithm and identify the
 * leaf node where searchKey may exist. If an index entry with
 * searchKey exists in the leaf node, set IndexCursor to its location
 * (i.e., IndexCursor.pid = PageId of the leaf node, and
 * IndexCursor.eid = the searchKey index entry number.) and return 0.
 * If not, set IndexCursor.pid = PageId of the leaf node and
 * IndexCursor.eid = the index entry immediately after the largest
 * index key that is smaller than searchKey, and return the error
 * code RC_NO_SUCH_RECORD.
 * Using the returned "IndexCursor", you will have to call readForward()
 * to retrieve the actual (key, rid) pair from the index.
 * @param key[IN] the key to find
 * @param cursor[OUT] the cursor pointing to the index entry with
 *                    searchKey or immediately behind the largest key
 *                    smaller than searchKey.
 * @return 0 if searchKey is found. Othewise an error code
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{

  // Go to leaf node
  PageId pid = rootPid;
  for (int i = 1; i < treeHeight; i++) {
    BTNonLeafNode nonLeafNode;
    if (nonLeafNode.read(pid, pf) < 0) {
      return RC_FILE_READ_FAILED;
    }
    nonLeafNode.locateChildPtr(searchKey, pid);
  }

  // Read leaf node
  BTLeafNode leaf;
  if (leaf.read(pid, pf) < 0) {
    return RC_FILE_READ_FAILED;
  }

  cursor.pid = pid;
  return leaf.locate(searchKey, cursor.eid);
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
  BTLeafNode leaf;
  if (leaf.read(cursor.pid, pf) < 0) {
    return RC_FILE_READ_FAILED;
  }

  int rc = leaf.readEntry(cursor.eid, key, rid);
  cursor.eid++;
  if (cursor.eid >= leaf.getKeyCount()) {
    cursor.eid = 0;
    cursor.pid = leaf.getNextNodePtr();
  }

  if (cursor.pid == RC_END_OF_TREE) { // use int min instead?
    return RC_END_OF_TREE;
  }
  return rc;
}
