#include "ies/Common/StringTree.hpp"

#include "gtest/gtest.h"

#include "ies/StdUtil/Find.hxx"

namespace ies
{

TEST(StringTree, TreeSpec)
{
    std::string treeSpec = "[0] {root, a, b} [1] {a, c} {b, d, e} [2] {d, f}";
    StringTree treeLevelAnnotated{"root", treeSpec};
    EXPECT_EQ(ToString(treeLevelAnnotated), treeSpec);

    StringTree tree{"root", "{root, a, b} {a, c} {b, d, e} {d, f}"};

    auto leafNodes = tree.GetLeafNodes("root");
    EXPECT_TRUE(Find(leafNodes, "c"));
    EXPECT_TRUE(Find(leafNodes, "f"));
    EXPECT_TRUE(Find(leafNodes, "e"));
    EXPECT_EQ(3u, tree.GetLeafNodes("root").size());

    std::set<std::string> level1Nodes{"a", "b"};
    EXPECT_EQ(level1Nodes, tree.GetNodesByLevel(1));
    std::set<std::string> level2Nodes{"c", "d", "e"};
    EXPECT_EQ(level2Nodes, tree.GetNodesByLevel(2));
    std::set<std::string> level3Nodes{"f"};
    ASSERT_EQ(level3Nodes, tree.GetNodesByLevel(3));
}

TEST(StringTree, CannotCreateSameNameNode)
{
    StringTree tree("root");

    ASSERT_THROW(
        {
            tree.CreateChildNode("root", "root");
        },
        std::runtime_error
    );
}

TEST(StringTree, CannotCreateSameNameChildNode)
{
    StringTree tree("root");
    tree.CreateChildNode("root", "a");
    tree.CreateChildNode("root", "b");

    tree.CreateChildNode("a", "c");

    ASSERT_THROW(
        {
            tree.CreateChildNode("b", "c");
        },
        std::runtime_error
    );
}

TEST(StringTree, HasNode)
{
    StringTree tree("root");

    EXPECT_TRUE(tree.HasNode("root"));
    ASSERT_FALSE(tree.HasNode("a"));
}

TEST(StringTree, GetLeafNodes)
{
    StringTree tree("root");
    tree.CreateChildNode("root", "a");
    tree.CreateChildNode("root", "b");
    tree.CreateChildNode("root", "c");

    tree.CreateChildNode("a", "d");
    tree.CreateChildNode("a", "e");
    tree.CreateChildNode("e", "f");

    tree.CreateChildNode("b", "g");
    tree.CreateChildNode("b", "h");

    EXPECT_EQ(2u, tree.GetLeafNodes("a").size());
    EXPECT_EQ(1u, tree.GetLeafNodes("c").size());
    auto leafNodes = tree.GetLeafNodes("root");
    EXPECT_TRUE(Find(leafNodes, "c"));
    EXPECT_TRUE(Find(leafNodes, "d"));
    EXPECT_TRUE(Find(leafNodes, "f"));
    EXPECT_TRUE(Find(leafNodes, "g"));
    EXPECT_TRUE(Find(leafNodes, "h"));
    auto nodeGLeafNodes = tree.GetLeafNodes("g");
    EXPECT_TRUE(Find(nodeGLeafNodes, "g"));
    ASSERT_EQ(5u, tree.GetLeafNodes("root").size());
}

TEST(StringTree, GetNodesByLevel)
{
    StringTree tree("root");
    tree.CreateChildNode("root", "a");
    tree.CreateChildNode("root", "b");
    tree.CreateChildNode("root", "c");

    tree.CreateChildNode("a", "d");
    tree.CreateChildNode("a", "e");
    tree.CreateChildNode("e", "f");

    tree.CreateChildNode("b", "g");
    tree.CreateChildNode("b", "h");

    //'' level 0    root
    //''       1  a   b  c
    //''       2 d e g h
    //''       3   f
    //'' note: GetNodesByLevel(GetMaxLevel()) is f
    //''       GetLeafNodes("root") is d f g h c (LeafNode is node with empty child)

    EXPECT_EQ(3, tree.GetMaxLevel());
    EXPECT_EQ(1u, tree.GetNodesByLevel(0).size());
    EXPECT_EQ(tree.GetRoot(), *tree.GetNodesByLevel(0).begin());
    std::set<std::string> level1Nodes{"a", "b", "c"};
    EXPECT_EQ(level1Nodes, tree.GetNodesByLevel(1));
    std::set<std::string> level2Nodes{"d", "e", "g", "h"};
    EXPECT_EQ(level2Nodes, tree.GetNodesByLevel(2));
    std::set<std::string> level3Nodes{"f"};
    ASSERT_EQ(level3Nodes, tree.GetNodesByLevel(3));
}

TEST(StringTree, ParentNode)
{
    StringTree tree("root");
    tree.CreateChildNode("root", "a");
    tree.CreateChildNode("root", "b");

    tree.CreateChildNode("a", "c");

    EXPECT_THROW(
        {
            StringTree treeWithEmptyRoot("");
        },
        std::runtime_error
    );

    EXPECT_THROW(
        {
            tree.CreateChildNode("root", "");
        },
        std::runtime_error
    );

    EXPECT_THROW(
        {
            tree.GetParentNode("root");
        },
        std::runtime_error
    );

    EXPECT_EQ("root", tree.GetParentNode("a"));
    EXPECT_EQ("root", tree.GetParentNode("b"));
    ASSERT_EQ("a", tree.GetParentNode("c"));
}

TEST(StringTree, ToString)
{
    std::string treeSpec = "{a, b, c} {b, d} {d, h} {c, e, f, g} {e, i} {g, j}";
    StringTree tree("a", treeSpec);
    std::string canonicalForm = "[0] {a, b, c} [1] {b, d} {c, e, f, g} [2] {d, h} {e, i} {g, j}";

    ASSERT_EQ(canonicalForm, ToString(tree));
}

TEST(StringTree, GetSubtreeNodes)
{
    StringTree tree("root", "{root, a, b} {a, c} {b, d, e} {d, f}");

    auto bSubNodes = tree.GetSubtreeNodes("b");
    std::set<std::string> expectBSubNodes{"b", "d", "e", "f"};
    EXPECT_EQ(expectBSubNodes, bSubNodes);

    auto nodes = tree.GetSubtreeNodes(tree.GetRoot());
    std::set<std::string> expectSubNodes{"a", "b", "c", "d", "e", "f", "root"};
    ASSERT_EQ(expectSubNodes, nodes);
}

TEST(StringTree, SubTree)
{
    StringTree tree("root", "{root, a, b} {a, c} {b, d, e} {d, f}");
    StringTree bSubTree("b", "{b, d, e} {d, f}");

    ASSERT_EQ(ToString(bSubTree), ToString(tree.GetSubtree("b")));
}

TEST(StringTree, DiffSameTree)
{
    StringTree before("root", "{root, a, b} {a, c} {b, d, e} {d, f}");
    StringTree after("root", "{root, a, b} {a, c} {b, d, e} {d, f}");

    auto diffResult = DiffStringTree(before, after);
    std::set<std::string> expectedRemovedNodes;
    std::set<std::string> expectedUpdatedNodes;
    EXPECT_EQ(expectedRemovedNodes, diffResult.RemovedNodes);
    ASSERT_EQ(expectedUpdatedNodes, diffResult.UpdatedNodes);
}

TEST(StringTree, DiffSimilarStringTree)
{
    StringTree before("a", "{a, b, c, e} {c, d}");
    StringTree after("a", "{a, b, d, f} {d, c}");

    auto diffResult = DiffStringTree(before, after);
    std::set<std::string> expectedRemovedNodes{"c", "d", "e"};
    std::set<std::string> expectedUpdatedNodes{"c", "d", "f"};
    EXPECT_EQ(expectedRemovedNodes, diffResult.RemovedNodes);
    ASSERT_EQ(expectedUpdatedNodes, diffResult.UpdatedNodes);
}

TEST(StringTree, DiffStringTree)
{
    StringTree before("a", "{a, b, c} {b, d}");
    StringTree after("e", "{e, f, g, h} {f, d}");

    auto diffResult = DiffStringTree(before, after);
    std::set<std::string> expectedRemovedNodes{"a", "b", "c", "d"};
    std::set<std::string> expectedUpdatedNodes{"d", "e", "f", "g", "h"};
    EXPECT_EQ(expectedRemovedNodes, diffResult.RemovedNodes);
    ASSERT_EQ(expectedUpdatedNodes, diffResult.UpdatedNodes);
}

TEST(StringTree, RemoveLevel)
{
    StringTree tree("root", "{root, a, b, c} {a, d, e} {e, f} {b, g, h}");

    //'' before remove level 2:
    //'' level 0    root
    //''       1  a   b  c
    //''       2 d e g h
    //''       3   f

    //'' after remove level 2:
    //'' level 0    root
    //''       1  a   b  c
    //''       2   f

    tree.RemoveLevel(2);

    std::set<std::string> level1Nodes{"a", "b", "c"};
    EXPECT_EQ(level1Nodes, tree.GetNodesByLevel(1));
    std::set<std::string> level2Nodes{"f"};
    EXPECT_EQ(level2Nodes, tree.GetNodesByLevel(2));
    EXPECT_EQ("a", tree.GetParentNode("f"));
    EXPECT_TRUE(Find(tree.GetChildNodes("a"), "f"));
    ASSERT_EQ(2, tree.GetMaxLevel());
}

TEST(StringTree, RemoveLevelCannotRemoveRoot)
{
    StringTree tree("root", "{root, a, b, c} {a, d, e} {e, f} {b, g, h}");

    ASSERT_THROW(
        {
            tree.RemoveLevel(0);
        },
        std::runtime_error
    );
}

TEST(StringTree, RemoveLevelLeafCase)
{
    StringTree tree("root", "{root, a, b, c} {a, d, e} {e, f} {b, g, h}");

    //'' before remove level 3:
    //'' level 0    root
    //''       1  a   b  c
    //''       2 d e g h
    //''       3   f

    //'' after remove level 3:
    //'' level 0    root
    //''       1  a   b  c
    //''       2 d e g h

    tree.RemoveLevel(3);

    std::set<std::string> level1Nodes{"a", "b", "c"};
    EXPECT_EQ(level1Nodes, tree.GetNodesByLevel(1));
    std::set<std::string> level2Nodes{"d", "e", "g", "h"};
    EXPECT_EQ(level2Nodes, tree.GetNodesByLevel(2));

    ASSERT_EQ(2, tree.GetMaxLevel());
}

TEST(StringTree, RemoveNode)
{
    StringTree tree("root", "{root, a, b, c} {a, d, e} {e, f} {b, g, h}");

    //'' before remove node e:
    //'' level 0    root
    //''       1  a   b  c
    //''       2 d e g h
    //''       3   f

    //'' after remove node e:
    //'' level 0    root
    //''       1  a   b  c
    //''       2 d   g h

    tree.RemoveNode("e");

    std::set<std::string> level1Nodes{"a", "b", "c"};
    EXPECT_EQ(level1Nodes, tree.GetNodesByLevel(1));
    std::set<std::string> level2Nodes{"d", "g", "h"};
    EXPECT_EQ(level2Nodes, tree.GetNodesByLevel(2));

    ASSERT_EQ(2, tree.GetMaxLevel());
}

TEST(StringTree, RenameNode)
{
    StringTree tree("root", "{root, a, b, c} {a, d, e} {e, f} {b, g, h}");

    //'' level 0    root
    //''       1  a   b  c
    //''       2 d e g h
    //''       3   f

    StringTreeRenameList renameList;
    renameList.emplace_back("e", "i");
    renameList.emplace_back("i", "j");

    tree.RenameNodes(renameList);

    std::set<std::string> level1Nodes{"a", "b", "c"};
    EXPECT_EQ(level1Nodes, tree.GetNodesByLevel(1));
    std::set<std::string> level2Nodes{"d", "g", "h", "j"};
    EXPECT_EQ(level2Nodes, tree.GetNodesByLevel(2));

    EXPECT_TRUE(Find(tree.GetChildNodes("a"), "j"));

    ASSERT_EQ("j", tree.GetParentNode("f"));
}

TEST(StringTree, RenameRoot)
{
    StringTree tree("root", "{root, a, b, c} {a, d, e} {e, f} {b, g, h}");
    tree.RenameNode("root", "myNewRoot");
    EXPECT_EQ("myNewRoot", tree.GetRoot());
    ASSERT_EQ("{myNewRoot, a, b, c} {a, d, e} {b, g, h} {e, f}", ToString(tree, false));
}

TEST(StringTree, GetNodesByLevelWithContext)
{
    StringTree tree("root", "{root, a, b, c} {a, d, e} {e, f} {b, g, h}");

    //'' level 0    root
    //''       1  a   b  c
    //''       2 d e g h
    //''       3   f

    std::set<std::string> aLevel1Nodes{"a"};
    EXPECT_EQ(aLevel1Nodes, tree.GetNodesByLevel("a", 1));

    std::set<std::string> aLevel0Nodes{"root"};
    EXPECT_EQ(aLevel0Nodes, tree.GetNodesByLevel("a", 0));

    std::set<std::string> eLevel1Nodes{"a"};
    EXPECT_EQ(eLevel1Nodes, tree.GetNodesByLevel("e", 1));

    std::set<std::string> aLevel2Nodes{"d", "e"};
    ASSERT_EQ(aLevel2Nodes, tree.GetNodesByLevel("a", 2));
}

}
