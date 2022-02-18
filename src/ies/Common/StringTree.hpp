#pragma once

//'' workaround for warning C4251: class T needs to have dll-interface to be used by clients of class T.
//'' just disable warning, not fix it properly currently
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "ies/ies_export.h"

#include <list>
#include <map>
#include <set>
#include <string>

namespace ies
{

//! @brief Internal node in a StringTree. Client of StringTree only use node as simple string name.
struct StringTreeNode
{
    int Level;
    std::set<std::string> ChildNodeNames;
    std::string ParentNodeName;

        StringTreeNode(int level, std::string parentNodeName);
};

//! @brief List of {FromName, ToName} pairs.
using StringTreeRenameList = std::list<std::pair<std::string, std::string>>;

//! @brief StringTree is a tree data structure of unique named nodes. Nodes are identified by string name.
//! Tree has utility for access with node level, starting from root.level=0.
//! Must have at least root node and cannot remove root node. MaxLevel is at least 0 (or size of depth = 1).
//! @note It's not for hierarchy have non-unique nodes.
//! (e.g. directory content, you can name two directory with same name at different place)
//! @example
//! StringTree {"a", "{a, b, c} {b, d} {c, e, f, g}"}
//! Level
//! 0              a (root)
//!              .   .
//!             |     |
//! 1           b     c
//!             .   . . .
//!             |  |  |  |
//! 2           d  e  f  g
class IES_EXPORT StringTree
{
public:
    //! @brief Construct a StringTree with only root node named [rootName].
        explicit StringTree(std::string rootName);

    //! @brief Convenient constructor to let user specify a tree in a string [treeSpec]. [treeSpec]'s level annotation is ignored.
    //! @param treeSpec: a list of child node mapping {node, childNodeCommaSeparateList} in correct child node creation order.
    //!     It's not required to be level-sorted and name-sorted, as long as each node is already created in previous mapping.
    //!     i.e. It's possible to use a Depth-First treeSpec.
    //! @example
    //! StringTree
    //! Level
    //! 0              a (root)
    //!              .   .
    //!             |     |
    //! 1           b     c
    //!             .  .  .  .
    //!             |  |  |  |
    //! 2           d  e  f  g
    //!             |  |     |
    //! 3           h  i     j
    //! Can be created by any following treeSpec:
    //! level-sorted: {a, b, c} {c, e, f, g} {b, d} {g, j} {e, i} {d, h}
    //! level-sorted, name sorted (canonical form): {a, b, c} {b, d} {c, e, f, g} {d, h} {e, i} {g, j}
    //!     = ToString(tree, levelAnnotated=false).
    //! depth-first: {a, b, c} {b, d} {d, h} {c, e, f, g} {e, i} {g, j}
    //! ToString(tree): [0] {a, b, c} [1] {b, d} {c, e, f, g} [2] {d, h} {e, i} {g, j}
    //! @note By definition root is already in treeSpec, two parameter to avoid confusion with ctor StringTree(root).
        StringTree(const std::string &rootName,
                   const std::string &treeSpec);

        const std::string &
        GetRoot()
        const;

        bool
        HasNode(const std::string &nodeName)
        const;

    //! @brief Create a childNode of node. Throw exception if [node] not exist or [childNode] already exist.
        void
        CreateChildNode(const std::string &nodeName, const std::string &childNodeName);

    //! @brief Remove a node and its subtree. Does nothing if [node] not exist. Throw exception if [node] is root.
        void
        RemoveNode(const std::string &nodeName);

    //! @brief Rename node. Throw exception if [fromNode] not exist or [toNode] exist.
    //! @note Can rename root.
        void
        RenameNode(const std::string &fromName, const std::string &toName);

    //! @brief Batch rename nodes by [renameList].
    //! @note See RenameNode.
        void
        RenameNodes(const StringTreeRenameList &renameList);

    //! @brief Get level of node. Throw exception if [node] not exist.
        int
        GetLevel(const std::string &nodeName)
        const;

    //! @brief Get childe nodes of node. Throw exception if [node] not exist.
        const std::set<std::string> &
        GetChildNodes(const std::string &nodeName)
        const;

