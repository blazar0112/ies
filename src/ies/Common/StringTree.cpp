#include "ies/Common/StringTree.hpp"

#include <functional>
#include <stdexcept>

#include "ies/Common/IntegralRange.hxx"
#include "ies/StdUtil/Find.hxx"
#include "ies/String/SplitString.hpp"

namespace
{
    const std::string NullNodeName;
}

namespace ies
{

StringTreeNode::
StringTreeNode(int level, std::string parentNodeName)
:   Level(level),
    ParentNodeName(std::move(parentNodeName))
{
}

StringTree::
StringTree(std::string rootName)
:   mRootName(std::move(rootName))
{
    if (mRootName==NullNodeName)
    {
        throw std::runtime_error("root cannot be empty.");
    }
    mNodes.emplace(mRootName, StringTreeNode{0, NullNodeName});
    mLevelToNodesMap[0].emplace(mRootName);
}

StringTree::
StringTree(const std::string &rootName,
           const std::string &treeSpec)
:   StringTree(rootName)
{
    auto nodeLists = SplitString("{}", treeSpec);
    for (auto &nodeList : nodeLists)
    {
        if (Find(nodeList, '['))
        {
            continue;
        }
        auto nodes = SplitString(", ", nodeList);
        if (nodes.empty())
        {
            continue;
        }
        auto &node = nodes[0];
        for (auto i : IndexRange{1, nodes.size()})
        {
            auto &childNode = nodes[i];
            CreateChildNode(node, childNode);
        }
    }
}

const std::string &
StringTree::
GetRoot()
const
{
    return mRootName;
}

bool
StringTree::
HasNode(const std::string &nodeName)
const
{
    return Find(mNodes, nodeName).has_value();
}

void
StringTree::
CreateChildNode(const std::string &nodeName, const std::string &childNodeName)
{
    if (childNodeName.empty())
    {
        throw std::runtime_error("childe node cannot be empty.");
    }

    auto findNode = Find(mNodes, nodeName);
    if (!findNode)
    {
        throw std::runtime_error("StringTree::CreateChildNode(): no such node ["+nodeName+"].");
    }

    if (HasNode(childNodeName))
    {
        throw std::runtime_error("StringTree::CreateChildNode(): child node ["+childNodeName+"] already exist.");
    }

    auto &treeNode = findNode.value()->second;
    treeNode.ChildNodeNames.emplace(childNodeName);
    auto childNodeLevel = treeNode.Level+1;
    mNodes.emplace(childNodeName, StringTreeNode{childNodeLevel, nodeName});
    mLevelToNodesMap[childNodeLevel].emplace(childNodeName);
}

void
StringTree::
RemoveNode(const std::string &nodeName)
{
    if (!HasNode(nodeName))
    {
        return;
    }

    if (nodeName==mRootName)
    {
        throw std::runtime_error("cannot remove root.");
    }

    auto &node = mNodes.at(nodeName);
    auto nodeLevel = node.Level;
    auto &parentNode = mNodes.at(node.ParentNodeName);
    parentNode.ChildNodeNames.erase(nodeName);

    auto subtree = GetSubtree(nodeName);
    if (subtree.GetMaxLevel()>0)
    {
        for (auto level : IntRange{1, subtree.GetMaxLevel()+1})
        {
            auto subNodeLevel = level+nodeLevel;
            for (auto &subNode : subtree.GetNodesByLevel(level))
            {
                mNodes.erase(subNode);
                mLevelToNodesMap.at(subNodeLevel).erase(subNode);
            }
        }
    }

    mNodes.erase(nodeName);
    mLevelToNodesMap.at(nodeLevel).erase(nodeName);

    //'' after removing nodes related to node, may cause level empty nodes.
    //'' shrink mLevelToNodesMap by checking from bottom to top.
    auto size = static_cast<int>(mLevelToNodesMap.size());
    for (auto level : ReverseIntRange{0, size})
    {
        if (!mLevelToNodesMap.at(level).empty())
        {
            break;
        }

        mLevelToNodesMap.erase(level);
    }
}

void
StringTree::
RenameNode(const std::string &fromName, const std::string &toName)
{
    try
    {
        if (!HasNode(fromName))
        {
            throw std::runtime_error("not exist node ["+fromName+"] to rename.");
        }
        if (HasNode(toName))
        {
            throw std::runtime_error("cannot rename to already existing node ["+toName+"].");
        }

        if (fromName==mRootName)
        {
            mRootName = toName;
        }

        auto node = mNodes.at(fromName);
        mNodes.erase(fromName);
        mNodes.emplace(toName, node);
        mLevelToNodesMap.at(node.Level).erase(fromName);
        mLevelToNodesMap.at(node.Level).emplace(toName);

        if (node.ParentNodeName!=NullNodeName)
        {
            auto &parentNode = mNodes.at(node.ParentNodeName);
            parentNode.ChildNodeNames.erase(fromName);
            parentNode.ChildNodeNames.emplace(toName);
        }

        for (auto &childNode : node.ChildNodeNames)
        {
            mNodes.at(childNode).ParentNodeName = toName;
        }
    }
    catch (const std::exception &e)
    {
        auto message =  "StringTree::RenameNode(): encounter exception:\n    "
                        +std::string(e.what());
        throw std::runtime_error(message);
    }
}

void
StringTree::
RenameNodes(const StringTreeRenameList &renameList)
{
    for (auto &[from, to] : renameList)
    {
        RenameNode(from, to);
    }
}

int
StringTree::
GetLevel(const std::string &nodeName)
const
{
    return GetNode(nodeName).Level;
}

const std::set<std::string> &
StringTree::
GetChildNodes(const std::string &nodeName)
const
{
    return GetNode(nodeName).ChildNodeNames;
}

const std::string &
StringTree::
GetParentNode(const std::string &nodeName)
const
{
    if (nodeName==mRootName)
    {
        throw std::runtime_error("cannot get root parent node.");
    }
    return GetNode(nodeName).ParentNodeName;
}

int
StringTree::
GetMaxLevel()
const
{
    if (mLevelToNodesMap.empty())
    {
        throw std::runtime_error("mLevelToNodesMap is empty");
    }
    return (*mLevelToNodesMap.crbegin()).first;
}

const std::set<std::string> &
StringTree::
GetNodesByLevel(int level)
const
{
    auto findLevel = Find(mLevelToNodesMap, level);
    if (!findLevel)
    {
        throw std::runtime_error("level is incorrect, level ["+std::to_string(level)+"], MaxLevel ["
                                 +std::to_string(GetMaxLevel())+"].");
    }
    return findLevel.value()->second;
}

std::set<std::string>
StringTree::
GetNodesByLevel(const std::string &context, int level)
const
{
    if (!HasNode(context))
    {
        throw std::runtime_error("StringTree::GetNodesByLevel(): has no context node ["+context+"].");
    }

    auto contextLevel = GetLevel(context);

    if (level<=contextLevel)
    {
        return {GetAncestor(context, contextLevel-level)};
    }

    std::set<std::string> descendantsOfLevel;
    for (auto &node : GetNodesByLevel(level))
    {
        if (GetAncestor(node, level-contextLevel)==context)
        {
            descendantsOfLevel.emplace(node);
        }
    }

    return descendantsOfLevel;
}

std::set<std::string>
StringTree::
GetLeafNodes(const std::string &nodeName)
const
{
    std::set<std::string> leafNodes;

    auto &childNodes = GetChildNodes(nodeName);
    if (childNodes.empty())
    {
        leafNodes.emplace(nodeName);
        return leafNodes;
    }

    for (auto &childNode : childNodes)
    {
        auto childLeafNodes = GetLeafNodes(childNode);
        for (auto &childLeafNode : childLeafNodes)
        {
            leafNodes.emplace(childLeafNode);
        }
    }

    return leafNodes;
}

std::set<std::string>
StringTree::
GetSubtreeNodes(const std::string &nodeName)
const
{
    std::set<std::string> subtreeNodes;
    auto &childNodes = GetChildNodes(nodeName);

    subtreeNodes.emplace(nodeName);
    for (auto &childNode : childNodes)
    {
        for (auto &subNode : GetSubtreeNodes(childNode))
        {
            subtreeNodes.emplace(subNode);
        }
    }

    return subtreeNodes;
}

StringTree
StringTree::
GetSubtree(const std::string &nodeName)
const
{
    if (!HasNode(nodeName))
    {
        throw std::runtime_error("no such node ["+nodeName+"].");
    }

    StringTree subtree{nodeName};

    std::function<void(const std::string &)> CreateNodeInSubtree;
    CreateNodeInSubtree = [this, &subtree, &CreateNodeInSubtree](const std::string &visitNodeName)
    {
        for (auto &childNode : GetChildNodes(visitNodeName))
        {
            subtree.CreateChildNode(visitNodeName, childNode);
            CreateNodeInSubtree(childNode);
        }
    };

    CreateNodeInSubtree(nodeName);
    return subtree;
}

void
StringTree::
RemoveLevel(int level)
{
    if (level==0)
    {
        throw std::runtime_error("cannot remove root.");
    }

    auto maxLevel = GetMaxLevel();
    if (level>maxLevel)
    {
        return;
    }

    for (auto &nodeName : GetNodesByLevel(level))
    {
        auto &node = mNodes.at(nodeName);
        auto &parentNode = mNodes.at(node.ParentNodeName);
        parentNode.ChildNodeNames.erase(nodeName);
        for (auto &childNodeName : node.ChildNodeNames)
        {
            parentNode.ChildNodeNames.emplace(childNodeName);
            auto &childNode = mNodes.at(childNodeName);
            childNode.ParentNodeName = node.ParentNodeName;
        }

        mNodes.erase(nodeName);
    }
    mLevelToNodesMap.erase(level);

    if (maxLevel>level)
    {
        for (auto adjustLevel : IntRange{level+1, maxLevel+1})
        {
            auto &adjustNodes = GetNodesByLevel(adjustLevel);
            for (auto &nodeName : adjustNodes)
            {
                auto &node = mNodes.at(nodeName);
                node.Level -= 1;
            }

            mLevelToNodesMap[adjustLevel-1] = adjustNodes;
            mLevelToNodesMap.erase(adjustLevel);
        }
    }
}

std::string
StringTree::
GetAncestor(const std::string &nodeName, int distance)
const
{
    auto ancestor = nodeName;
    for (auto &l : IntRange{0, distance, EmptyPolicy::Allow})
    {
        (void)l;
        ancestor = GetParentNode(ancestor);
    }
    return ancestor;
}

const StringTreeNode &
StringTree::
GetNode(const std::string &nodeName)
const
{
    auto findNode = Find(mNodes, nodeName);
    if (!findNode)
    {
        throw std::runtime_error("GetNode(): no such node ["+nodeName+"].");
    }

    return findNode.value()->second;
}

StringTreeNode &
StringTree::
GetNode(const std::string &nodeName)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    return const_cast<StringTreeNode &>(std::as_const(*this).GetNode(nodeName));
}

