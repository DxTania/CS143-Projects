#include <iostream>
#include <string.h>
#include "BTreeNode.h"

using namespace std;

BTLeafNode::BTLeafNode()
{
  memset(buffer, 0, PageFile::PAGE_SIZE);
}

/**
 * A leaf entry corresponds to key, record id pair in the leaf node
 */
void BTLeafNode::readLeafEntry(LeafEntry *entry, int entryNum)
{
  memcpy(entry, buffer + entryNum * sizeof(LeafEntry), sizeof(LeafEntry));
}

void BTLeafNode::writeLeafEntry(LeafEntry *entry, int entryNum)
{
  memcpy(buffer + entryNum * sizeof(LeafEntry), entry, sizeof(LeafEntry));
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

  LeafEntry entry;
  for (numEntries = 0; numEntries < maxEntries; numEntries++) {
    readLeafEntry(&entry, numEntries);
    // stop when key is 0
    if (entry.key == 0) {
      break;
    }
  }

  return numEntries;
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

    if (pos <= numEntries) {
      // Shift all other entries down
      for (int i = numEntries - 1; i >= pos; i--) {
        LeafEntry entry;
        readLeafEntry(&entry, i);
        writeLeafEntry(&entry, i + 1);
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
  LeafEntry entry;
  for (eid = 0; eid < getKeyCount(); eid++) {
    readLeafEntry(&entry, eid);
    if (entry.key == searchKey) {
      return 0;
    } else if (entry.key > searchKey) {
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

  LeafEntry entry;
  readLeafEntry(&entry, eid);

  key = entry.key;
  rid = entry.rid;
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

void BTLeafNode::printNode()
{
  LeafEntry entry;
  for (int i = 0; i < getKeyCount(); i++) {
    readLeafEntry(&entry, i);
    cout << "Key: " << entry.key << " Pid: " << entry.rid.pid << " Sid: " << entry.rid.sid << endl;
  }
}

BTNonLeafNode::BTNonLeafNode()
{
  memset(buffer, 0, PageFile::PAGE_SIZE);
}

/**
 * A non-leaf entry corresponds to a key and PageId linking to child.
 * The node linked to will contain keys greater than or equal to
 */
void BTNonLeafNode::readNonLeafEntry(NonLeafEntry *entry, int entryNum)
{
  memcpy(entry, buffer + sizeof(PageId) + entryNum * sizeof(NonLeafEntry), sizeof(NonLeafEntry));
}

void BTNonLeafNode::writeNonLeafEntry(NonLeafEntry *entry, int entryNum)
{
  memcpy(buffer + sizeof(PageId) + entryNum * sizeof(NonLeafEntry), entry, sizeof(NonLeafEntry));
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

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{
  int numEntries;
  int maxEntries = (PageFile::PAGE_SIZE - sizeof(PageId)) / sizeof(NonLeafEntry);

  NonLeafEntry entry;
  for (numEntries = 0; numEntries < maxEntries; numEntries++) {
    readNonLeafEntry(&entry, numEntries);
    // stop when key is 0
    if (entry.key == 0) {
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
      NonLeafEntry entry;
      readNonLeafEntry(&entry, pos);

      if (key < entry.key) {
        break;
      }
    }

    for (int i = numEntries - 1; i >= pos; i--) {
      NonLeafEntry entry;
      readNonLeafEntry(&entry, i);
      writeNonLeafEntry(&entry, i + 1);
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
{ return 0; }

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{
  int i;
  NonLeafEntry entry;
  for (i = getKeyCount() - 1; i >= 0; i--) {
    readNonLeafEntry(&entry, i);
    if (searchKey >= entry.key) {
      return entry.pid;
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

void BTNonLeafNode::printNode()
{
  PageId first;
  memcpy(&first, buffer, sizeof(PageId));
  cout << first << "; ";

  NonLeafEntry entry;
  for (int i = 0; i < getKeyCount(); i++) {
    readNonLeafEntry(&entry, i);
    cout << "Key:" << entry.key << " Page id:" << entry.pid << "; ";
  }
}