    //! @brief Get parent node of node. Throw exception if [node] not exist or [node] is root.
        const std::string &
        GetParentNode(const std::string &nodeName)
        const;

    //! @brief Get max level among nodes.
        int
        GetMaxLevel()
        const;

        const std::set<std::string> &
        GetNodesByLevel(int level)
        const;

    //! @brief Get all nodes of level related to [context] node.
    //! If level is higher then or equal to context node, return ancestor of context at that level,
    //! else if level is lower then context, return subtree nodes at that level.
    //! @example
    //! StringTree {"a", "{a, b, c} {b, d} {c, e, f, g}"}
    //! Level
    //! 0              a (root)
    //!              .   .
    //!             |     |
    //! 1           b     c
    //!             .   . . .
    //!             |  |  |  |
    //! 2           d  e  f  g
    //! GetNodesByLevel("c", 0) = {"a"}
    //! GetNodesByLevel("c", 1) = {"c"}
    //! GetNodesByLevel("c", 2) = {"e", "f", "g"}
    //! Also GetNodesByLevel(level) = GetNodesByLevel(root, level).
        std::set<std::string>
        GetNodesByLevel(const std::string &context, int level)
        const;

    //! @brief Get all leaf nodes in [node]'s subtree. Leaf node is node without child nodes.
    //! @example
    //! Level
    //! 0              a (root)
    //!              .   .
    //!             |     |
    //! 1           b     c
    //!                .  .  .
    //!                |  |  |
    //! 2              e  f  g
    //!                |
    //! 3              d
    //! LeafNodes = {"b", "d", "f", "g"}
        std::set<std::string>
        GetLeafNodes(const std::string &nodeName)
        const;

    //! @brief Get nodes in subtree of [node] (including node).
        std::set<std::string>
        GetSubtreeNodes(const std::string &nodeName)
        const;

    //! @brief Get subtree of node as a StringTree, which root is node by definition.
        StringTree
        GetSubtree(const std::string &nodeName)
        const;

    //! @brief Remove (squeeze) all nodes in level. Parent level node have each removed node's merged all child level nodes.
    //! @example
    //! Level
    //! 0              a (root)
    //!              .   .
    //!             |     |
    //! 1           b     c
    //!                .  .  .
    //!                |  |  |
    //! 2              e  f  g
    //!                |     |
    //! 3              d     h
    //!
    //! RemoveLevel(2)
    //! Level
    //! 0              a (root)
    //!              .   .
    //!             |     |
    //! 1           b     c
    //!                  . .
    //!                 |   |
    //! 2               d   h
        void
        RemoveLevel(int level);

private:
    //! @brief Name of RootNode.
    std::string mRootName;
    //! @brief Map of {NodeName, Node}. Nodes in tree (including root).
    std::map<std::string, StringTreeNode> mNodes;
    //! @brief Map of {Level, NodeNamesInThisLevel}.
    std::map<int, std::set<std::string>> mLevelToNodesMap;

        std::string
        GetAncestor(const std::string &nodeName, int distance)
        const;

        const StringTreeNode &
        GetNode(const std::string &nodeName)
        const;

        StringTreeNode &
        GetNode(const std::string &nodeName);
};

struct IES_EXPORT StringTreeDiffResult
{
    //! @brief Nodes that are removed.
    std::set<std::string> RemovedNodes;
    //! @brief Nodes that have anything in subtree changed.
    std::set<std::string> UpdatedNodes;
};

//! @brief Convert StringTree to string formatted by treeSpec '[level] {node, childNodeCommaSeparateList}'.
//! e.g. StringTree tree{"a", "{a, b, c} {b, d} {c, e, f, g}"}
//! ToString(tree) = "[0] {a, b, c} [1] {b, d} {c, e, f, g}".
//! childNode is sorted so ToString() is canonical.
IES_EXPORT
std::string
ToString(const StringTree &tree, bool levelAnnotated=true);

//! @brief Compare two StringTrees [before] and [after] and store in DiffResult.
//! RemovedNodes: nodes that were in [before] and removed in [after].
//! UpdatedNodes: nodes that are both in [before] and [after] but subtree is changed or nodes added in [after].
IES_EXPORT
StringTreeDiffResult
DiffStringTree(const StringTree &before, const StringTree &after);

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
