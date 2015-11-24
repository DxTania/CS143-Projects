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
  rootPid = 0;
  if (pf.open(indexname, mode) == 0) {
    if (pf.endPid() == 0) {
      // create the index
      BTLeafNode leaf;
      return leaf.write(rootPid, pf);
    }
    return 0;
  }

  return RC_FILE_OPEN_FAILED;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
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
  char buffer[PageFile::PAGE_SIZE];
  pf.read(pid, buffer);
  bool isLeaf;
  memcpy(&isLeaf, buffer + PageFile::PAGE_SIZE - 1, 1);

  if (!isLeaf) {
    // non leaf nodes
    BTNonLeafNode* node = (BTNonLeafNode*) buffer;

    PageId childPid;
    node->locateChildPtr(key, childPid);

    int rc = insertTraverse(key, rid, childPid, siblingPid, siblingKey, curHeight + 1);
    if (rc == RC_NODE_FULL) {
      // we split into two

      if (node->isFull()) {
        // overflow, split here too. check if we are root
        BTNonLeafNode sibling;
        int midKey;
        node->insertAndSplit(siblingKey, siblingPid, sibling, midKey);
        siblingPid = pf.endPid();
        sibling.write(siblingPid, pf);
        node->write(pid, pf);

        if (pid == rootPid) {
          // create new root node
          BTNonLeafNode root;
          pid = pf.endPid();
          node->write(pid, pf);

          root.initializeRoot(pid, midKey, siblingPid);
          root.write(rootPid, pf);

        } else {
          // hand back new sibling pid and key
          siblingKey = midKey;
          return RC_NODE_FULL;
        }
      } else {
        // no overflow
        node->insert(siblingKey, siblingPid);
        node->write(pid, pf);
      }
    } else {
      // it was successfully inserted without overflow
    }

  } else {
    // we reached a leaf
    BTLeafNode* leaf = (BTLeafNode*) buffer;

    if (leaf->isFull()) {
      BTLeafNode sibling;
      leaf->insertAndSplit(key, rid, sibling, siblingKey);

      siblingPid = pf.endPid();
      // set sibling next ptr to what leaf was pointing to
      sibling.setNextNodePtr(leaf->getNextNodePtr());
      // set leaf next ptr to sibling pid
      leaf->setNextNodePtr(siblingPid);
      sibling.write(siblingPid, pf);
      leaf->write(pid, pf);

      if (pid == rootPid) {
        // create new root node
        BTNonLeafNode root;
        pid = pf.endPid();
        leaf->write(pid, pf);
        root.initializeRoot(pid, siblingKey, siblingPid);
        root.write(rootPid, pf);

      } else {
        // recursively hands back siblingPid and siblingKey
        return RC_NODE_FULL;
      }

    } else {
      leaf->insert(key, rid);
      leaf->write(pid, pf);
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

  // Go to leaf node start at root
  PageId pid = rootPid;
  char buffer[PageFile::PAGE_SIZE];
  bool isLeaf;

  for(;;) {
    if (pf.read(pid, buffer) < 0) {
      return RC_FILE_READ_FAILED;
    }
    memcpy(&isLeaf, buffer + PageFile::PAGE_SIZE - 1, 1);
    if (isLeaf) {
      // we found the leaf!
      break;
    }
    BTNonLeafNode *nonLeafNode = (BTNonLeafNode*) buffer;
    nonLeafNode->locateChildPtr(searchKey, pid);
  }

  // Read leaf node
  BTLeafNode *leaf = (BTLeafNode*) buffer;
  cursor.pid = pid;
  return leaf->locate(searchKey, cursor.eid);
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
