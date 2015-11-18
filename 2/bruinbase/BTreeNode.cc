#include <iostream>
#include <string.h>
#include "BTreeNode.h"
#define MAX_KEYS 70;

using namespace std;

BTLeafNode::BTLeafNode()
{
  memset(buffer, 0, PageFile::PAGE_SIZE);
  setNextNodePtr(RC_END_OF_TREE);
}

/**
 * A leaf entry corresponds to key, record id pair in the leaf node
 */
BTLeafNode::LeafEntry* BTLeafNode::readLeafEntry(int entryNum)
{
  return ((LeafEntry*) buffer) + entryNum;
}

void BTLeafNode::writeLeafEntry(LeafEntry *entry, int entryNum)
{
  memcpy(buffer + entryNum * sizeof(LeafEntry), entry, sizeof(LeafEntry));
}

void BTLeafNode::zeroLeafEntry(int entryNum)
{
  memset(buffer + entryNum * sizeof(LeafEntry), 0, sizeof(LeafEntry));
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
  return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{
  return pf.write(pid, buffer);
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
  int numEntries;
  int maxEntries = (PageFile::PAGE_SIZE - sizeof(PageId)) / sizeof(LeafEntry);

  for (numEntries = 0; numEntries < maxEntries; numEntries++) {
    LeafEntry* entry = readLeafEntry(numEntries);
    // stop when key is 0
    if (entry->key == 0) {
      break;
    }
  }

  return numEntries;
}

bool BTLeafNode::isFull()
{
  return getKeyCount() == MAX_KEYS;
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{
  int maxEntries = (PageFile::PAGE_SIZE - sizeof(PageId)) / sizeof(LeafEntry);
  int numEntries = getKeyCount();

  if (numEntries < maxEntries) {

    int pos;
    locate(key, pos);

    if (pos < numEntries) {
      // Shift all other entries down
      for (int i = numEntries - 1; i >= pos; i--) {
        writeLeafEntry(readLeafEntry(i), i + 1);
      }
    }

    // Finally, insert new leaf entry
    LeafEntry leafEntry = { rid, key };
    writeLeafEntry(&leafEntry, pos);

    return 0;
  } else {
    return RC_NODE_FULL;
  }
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{
  int numEntries = getKeyCount();
  bool fixMin = false;
  int siblingStart = (numEntries + 1) / 2;

  int pos;
  locate(key, pos);

  // Ensure half and half
  if (pos == siblingStart) {
    if (numEntries % 2 == 0) {
      // even entries
      fixMin = true;
    } else {
      // odd entries
    }
  } else if (pos > siblingStart) {
    siblingStart++;
  }

  // Copy half the keys into sibling.
  for (int i = siblingStart; i < numEntries; i++) {
    LeafEntry* entry = readLeafEntry(i);
    sibling.insert(entry->key, entry->rid);
    // Remove entry from this leaf
    zeroLeafEntry(i);
  }

  if (pos < siblingStart || fixMin) {
    insert(key, rid);
  } else {
    sibling.insert(key, rid);
  }

  siblingKey = sibling.readLeafEntry(0)->key;

  return 0;
}

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{
  for (eid = 0; eid < getKeyCount(); eid++) {
    LeafEntry* entry = readLeafEntry(eid);
    if (entry->key == searchKey) {
      return 0;
    } else if (entry->key > searchKey) {
      return RC_NO_SUCH_RECORD;
    }
  }

  // search key is greater than all keys
  return RC_NO_SUCH_RECORD;
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
  if (eid < 0 || eid >= getKeyCount()) {
    return RC_NO_SUCH_RECORD;
  }

  LeafEntry* entry = readLeafEntry(eid);
  key = entry->key;
  rid = entry->rid;
  return 0;
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{
  PageId pid;
  memcpy(&pid, buffer + PageFile::PAGE_SIZE - sizeof(PageId), sizeof(PageId));
  return pid;
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{
  memcpy(buffer + PageFile::PAGE_SIZE - sizeof(PageId), &pid, sizeof(PageId));
  return 0;
}

void BTLeafNode::printNode(PageId pid)
{
  cout << "-----Leaf Node " << pid << "----" << endl;

  for (int i = 0; i < getKeyCount(); i++) {
    LeafEntry* entry = readLeafEntry(i);
    cout << "Key: " << entry->key << " Pid: " << entry->rid.pid << " Sid: " << entry->rid.sid << endl;
  }

  cout << getNextNodePtr() << endl;
  cout << "-----End Node----" << endl;
}

BTNonLeafNode::BTNonLeafNode()
{
  memset(buffer, 0, PageFile::PAGE_SIZE);
}

/**
 * A non-leaf entry corresponds to a key and PageId linking to child.
 * The node linked to will contain keys greater than or equal to
 */
BTNonLeafNode::NonLeafEntry* BTNonLeafNode::readNonLeafEntry(int entryNum)
{
  return ((NonLeafEntry*) (buffer + sizeof(PageId))) + entryNum;
}

void BTNonLeafNode::writeNonLeafEntry(NonLeafEntry *entry, int entryNum)
{
  memcpy(buffer + sizeof(PageId) + entryNum * sizeof(NonLeafEntry), entry, sizeof(NonLeafEntry));
}

void BTNonLeafNode::zeroNonLeafEntry(int entryNum)
{
  memset(buffer + sizeof(PageId) + entryNum * sizeof(NonLeafEntry), 0, sizeof(NonLeafEntry));
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{
  return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{
  return pf.write(pid, buffer);
}

bool BTNonLeafNode::isFull()
{
  return getKeyCount() == MAX_KEYS;
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{
  int numEntries;
  int maxEntries = (PageFile::PAGE_SIZE - sizeof(PageId)) / sizeof(NonLeafEntry);

  for (numEntries = 0; numEntries < maxEntries; numEntries++) {
    NonLeafEntry* entry = readNonLeafEntry(numEntries);
    // stop when key is 0
    if (entry->key == 0) {
      break;
    }
  }

  return numEntries;
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{
  int numEntries = getKeyCount();
  int maxEntries = (PageFile::PAGE_SIZE - sizeof(PageId)) / sizeof(NonLeafEntry);

  if (numEntries < maxEntries) {

    int pos;
    for (pos = 0; pos < numEntries; pos++) {
      NonLeafEntry* entry = readNonLeafEntry(pos);
      if (key < entry->key) {
        break;
      }
    }

    for (int i = numEntries - 1; i >= pos; i--) {
      writeNonLeafEntry(readNonLeafEntry(i), i + 1);
    }

    NonLeafEntry entry = { key, pid };
    writeNonLeafEntry(&entry, pos);
    return 0;

  } else {
    return RC_NODE_FULL;
  }
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{
  int numEntries = getKeyCount();

  int pos;
  for (pos = 0; pos < numEntries; pos++) {
    NonLeafEntry* entry = readNonLeafEntry(pos);
    if (key < entry->key) {
      break;
    }
  }

  // Copy half the keys into sibling. One more key on left side.
  int siblingStart = (numEntries + 1) / 2;
  if (pos == siblingStart) {
    // Don't need to insert this key, but should insert pid
    midKey = key;
    sibling.initializeRoot(pid, 0, 0);
  } else {
    // we have to bubble up a key
    if (pos < siblingStart) {
      siblingStart--;
    }
    midKey = readNonLeafEntry(siblingStart)->key;
    // init left ptr for sibling node
    PageId lastPid;
    memcpy(&lastPid, buffer + 2 * sizeof(PageId) + sizeof(NonLeafEntry) * siblingStart, sizeof(PageId));
    sibling.initializeRoot(lastPid, 0, 0);
  }

  for (int i = siblingStart; i < numEntries; i++) {
    NonLeafEntry* entry = readNonLeafEntry(i);
    if (entry->key != midKey) {
      sibling.insert(entry->key, entry->pid);
    }
    // Remove entry from this leaf
    zeroNonLeafEntry(i);
  }

  // Don't insert key if it was the midkey
  if (pos <= siblingStart && midKey != key) {
    insert(key, pid);
  } else if (pos > siblingStart) {
    sibling.insert(key, pid);
  }

  return 0;
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{
  for (int i = getKeyCount() - 1; i >= 0; i--) {
    NonLeafEntry* entry = readNonLeafEntry(i);
    if (searchKey >= entry->key) {
      pid = entry->pid;
      return 0;
    }
  }

  // search key is less than all keys, return first page id
  memcpy(&pid, buffer, sizeof(PageId));
  return 0;
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{
  memset(buffer, 0, PageFile::PAGE_SIZE);
  NonLeafEntry entry = { key, pid2 };
  writeNonLeafEntry(&entry, 0);
  memcpy(buffer, &pid1, sizeof(PageId));
  return 0;
}

void BTNonLeafNode::printNode(PageId pid)
{
  cout << "-----Node " << pid << "----" << endl;

  PageId first;
  memcpy(&first, buffer, sizeof(PageId));
  cout << "Pid: " << first << "; ";

  for (int i = 0; i < getKeyCount(); i++) {
    NonLeafEntry* entry = readNonLeafEntry(i);
    cout << "Key:" << entry->key << " Pid:" << entry->pid << "; ";
  }

  cout << endl << "-----End Node----" << endl;
}