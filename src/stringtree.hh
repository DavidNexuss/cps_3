/*
 * =====================================================================================
 *
 *       Filename:  stringtree.hh
 *
 *    Description:  StringTreeDS
 *
 *        Version:  1.0
 *        Created:  05/18/25 08:32:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <memory>
#include <string>
#include <vector>

template <typename T>
class StringTree {
  public:
  struct UniquePathStrip {
    std::string word;
    int         startingindex = 0;
    T           value;

    UniquePathStrip() :
      startingindex(0), value() {}
  };

  private:
  template <typename K>
  using ptr = std::unique_ptr<K>;

  struct Node {
    ptr<Node> nextNodes[2];
    bool      isLeaf = false;
    T         value;

    Node() :
      isLeaf(false), value() {}
  };

  ptr<Node> rootNode;

  void addStringRecursive(const std::string& key, const T& value, ptr<Node>& node, int index) {
    if (!node) node = std::make_unique<Node>();
    if (key.size() == index) {
      node->value  = value;
      node->isLeaf = true;
      return;
    }
    int c = (key[index] == '1') ? 1 : 0;
    if (!node->nextNodes[c]) {
      node->nextNodes[c] = std::make_unique<Node>();
    }
    addStringRecursive(key, value, node->nextNodes[c], index + 1);
  }

  void getPaths(const ptr<Node>& node, UniquePathStrip currentPath, std::vector<UniquePathStrip>& paths) {
    if (!node) return;

    if (node->isLeaf) {
      currentPath.value = node->value;
      paths.push_back(currentPath);
      return;
    }

    if (node->nextNodes[0] && !node->nextNodes[1]) {
      currentPath.word.push_back('0');
      getPaths(node->nextNodes[0], currentPath, paths);

    } else if (!node->nextNodes[0] && node->nextNodes[1]) {
      currentPath.word.push_back('1');
      getPaths(node->nextNodes[1], currentPath, paths);

    } else if (node->nextNodes[0] && node->nextNodes[1]) {
      // Branch 0
      UniquePathStrip leftPath = currentPath;
      leftPath.word.push_back('0');
      getPaths(node->nextNodes[0], leftPath, paths);

      // Branch 1
      UniquePathStrip rightPath;
      rightPath.word.push_back('1');
      rightPath.startingindex = static_cast<int>(currentPath.word.size()) + currentPath.startingindex;
      getPaths(node->nextNodes[1], rightPath, paths);
    }
  }

  public:
  StringTree() {
    rootNode = std::make_unique<Node>();
  }

  void addString(const std::string& key, const T& value) {
    addStringRecursive(key, value, rootNode, 0);
  }

  std::vector<UniquePathStrip> getPaths() {
    std::vector<UniquePathStrip> paths;
    UniquePathStrip              root;
    getPaths(rootNode, root, paths);
    return paths;
  }
};