std::string
ToString(const StringTree &tree, bool levelAnnotated)
{
    std::string canonicalTreeSpec;
    for (auto level : IntRange{0, tree.GetMaxLevel()})
    {
        auto &nodes = tree.GetNodesByLevel(level);
        if (nodes.empty())
        {
            throw std::runtime_error("ToString(StringTree): non-max level have empty nodes.");
        }

        if (level!=0)
        {
            canonicalTreeSpec += " ";
        }

        if (levelAnnotated)
        {
            canonicalTreeSpec += "["+std::to_string(level)+"] ";
        }

        bool isFirstNode = true;
        for (auto &node : nodes)
        {
            auto &childNodes = tree.GetChildNodes(node);
            if (childNodes.empty())
            {
                continue;
            }

            if (!isFirstNode)
            {
                canonicalTreeSpec += " ";
            }
            isFirstNode = false;
            canonicalTreeSpec += "{"+node;
            for (auto &childNode : childNodes)
            {
                canonicalTreeSpec += ", "+childNode;
            }
            canonicalTreeSpec += "}";
        }
    }
    return canonicalTreeSpec;
}

StringTreeDiffResult
DiffStringTree(const StringTree &before, const StringTree &after)
{
    StringTreeDiffResult result;

    if (before.GetRoot()!=after.GetRoot())
    {
        result.RemovedNodes = before.GetSubtreeNodes(before.GetRoot());
        result.UpdatedNodes = after.GetSubtreeNodes(after.GetRoot());
        return result;
    }

    auto &beforeRootChildNodes = before.GetChildNodes(before.GetRoot());
    auto &afterRootChildNodes = after.GetChildNodes(after.GetRoot());

    for (auto &beforeRootChildNode : beforeRootChildNodes)
    {
        if (!Find(afterRootChildNodes, beforeRootChildNode))
        {
            for (auto &node : before.GetSubtreeNodes(beforeRootChildNode))
            {
                result.RemovedNodes.emplace(node);
            }
            continue;
        }

        auto subTreeDiffResult = DiffStringTree(before.GetSubtree(beforeRootChildNode),
                                                after.GetSubtree(beforeRootChildNode));
        for (auto &node : subTreeDiffResult.RemovedNodes)
        {
            result.RemovedNodes.emplace(node);
        }
        for (auto &node : subTreeDiffResult.UpdatedNodes)
        {
            result.UpdatedNodes.emplace(node);
        }
    }

    for (auto &afterRootChildNode : afterRootChildNodes)
    {
        if (!Find(beforeRootChildNodes, afterRootChildNode))
        {
            for (auto &node : after.GetSubtreeNodes(afterRootChildNode))
            {
                result.UpdatedNodes.emplace(node);
            }
        }
    }

    return result;
}

}
